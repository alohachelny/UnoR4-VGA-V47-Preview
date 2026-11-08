// ============================================================
// V47.0: Frame 2px + HARDWARE LINE START
// (SPI1 + DMAC, VRAM 320x240)
// ============================================================
//
// WHY WERE THERE JAGGED PIXELS? (lines shifted by 1-2 pixels)
// Previously, the start of the stream on each line was done by the CPU: 
// a waitAbs() loop spins with 4-5 cycles per iteration, and exiting it 
// randomly delays by 0...5 CPU cycles. At 24 MHz SPI, 5 cycles is 2.4 bits, 
// i.e., 1-2 pixels. The picture depends on the remainder, changing from line 
// to line. Hardware transmission (DMAC) has no jitter: the jaggedness was 
// caused precisely by the START of the first SPDR write.
//
// WHAT WAS DONE:
// Line start is now hardware-driven (HW_START = 1):
// GPT7 (same timer as HSYNC) triggers the GTCCRA compare event (0x08F).
// DMAC1 writes the FIRST word of the line into SPDR. The rest is handled by 
// DMAC0 via the SPI1_TXI event. The CPU is no longer involved in the start.

#include <SPI.h>
#include <FspTimer.h>
#include "bsp_api.h"

#define HW_START        1       // 1 = Hardware line start (GPT7 -> DMAC1), 0 = Software start
#define USE_BSWAP       0       // 0 = Use XOR bit-mask trick, 1 = Use __builtin_bswap32

#define V_TOTAL         525
#define V_SYNC_LINES    3
#define V_BACK          33
#define V_VISIBLE       480
#define ACTIVE_START    (V_SYNC_LINES + V_BACK)
#define ACTIVE_END      (ACTIVE_START + V_VISIBLE)
#define POLL_LINE       (V_TOTAL - 1)
#define CPU_FREQ_HZ     48000000UL
#define US_TO_TICKS(us) ((uint32_t)((us) * (CPU_FREQ_HZ / 1000000UL)))
#define HSYNC_HZ        31475.0 
#define H_SYNC_CYC      US_TO_TICKS(4)

#define DEF_BEG         200
#if HW_START
#define BEG_MIN         150         // Must be large enough to arm DMAC (approx 100-120 cycles)
#else
#define BEG_MIN         0
#endif
#define BEG_MAX         500

#define TOTAL_WORDS     19          // 19 words per line stream
#define VRAM_WORDS      10          // 10 words = 320 pixels
#define VRAM_HEIGHT     240         // Framebuffer height

#define FRAME_THICK     2           // Frame thickness
#define FRAME_INSET     2           // Frame inset from edge

#define M_CLR           0
#define M_SET           1
#define M_XOR           2

int16_t  g_active_beg = DEF_BEG;
int16_t  g_win_left   = 4;          // Window shift in words (0...8)
int16_t  g_win_top    = 100;        // Vertical window shift, lines

uint32_t g_period = 1525;
uint32_t g_t_line = 0;

// Bouncing square 16x16
#define SQUARE_SIZE     16
#define SQ_MIN_X        (FRAME_INSET + FRAME_THICK)
#define SQ_MAX_X        (320 - FRAME_INSET - FRAME_THICK - SQUARE_SIZE)
#define SQ_MIN_Y        (FRAME_INSET + FRAME_THICK)
#define SQ_MAX_Y        (VRAM_HEIGHT - FRAME_INSET - FRAME_THICK - SQUARE_SIZE)
int16_t square_x = 50;
int16_t square_y = 50;
int8_t  square_dx = 3;
int8_t  square_dy = 2;

#define VSYNC_PIN       8
#define SPI_REG         R_SPI1

// Event link numbers (ICU DELSR)
#define EVT_SPI1_TXI    0x0B3       // SPI1 Transmit Buffer Empty
#define EVT_GPT7_CCMPA  0x08F       // GPT7 Compare Match A (hardware line start)
#define ICU_DELSR(n)    (*(volatile uint32_t *)(0x40006280u + 4u * (n)))
#define ICU_IELSR(n)    (*(volatile uint32_t *)(0x40006300u + 4u * (n)))
#define DMA_DMAST       (*(volatile uint8_t  *)(0x40005200u))

// ============================================================
// VRAM: 240 lines of 10 words (320 pixels)
// ============================================================
static uint32_t vram[VRAM_HEIGHT][VRAM_WORDS];

// ============================================================
// 8x8 font, column-major (59 chars: from space to 'Z')
// ============================================================
static const uint8_t font8x8[59][8] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, {0x00,0x00,0x06,0x5F,0x5F,0x06,0x00,0x00},
    {0x00,0x07,0x07,0x00,0x07,0x07,0x00,0x00}, {0x14,0x7F,0x7F,0x14,0x7F,0x7F,0x14,0x00},
    {0x24,0x2E,0x6B,0x6B,0x3A,0x12,0x00,0x00}, {0x46,0x66,0x30,0x18,0x0C,0x66,0x62,0x00},
    {0x30,0x7A,0x4F,0x5D,0x37,0x7A,0x48,0x00}, {0x04,0x07,0x03,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x1C,0x3E,0x63,0x41,0x00,0x00,0x00}, {0x00,0x41,0x63,0x3E,0x1C,0x00,0x00,0x00},
    {0x08,0x2A,0x3E,0x1C,0x1C,0x3E,0x2A,0x08}, {0x08,0x08,0x3E,0x3E,0x08,0x08,0x00,0x00},
    {0x00,0x80,0xE0,0x60,0x00,0x00,0x00,0x00}, {0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00},
    {0x00,0x00,0x60,0x60,0x00,0x00,0x00,0x00}, {0x60,0x30,0x18,0x0C,0x06,0x03,0x01,0x00},
    {0x3E,0x7F,0x71,0x59,0x4D,0x7F,0x3E,0x00}, {0x40,0x42,0x7F,0x7F,0x40,0x40,0x00,0x00},
    {0x62,0x73,0x59,0x49,0x6F,0x66,0x00,0x00}, {0x22,0x63,0x49,0x49,0x7F,0x36,0x00,0x00},
    {0x18,0x1C,0x16,0x13,0x7F,0x7F,0x10,0x00}, {0x27,0x67,0x45,0x45,0x7D,0x39,0x00,0x00},
    {0x3C,0x7E,0x4B,0x49,0x79,0x30,0x00,0x00}, {0x03,0x03,0x71,0x79,0x0F,0x07,0x00,0x00},
    {0x36,0x7F,0x49,0x49,0x7F,0x36,0x00,0x00}, {0x06,0x4F,0x49,0x69,0x3F,0x1E,0x00,0x00},
    {0x00,0x00,0x66,0x66,0x00,0x00,0x00,0x00}, {0x00,0x80,0xE6,0x66,0x00,0x00,0x00,0x00},
    {0x08,0x1C,0x36,0x63,0x41,0x00,0x00,0x00}, {0x24,0x24,0x24,0x24,0x24,0x24,0x00,0x00},
    {0x00,0x41,0x63,0x36,0x1C,0x08,0x00,0x00}, {0x02,0x03,0x51,0x59,0x0F,0x06,0x00,0x00},
    {0x3E,0x7F,0x41,0x5D,0x5D,0x1F,0x1E,0x00}, {0x7C,0x7E,0x13,0x13,0x7E,0x7C,0x00,0x00},
    {0x41,0x7F,0x7F,0x49,0x49,0x7F,0x36,0x00}, {0x1C,0x3E,0x63,0x41,0x41,0x63,0x22,0x00},
    {0x41,0x7F,0x7F,0x41,0x63,0x3E,0x1C,0x00}, {0x41,0x7F,0x7F,0x49,0x5D,0x41,0x63,0x00},
    {0x41,0x7F,0x7F,0x49,0x1D,0x01,0x03,0x00}, {0x1C,0x3E,0x63,0x41,0x51,0x73,0x72,0x00},
    {0x7F,0x7F,0x08,0x08,0x7F,0x7F,0x00,0x00}, {0x00,0x41,0x7F,0x7F,0x41,0x00,0x00,0x00},
    {0x30,0x70,0x40,0x41,0x7F,0x3F,0x01,0x00}, {0x41,0x7F,0x7F,0x08,0x1C,0x77,0x63,0x00},
    {0x41,0x7F,0x7F,0x41,0x40,0x60,0x70,0x00}, {0x7F,0x7F,0x0E,0x1C,0x0E,0x7F,0x7F,0x00},
    {0x7F,0x7F,0x06,0x0C,0x18,0x7F,0x7F,0x00}, {0x1C,0x3E,0x63,0x41,0x63,0x3E,0x1C,0x00},
    {0x41,0x7F,0x7F,0x49,0x09,0x0F,0x06,0x00}, {0x1E,0x3F,0x21,0x71,0x7F,0x5E,0x00,0x00},
    {0x41,0x7F,0x7F,0x09,0x19,0x7F,0x66,0x00}, {0x26,0x6F,0x4D,0x59,0x73,0x32,0x00,0x00},
    {0x03,0x41,0x7F,0x7F,0x41,0x03,0x00,0x00}, {0x7F,0x7F,0x40,0x40,0x7F,0x7F,0x00,0x00},
    {0x1F,0x3F,0x60,0x60,0x3F,0x1F,0x00,0x00}, {0x7F,0x7F,0x30,0x18,0x30,0x7F,0x7F,0x00},
    {0x43,0x67,0x3C,0x18,0x3C,0x67,0x43,0x00}, {0x07,0x4F,0x78,0x78,0x4F,0x07,0x00,0x00},
    {0x47,0x63,0x71,0x59,0x4D,0x67,0x73,0x00}
};

// ============================================================
// GRAPHICS
// USE_BSWAP = 0: bit ((x & 31) ^ 7) - this is the endianness fix!
// USE_BSWAP = 1: bit (31 - (x & 31)) and bswap32 during line generation
// ============================================================
static inline void vram_hspan(int y, int x0, int x1, uint8_t mode) {
    uint32_t *row = vram[y];
    const int b0 = x0 >> 3, b1 = x1 >> 3;
    for (int bx = b0; bx <= b1; bx++) {
        const int a = (bx == b0) ? (x0 & 7) : 0;
        const int b = (bx == b1) ? (x1 & 7) : 7;
        uint32_t m = ((1u << (8 - a)) - 1u) & ~((1u << (7 - b)) - 1u);
#if USE_BSWAP
        m <<= (3 - (bx & 3)) * 8;
#else
        m <<= (bx & 3) * 8;
#endif
        uint32_t *w = &row[bx >> 2];
        if      (mode == M_SET) *w |= m;
        else if (mode == M_CLR) *w &= ~m;
        else                    *w ^= m;
    }
}

void vram_fill_rect(int x, int y, int w, int h, uint8_t mode) {
    if (w <= 0 || h <= 0) return;
    int x1 = x + w - 1, y1 = y + h - 1;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x1 > 319) x1 = 319;
    if (y1 >= VRAM_HEIGHT) y1 = VRAM_HEIGHT - 1;
    if (x > x1 || y > y1) return;
    for (int py = y; py <= y1; py++) vram_hspan(py, x, x1, mode);
}

void vram_draw_frame() {
    const int x0 = FRAME_INSET;
    const int y0 = FRAME_INSET;
    const int w  = 320 - 2 * FRAME_INSET;
    const int h  = VRAM_HEIGHT - 2 * FRAME_INSET;
    vram_fill_rect(x0,                   y0,                   w,           FRAME_THICK, M_SET);
    vram_fill_rect(x0,                   y0 + h - FRAME_THICK, w,           FRAME_THICK, M_SET);
    vram_fill_rect(x0,                   y0,                   FRAME_THICK, h,           M_SET);
    vram_fill_rect(x0 + w - FRAME_THICK, y0,                   FRAME_THICK, h,           M_SET);
}

void vram_draw_char(int x, int y, char c) {
    if (c < ' ' || c > 'Z') return;
    const uint8_t *glyph = font8x8[c - ' '];
    for (int col = 0; col < 8; col++) {
        uint8_t b = glyph[col];
        for (int row = 0; row < 8; row++) {
            if (b & (1 << row)) vram_fill_rect(x + col * 2, y + row * 2, 2, 2, M_SET);
        }
    }
}

void vram_draw_string(int x, int y, const char *str) {
    while (*str) { vram_draw_char(x, y, *str); x += 16; str++; }
}

// ============================================================
// LINE GENERATION: from VRAM to the DMA buffer
// ============================================================
inline void generate_line(uint32_t *dst, uint16_t v_line) {
    for (uint16_t i = 0; i < TOTAL_WORDS; i++) dst[i] = 0;
    if (v_line >= g_win_top && v_line < (g_win_top + VRAM_HEIGHT)) {
        const uint32_t *src = vram[v_line - g_win_top];
        for (uint16_t i = 0; i < VRAM_WORDS; i++) {
#if USE_BSWAP
            dst[g_win_left + i] = __builtin_bswap32(src[i]);
#else
            dst[g_win_left + i] = src[i];
#endif
        }
    }
}

// ============================================================
// SYSTEM & LOW-LEVEL
// ============================================================
static uint32_t buf_a[TOTAL_WORDS];
static uint32_t buf_b[TOTAL_WORDS];
static uint32_t *cur_buf = buf_a;
static uint32_t *nxt_buf = buf_b;

FspTimer hsync_timer;

inline __attribute__((always_inline)) void vsync_high() { R_PORT3->POSR = (1u << 4); }
inline __attribute__((always_inline)) void vsync_low()  { R_PORT3->PORR = (1u << 4); }

// Button macros (active low)
inline __attribute__((always_inline)) bool btn_up()   { return (R_PORT1->PIDR & (1u << 5)) == 0; } // D2
inline __attribute__((always_inline)) bool btn_down() { return (R_PORT1->PIDR & (1u << 4)) == 0; } // D3
inline __attribute__((always_inline)) bool btn_mode() { return (R_PORT1->PIDR & (1u << 3)) == 0; } // D4
inline __attribute__((always_inline)) bool btn_spd()  { return (R_PORT1->PIDR & (1u << 2)) == 0; } // D5
inline __attribute__((always_inline)) bool btn_chg()  { return (R_PORT1->PIDR & (1u << 6)) == 0; } // D6

// Busy-wait until DWT_CYCCNT reaches 'target'
inline __attribute__((always_inline)) void waitAbs(uint32_t target) {
    uint32_t cnt;
    __asm volatile("1: ldr %0, [%1]\n subs %0, %0, %2\n bmi 1b"
                   : "=&r" (cnt) : "r" (&DWT->CYCCNT), "r" (target) : "cc");
}

// Wait for a GPT7 wrap (overflow) and return the exact DWT time of the wrap
inline __attribute__((always_inline)) uint32_t waitGptWrap() {
    uint32_t prev = R_GPT7->GTCNT;
    uint32_t cur;
    uint32_t t0 = DWT->CYCCNT;
    while (true) {
        cur = R_GPT7->GTCNT;
        if (cur < prev) break; // Wrap detected
        prev = cur;
        if ((DWT->CYCCNT - t0) > US_TO_TICKS(100000)) break; // Timeout protection
    }
    return DWT->CYCCNT - cur;
}

void init_dwt_counter() {
    ITM->LAR = 0xC5ACCE55; // Unlock ITM/DWT registers (critical!)
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    __DSB();
}

// Detach FSP SPI interrupts to prevent the framework from interfering with DMA
void detach_spi_irqs() {
    for (uint8_t i = 0; i < 32; i++) {
        uint32_t ev = ICU_IELSR(i) & 0x1FFu;
        if (ev >= 0x0B2 && ev <= 0x0B6) {
            NVIC_DisableIRQ((IRQn_Type)i);
            ICU_IELSR(i) = 0;
        }
    }
}

// Direct SPI1 configuration (bypassing FSP overhead)
void spi_override_config() {
    SPI_REG->SPCR     = 0;
    SPI_REG->SPPCR    = 0;                 // MOIFE = 0 (keep last bit during pauses)
    SPI_REG->SPBR     = 0;                 // 24 MHz
    SPI_REG->SPDCR    = (1u << 5);         // 32-bit access to SPDR
    SPI_REG->SPCKD    = 0;
    SPI_REG->SSLND    = 0;
    SPI_REG->SPND     = 0;
    SPI_REG->SPCR2    = 0;
    SPI_REG->SPCMD[0] = (0x3u << 8);       // 32-bit frame
    // Enable: SPMS, TXMD, MSTR, SPTIE, SPE
    SPI_REG->SPCR     = (1u << 0) | (1u << 1) | (1u << 3) | (1u << 5) | (1u << 6);
}

void setup_dmac() {
    R_MSTP->MSTPCRA &= ~(1u << 22); // Enable DMAC module
    DMA_DMAST = 1;

    // DMAC0: words 1...18 via SPI1_TXI event
    R_DMAC0->DMAMD = 0x8000;               // Source increment, Dest fixed
    R_DMAC0->DMDAR = (uint32_t)&SPI_REG->SPDR;
    R_DMAC0->DMTMD = 0x0201;               // 32-bit, trigger by event

#if HW_START
    // DMAC1: first word via GPT7 Compare Match event (single transfer)
    R_DMAC1->DMAMD = 0x0000;               // Source fixed, Dest fixed
    R_DMAC1->DMDAR = (uint32_t)&SPI_REG->SPDR;
    R_DMAC1->DMTMD = 0x0201;               // 32-bit, trigger by event
#endif
}

inline __attribute__((always_inline)) void dmac_arm_line(const uint32_t *src, uint16_t count) {
    R_DMAC0->DMCNT = 0;
    R_DMAC0->DMSTS = 0x01;                 // Clear TE flag
    R_DMAC0->DMSTS = 0x00;
    R_DMAC0->DMSAR = (uint32_t)src;
    R_DMAC0->DMCRA = count;
    ICU_DELSR(0)   = EVT_SPI1_TXI;         // Link event & clear pending request
    R_DMAC0->DMCNT = 1;
}

#if HW_START
inline __attribute__((always_inline)) void dmac1_arm_start(const uint32_t *first_word) {
    R_DMAC1->DMCNT = 0;
    R_DMAC1->DMSTS = 0x01;
    R_DMAC1->DMSTS = 0x00;
    R_DMAC1->DMSAR = (uint32_t)first_word;
    R_DMAC1->DMCRA = 1;
    ICU_DELSR(1)   = EVT_GPT7_CCMPA;       // Link GPT7 Compare Match A event
    R_DMAC1->DMCNT = 1;
}

// Set the moment when the line starts (GPT7 GTCCRA value)
void set_start_compare(int16_t beg) {
    R_GPT7->GTCCR[0] = (uint32_t)beg;      // GTCCRA
    R_GPT7->GTCCR[2] = (uint32_t)beg;      // GTCCRC: buffer for A if enabled
}
#endif

void setup() {
    pinMode(2, INPUT_PULLUP); pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP); pinMode(5, INPUT_PULLUP); pinMode(6, INPUT_PULLUP);
    delay(100);

    init_dwt_counter();
    memset(vram, 0, sizeof(vram));

    // Draw static scene
    vram_draw_frame();
    vram_draw_string(16, 12, "VGA 320X240");
    vram_draw_string(16, 32, "FRAME + HW START");
    vram_draw_string(16, 52, "V47.0");

    // Initial square (XOR draw)
    vram_fill_rect(square_x, square_y, SQUARE_SIZE, SQUARE_SIZE, M_XOR);

    pinMode(VSYNC_PIN, OUTPUT);
    vsync_high();

    SPI.begin();
    pinMode(12, INPUT);
    pinMode(13, INPUT);
    pinPeripheral(11, (uint32_t)(IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SPI));
    delay(10);

    detach_spi_irqs();
    setup_dmac();
    spi_override_config();

    if (!hsync_timer.begin(TIMER_MODE_PWM, GPT_TIMER, 7, HSYNC_HZ, 50.0)) while (1) {}
    pinPeripheral(9, (uint32_t)(IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_GPT1));
    hsync_timer.enable_pwm_channel(CHANNEL_B);
    hsync_timer.open();

    g_period = (uint32_t)R_GPT7->GTPR + 1;
    hsync_timer.set_duty_cycle(g_period - H_SYNC_CYC, CHANNEL_B);
    hsync_timer.start();

#if HW_START
    // Enable GPT7 Compare Match A event (required for DMAC1)
    R_GPT7->GTINTAD |= 1u;
    set_start_compare(g_active_beg);
#endif

    g_t_line = waitGptWrap();

    // Global interrupt disable to guarantee stable timings (no USB/SysTick jitter)
    noInterrupts(); 
}

__attribute__((long_call, noinline))
void video_forever() {
    const uint32_t period = g_period;
    static uint16_t holdUp = 0, holdDn = 0, holdSpd = 0, holdChg = 0;

    while (true) {
        for (uint16_t line = 0; line < V_TOTAL; line++) {
            if (line == 0) {
                g_t_line = waitGptWrap();
            } else {
                g_t_line += period;
                while ((int32_t)(DWT->CYCCNT - g_t_line) >= (int32_t)period) {
                    g_t_line += period;
                }
            }

            waitAbs(g_t_line);

            if (line == 0)            vsync_low();
            if (line == V_SYNC_LINES) vsync_high();

            // Square update in V-Blank (line 10) to avoid tearing.
            // XOR logic: erase old -> move -> draw new. 
            if (line == 10) {
                vram_fill_rect(square_x, square_y, SQUARE_SIZE, SQUARE_SIZE, M_XOR);
                square_x += square_dx;
                square_y += square_dy;
                if (square_x < SQ_MIN_X) { square_x = SQ_MIN_X; square_dx = -square_dx; }
                if (square_x > SQ_MAX_X) { square_x = SQ_MAX_X; square_dx = -square_dx; }
                if (square_y < SQ_MIN_Y) { square_y = SQ_MIN_Y; square_dy = -square_dy; }
                if (square_y > SQ_MAX_Y) { square_y = SQ_MAX_Y; square_dy = -square_dy; }
                vram_fill_rect(square_x, square_y, SQUARE_SIZE, SQUARE_SIZE, M_XOR);
            }

            // Prepare the first visible line in V-Blank
            if (line == 1) generate_line(cur_buf, 0);

            if (line >= ACTIVE_START && line < ACTIVE_END) {
                const uint16_t v_line = line - ACTIVE_START;

                dmac_arm_line(&cur_buf[1], TOTAL_WORDS - 1);   // DMAC0: words 1...18
#if HW_START
                dmac1_arm_start(&cur_buf[0]);                  // DMAC1: word 0 via GPT7
#endif

                // Wait for the BEG moment. In HW mode, we still wait here 
                // to avoid overwriting the buffer while the previous line is still transmitting.
                waitAbs(g_t_line + (uint32_t)g_active_beg);
#if !HW_START
                SPI_REG->SPDR = cur_buf[0];                    // Software start (fallback)
#endif

                // While DMA transmits, prepare the next line in the second buffer
                if (v_line + 1 < V_VISIBLE) {
                    generate_line(nxt_buf, v_line + 1);
                }
                uint32_t *t = cur_buf; cur_buf = nxt_buf; nxt_buf = t;
            }

            // Poll buttons at the end of the frame (Line 524)
            if (line == POLL_LINE) {
                bool up = btn_up(), dn = btn_down(), md = btn_mode();
                bool sp = btn_spd(), ch = btn_chg();

                // D2 / D3: BEG + / - (auto-repeat if held)
                if (up && !dn && !md && !sp && !ch) {
                    if (++holdUp == 3 || (holdUp > 30 && holdUp % 6 == 0)) {
                        if (g_active_beg < BEG_MAX) {
                            g_active_beg += 1;
#if HW_START
                            set_start_compare(g_active_beg);
#endif
                        }
                    }
                } else holdUp = 0;

                if (dn && !up && !md && !sp && !ch) {
                    if (++holdDn == 3 || (holdDn > 30 && holdDn % 6 == 0)) {
                        if (g_active_beg > BEG_MIN) {
                            g_active_beg -= 1;
#if HW_START
                            set_start_compare(g_active_beg);
#endif
                        }
                    }
                } else holdDn = 0;

                // D5 / D6: Shift window left / right by 1 word (32 pixels), max 8
                if (sp && !up && !dn && !md && !ch) {
                    if (++holdSpd == 5) { if (g_win_left > 0) g_win_left--; }
                } else holdSpd = 0;

                if (ch && !up && !dn && !md && !sp) {
                    if (++holdChg == 5) { if (g_win_left < (TOTAL_WORDS - VRAM_WORDS - 1)) g_win_left++; }
                } else holdChg = 0;
            }
        }
    }
}

void loop() { video_forever(); }

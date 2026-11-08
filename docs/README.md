# Technical Overview: VGA V47.0 (Bare-Metal)

This document briefly explains the low-level architecture of the V47.0 engine.

## 1. Timing & Synchronization

* **HSync:** Generated entirely by hardware using the GPT7 timer. The timer period is set to 1525 ticks (~31.77 µs, corresponding to approximately 31.475 kHz).
* **VSync:** Generated in software during the frame blanking period.
* **Hardware Line Start:** Instead of relying on software delays (`waitAbs`), the engine uses a GPT7 Compare Match event (GTCCRA) to trigger DMAC1. DMAC1 writes the first 32-bit word directly into the SPI data register. This eliminates the "jitter" (0-5 CPU ticks delay) that caused jagged pixels.

## 2. SPI & DMA

* The SPI1 peripheral is configured for 32-bit frames at 24 MHz.
* **DMA scan-out:** The CPU prepares each video line in one of two alternating line buffers (`buf_a` and `buf_b`). DMA transfers the prepared line to SPI1. The first word is started by a GPT7 hardware event, while the remaining words are transferred in response to SPI1 transmit events.
* To handle the Endianness/bit-order of the SPI peripheral, a bitwise XOR mask `(x & 31) ^ 7` is used when writing to VRAM, eliminating the need for expensive `__builtin_bswap32` operations during DMA setup.

## 3. Memory Layout (VRAM)

* The frame buffer is `vram[240][10]`. Each row is 10 words (10 * 32 bits = 320 pixels).
* The horizontal image position is adjusted by changing where the image data is placed in the line buffer.

## 4. V-Blank Updates

* To prevent screen tearing, the bouncing square is updated during `line == 10` (which falls within the VSync back-porch blanking interval). The CPU has plenty of time (hundreds of microseconds) to erase the old square and draw the new one before the visible lines start.

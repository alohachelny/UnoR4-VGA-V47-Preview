# Hardware Connections & Pinout

## VGA Connector Pinout (DE-15)

**Monochrome (Green) Setup:**

| VGA Pin | Description | Arduino Connection |
| :--- | :--- | :--- |
| Pin 1 (Red) | Not used | Leave unconnected (or tie to GND) |
| Pin 2 (Green) | Video | D11 via **470–680 Ω** resistor |
| Pin 3 (Blue) | Not used | Leave unconnected (or tie to GND) |
| Pin 13 (HSync) | Horizontal Sync | D9 via **470–1 kΩ** resistor |
| Pin 14 (VSync) | Vertical Sync | D8 via **470–1 kΩ** resistor |
| Pins 5, 6, 7, 8, 10 (GND) | Ground | Arduino GND (short, thick wire) |

**Alternative Colors / White:**
*   To use **Red** or **Blue** instead of Green — connect the Arduino signal to Pin 1 or Pin 3 respectively.
*   To make **White** (or any custom color) — connect R, G and B pins together. Use separate 470–680 Ω resistors on each color pin to avoid crosstalk.

## Arduino Uno R4 Minima Pin Assignment

| Arduino Pin | RA4M1 Port | Function | Description |
| :--- | :--- | :--- | :--- |
| **D9** | P303 | HSync | Hardware GPT7 output |
| **D8** | P304 | VSync | Software GPIO |
| **D11** | P109 | Video | SPI1 MOSI (data) |
| **D2** | P105 | Button 1 | BEG + |
| **D3** | P104 | Button 2 | BEG − |
| **D4** | P103 | Button 3 | Reserved (unused in V47.0) |
| **D5** | P102 | Button 4 | Window shift left |
| **D6** | P106 | Button 5 | Window shift right |

## Electrical Characteristics & Resistor Values

**Board logic level:** The Arduino Uno R4 Minima exposes **5 V logic** on its digital pins, even though the RA4M1 silicon itself is a 3.3 V part. The board uses an on-board level shifter between the MCU and the headers. Digital pins can source/sink **up to 8 mA** per pin (per Arduino documentation).

### 1. Video Line (470–680 Ω)

*   **Why these values:** The VGA standard expects a 0.7 V signal into a 75 Ω load. With a 5 V source, the ideal series resistor is:

    `R = 75 × (5 / 0.7 − 1) ≈ 461 Ω`

    So **470 Ω** gives the correct 0.7 V level.

*   **Current check:** `I = 5 V / (470 + 75) ≈ 9.2 mA`. This is **above** the 8 mA pin limit on peak. However, the video line is only active during ~52% of the visible line time (pixels are ~50/50 black/white), so the average current stays well below 8 mA. **470 Ω is a good practical value and is used in the reference build.**

*   **Safer alternative:** If you want to stay strictly within the 8 mA limit, use **680 Ω**. The signal drops to about 0.5 V, which is still within the VGA spec (0–0.7 V per channel), just slightly dimmer. **680 Ω is recommended for continuous long-term use.**

### 2. Sync Lines (470–1 kΩ)

*   **Why these values:** Sync signals don't require a precise voltage; they just need to pull the line cleanly.
*   **Empirical data from this project:**
    *   **250 Ω — too weak**, signal unstable on some monitors, and current 20 mA (way over 8 mA limit).
    *   **470 Ω — works reliably** on tested monitors. Peak current 10.6 mA, but sync is only 4 µs out of every 31.7 µs (12.6% duty), so average current is ~1.3 mA.
    *   **750 Ω to 1 kΩ — works reliably and is safest**. Peak current 6.7–5 mA, average well below 1 mA.
*   **Recommendation:** **470 Ω** if you want maximum compatibility on old monitors; **680 Ω–1 kΩ** if you want to stay strictly within spec.

### 3. Ground

Short, thick wire from Arduino GND to VGA connector GND. At 24 MHz SPI, a poor ground causes visible noise ("sparkles").

---
---

# Подключение и распиновка (Русская версия)

## Распиновка разъёма VGA (DE-15)

**Монохромный режим (зелёный):**

| Пин VGA | Назначение | Подключение к Arduino |
| :--- | :--- | :--- |
| Пин 1 (Красный) | Не используется | Оставить свободным (или на GND) |
| Пин 2 (Зелёный) | Видео | D11 через **470–680 Ом** |
| Пин 3 (Синий) | Не используется | Оставить свободным (или на GND) |
| Пин 13 (HSync) | Строчная синхронизация | D9 через **470 Ом–1 кОм** |
| Пин 14 (VSync) | Кадровая синхронизация | D8 через **470 Ом–1 кОм** |
| Пины 5, 6, 7, 8, 10 (GND) | Земля | GND Arduino (короткий толстый провод) |

**Другие цвета / Белый:**
*   Чтобы использовать **Красный** или **Синий** вместо зелёного — подключите сигнал к Пину 1 или Пину 3 соответственно.
*   Чтобы получить **Белый** (или свой оттенок) — соедините пины R, G и B вместе. Используйте отдельные резисторы 470–680 Ом на каждый цвет, чтобы избежать перекрёстных помех.

## Распиновка Arduino Uno R4 Minima

| Пин Arduino | Порт RA4M1 | Функция | Описание |
| :--- | :--- | :--- | :--- |
| **D9** | P303 | HSync | Аппаратный выход GPT7 |
| **D8** | P304 | VSync | Программный GPIO |
| **D11** | P109 | Video | SPI1 MOSI (данные) |
| **D2** | P105 | Кнопка 1 | BEG + |
| **D3** | P104 | Кнопка 2 | BEG − |
| **D4** | P103 | Кнопка 3 | Зарезервирована (в V47.0 не используется) |
| **D5** | P102 | Кнопка 4 | Сдвиг окна влево |
| **D6** | P106 | Кнопка 5 | Сдвиг окна вправо |

## Электрические характеристики и номиналы резисторов

**Логические уровни платы:** Arduino Uno R4 Minima выдаёт на цифровых выводах **5 В**, хотя сам кристалл RA4M1 работает от 3.3 В. На плате стоит сдвиговый регистр уровней между MCU и гребёнкой. Максимальный ток на цифровой вывод по документации Arduino — **до 8 мА**.

### 1. Видеолиния (470–680 Ом)

*   **Почему такие номиналы:** Стандарт VGA ожидает сигнал 0.7 В на нагрузке 75 Ом. При источнике 5 В идеальный последовательный резистор:
    
    `R = 75 × (5 / 0.7 − 1) ≈ 461 Ом`
    
    То есть **470 Ом** даёт ровно 0.7 В.

*   **Проверка по току:** `I = 5 В / (470 + 75) ≈ 9.2 мА`. Это **выше** предела 8 мА по пику. Однако видеолиния активна только ~52% времени видимой строки (пиксели распределены примерно 50/50), поэтому средний ток остаётся ниже 8 мА. **470 Ом — рабочее значение, используется в референсной сборке.**

*   **Более безопасный вариант:** **680 Ом** — сигнал ~0.5 В, что укладывается в стандарт VGA (0–0.7 В), но чуть тусклее. **680 Ом рекомендуется для долговременного использования.**

### 2. Линии синхронизации (470 Ом–1 кОм)

*   **Почему такие номиналы:** синхронизация не требует точного напряжения, главное — чётко «просадить» линию.
*   **Практика проекта:**
    *   **250 Ом — слишком слабо**, нестабильно на некоторых мониторах, ток 20 мА (далеко за 8 мА).
    *   **470 Ом — работает надёжно** на проверенных мониторах. Пиковый ток 10.6 мА, но синхроимпульс занимает 4 мкс из 31.7 мкс (12.6% скважности), средний ток ~1.3 мА.
    *   **750 Ом–1 кОм — работает надёжно и безопасно по току**. Пиковый ток 6.7–5 мА, средний < 1 мА.
*   **Рекомендация:** **470 Ом** — для максимальной совместимости со старыми мониторами; **680 Ом–1 кОм** — если хочется остаться строго в пределах спецификации.

### 3. Земля

Короткий толстый провод от GND Arduino к GND VGA-разъёма. На частоте SPI 24 МГц плохая земля даёт видимый шум («искры»).

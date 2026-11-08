# Hardware Connections & Pinout

## VGA Connector Pinout (DE-15)
For a standard VGA cable, you only need a few pins. 

**Monochrome (Green) Setup:**
*   **Pin 1 (Red):** Leave unconnected or connect to GND.
*   **Pin 2 (Green):** Connect to Arduino D11 via a 470–520Ω resistor.
*   **Pin 3 (Blue):** Leave unconnected or connect to GND.
*   **Pin 13 (HSync):** Connect to Arduino D9 via a 470–750Ω resistor.
*   **Pin 14 (VSync):** Connect to Arduino D8 via a 470–750Ω resistor.
*   **Pins 5, 6, 7, 8, 10 (GND):** Connect to Arduino GND.

**Alternative Colors & White Setup:**
*   To use a **Red** or **Blue** video channel instead of Green, simply connect the Arduino signal to Pin 1 or Pin 3 respectively (using the same 470-520Ω resistor).
*   To create a **custom color** (or White), connect the R, G, and B pins together. You can use a single resistor to the Arduino pin, or better, use separate 470-520Ω resistors for each pin to prevent cross-talk and create custom mixed colors via your own voltage divider network.

## Arduino Uno R4 Minima Pinout

| Arduino Pin | RA4M1 Port | Function | Description |
| :--- | :--- | :--- | :--- |
| **D9** | P303 | HSync | Hardware timer output (GPT7). |
| **D8** | P304 | VSync | Software GPIO. |
| **D11** | P109 | Video | SPI1 MOSI (Data output). |
| **D2** | P105 | Button 1 | Adjust line phase (BEG +). |
| **D3** | P104 | Button 2 | Adjust line phase (BEG -). |
| **D5** | P102 | Button 3 | Shift window left. |
| **D6** | P106 | Button 4 | Shift window right. |

## Schematic Notes & Resistor Values

*Important note: Although the RA4M1 chip operates at 3.3V internally, the Uno R4 Minima uses a level shifter on its digital I/O pins, meaning they output **5V**.*

Based on empirical testing and electrical calculations, the following resistor values are recommended:

### 1. Video Line (470–520 Ω)
*   **Why:** The VGA standard expects a 0.7V peak-to-peak signal into a 75Ω load. Using a voltage divider formula with a 5V source (\(0.7V = 5V \times \frac{75Ω}{75Ω + R}\)), the ideal series resistor is approximately **460Ω**.
*   **Practical:** 470Ω gives a perfectly bright image and is the theoretical ideal. 520Ω gives a slightly dimmer but completely safe signal.
*   *Warning:* Do not use resistors lower than 400Ω for video, as you risk overvoltage on the VGA monitor.

### 2. Sync Lines (470–750 Ω)
*   **Why:** The sync signals (HSync and VSync) don't require precise voltage matching but must pull the line cleanly.
*   **Empirical data:** Testing showed that **250Ω is too weak** (causing signal instability) and places a **very high load on the GPIO pin** (drawing 20mA, which is the absolute maximum limit). 
*   **Recommendation:** **470Ω to 750Ω** (or even up to 1 kΩ depending on your monitor) provides a crisp, stable sync pulse while keeping the current draw safely between 6mA and 10mA.

### 3. Ground (GND)
*   **Recommendation:** Use the shortest and thickest wire possible between the Arduino GND and the VGA connector GND. At 24 MHz SPI speeds, a poor ground connection will cause noise and "sparkles" on the screen.

---
---

# Подключение и распиновка (Русская версия)

## Распиновка разъема VGA (DE-15)
Для стандартного VGA-кабеля вам нужно всего несколько пинов.

**Монохромный режим (Зеленый):**
*   **Пин 1 (Красный):** Оставить неподключенным или посадить на GND.
*   **Пин 2 (Зеленый):** Подключить к пину D11 Arduino через резистор **470–520 Ом**.
*   **Пин 3 (Синий):** Оставить неподключенным или посадить на GND.
*   **Пин 13 (HSync):** Подключить к пину D9 Arduino через резистор **470–750 Ом**.
*   **Пин 14 (VSync):** Подключить к пину D8 Arduino через резистор **470–750 Ом**.
*   **Пины 5, 6, 7, 8, 10 (GND):** Подключить к GND Arduino.

**Альтернативные цвета и Белый:**
*   Если вы хотите использовать **Красный** или **Синий** канал вместо Зеленого, просто подключите сигнал с Arduino к Пину 1 или Пину 3 соответственно (через такой же резистор 470-520 Ом).
*   Если вы хотите получить **свой цвет** (или Белый), соедините пины R, G и B вместе. Вы можете использовать один резистор к выводу Arduino, либо (что правильнее) использовать отдельные резисторы 470-520 Ом для каждого пина, чтобы избежать перекрестных помех и собрать свой собственный делитель напряжения для нужного оттенка.

## Распиновка Arduino Uno R4 Minima

| Пин Arduino | Порт RA4M1 | Функция | Описание |
| :--- | :--- | :--- | :--- |
| **D9** | P303 | HSync | Аппаратный выход таймера (GPT7). |
| **D8** | P304 | VSync | Программный вывод (GPIO). |
| **D11** | P109 | Video | SPI1 MOSI (Выход данных). |
| **D2** | P105 | Кнопка 1 | Регулировка фазы строки (BEG +). |
| **D3** | P104 | Кнопка 2 | Регулировка фазы строки (BEG -). |
| **D5** | P102 | Кнопка 3 | Сдвиг окна влево. |
| **D6** | P106 | Кнопка 4 | Сдвиг окна вправо. |

## Схемотехника и номиналы резисторов

*Важное примечание: Хотя чип RA4M1 работает от 3.3В, на Uno R4 Minima стоят сдвиговые регистры, поэтому цифровые выводы выдают **5 Вольт**.*

На основе практических тестов и расчетов рекомендуются следующие номиналы:

### 1. Видеолиния (470–520 Ом)
*   **Почему:** Стандарт VGA ожидает сигнал с размахом 0.7 В на нагрузке 75 Ом. По формуле делителя напряжения (\(0.7В = 5В \times \frac{75}{75 + R}\)), идеальный резистор — около **460 Ом**.
*   **Практика:** 470 Ом дают идеальную яркость. 520 Ом дают чуть более тусклый, но абсолютно безопасный сигнал.
*   *Внимание:* Не используйте резисторы меньше 400 Ом для видео, это может превысить допустимое напряжение на входе монитора.

### 2. Линии синхронизации (470–750 Ом)
*   **Почему:** Сигналы синхронизации не требуют точного согласования по напряжению, но должны четко «просаживать» линию.
*   **Практика:** Тесты показали, что резистор на **250 Ом — слишком слабый** (вызывает нестабильность) и дает **очень высокую нагрузку на вывод** (ток 20 мА, это абсолютный предел). 
*   **Рекомендация:** **470–750 Ом** (или даже до 1 кОм в зависимости от монитора) обеспечивают чистый синхроимпульс при безопасном токе 6-10 мА.

### 3. Земля (GND)
*   **Рекомендация:** Используйте максимально короткий и толстый провод между GND Arduino и GND разъема VGA. На частоте SPI 24 МГц плохой контакт земли вызовет шум и «искры» на экране.

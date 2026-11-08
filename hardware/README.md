# Hardware Connections & Pinout

## VGA Connector Pinout (DE-15)
For a standard VGA cable, you only need 5 pins for monochrome output:
*   **Pin 1 (Red):** Not connected (or tie to Green via resistor for future color support)
*   **Pin 2 (Green):** Connect to Arduino D11 via a 470–520Ω resistor.
*   **Pin 3 (Blue):** Not connected (or tie to Green via resistor for future color support)
*   **Pin 13 (HSync):** Connect to Arduino D9 via a 470–750Ω resistor.
*   **Pin 14 (VSync):** Connect to Arduino D8 via a 470–750Ω resistor.
*   **Pins 5, 6, 7, 8, 10 (GND):** Connect to Arduino GND.

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

Based on empirical testing with the Uno R4 Minima, the following resistor values are recommended:

### 1. Video Line (Green, and future R/B channels)
*   **Value:** **470–520 Ω**
*   **Why:** The VGA standard expects a 0.7V peak-to-peak signal (75Ω impedance). The Arduino outputs 3.3V. A 470Ω resistor acts as a voltage divider to bring the level down to the correct range and protects the monitor. 
*   *Note:* 470Ω is the ideal value but brings the current draw close to the pin's maximum limit. If you want a safer margin, use **520Ω** (the brightness will be slightly lower, but it's perfectly safe).

### 2. Sync Lines (HSync D9, VSync D8)
*   **Value:** **470–750 Ω** (Values up to **1 kΩ** may also work depending on the monitor)
*   **Why:** The sync signals don't need precise voltage matching, but they do need to pull the line cleanly. A 250Ω resistor was tested and found to be too weak (causing signal instability). A 470Ω to 750Ω resistor provides a clean, stable sync pulse without overloading the GPIO pins. 

### 3. Ground (GND)
*   **Recommendation:** Use the shortest and thickest wire possible between the Arduino GND and the VGA connector GND. At 24 MHz SPI speeds, a poor ground connection will cause noise and "sparkles" on the screen.

---
---

# Подключение и распиновка (Русская версия)

## Распиновка разъема VGA (DE-15)
Для стандартного VGA-кабеля при монохромном выводе вам нужно только 5 пинов:
*   **Пин 1 (Красный):** Не подключен (или соединить с зеленым через резистор для будущей поддержки цветов).
*   **Пин 2 (Зеленый):** Подключить к пину D11 Arduino через резистор **470–520 Ом**.
*   **Пин 3 (Синий):** Не подключен (или соединить с зеленым через резистор для будущей поддержки цветов).
*   **Пин 13 (HSync):** Подключить к пину D9 Arduino через резистор **470–750 Ом**.
*   **Пин 14 (VSync):** Подключить к пину D8 Arduino через резистор **470–750 Ом**.
*   **Пины 5, 6, 7, 8, 10 (GND):** Подключить к GND Arduino.

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

На основе практических тестов с Uno R4 Minima рекомендуются следующие номиналы:

### 1. Видеолиния (Зеленый, а в будущем R/B каналы)
*   **Номинал:** **470–520 Ом**
*   **Почему:** Стандарт VGA ожидает сигнал с размахом 0.7 В (импеданс 75 Ом). Arduino выдает 3.3 В. Резистор на 470 Ом работает как делитель напряжения, понижая уровень до нужного и защищая монитор. 
*   *Примечание:* 470 Ом — идеальный номинал, но ток через вывод приближается к максимально допустимому. Для большей безопасности используйте **520 Ом** (яркость будет чуть ниже, но это абсолютно безопасно для платы).

### 2. Линии синхронизации (HSync D9, VSync D8)
*   **Номинал:** **470–750 Ом** (значения вплоть до **1 кОм** также могут работать в зависимости от монитора).
*   **Почему:** Сигналы синхронизации не требуют точного согласования по напряжению, но они должны четко «просаживать» линию. В тестах резистор на 250 Ом оказался слишком слабым (вызывал нестабильность). Резистор на 470–750 Ом обеспечивает чистый стабильный синхроимпульс без перегрузки пинов.

### 3. Земля (GND)
*   **Рекомендация:** Используйте максимально короткий и толстый провод между GND Arduino и GND разъема VGA. На частоте SPI 24 МГц плохой контакт земли вызовет шум и «искры» на экране.

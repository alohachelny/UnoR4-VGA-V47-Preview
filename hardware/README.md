# Hardware Connections & Pinout

## VGA Connector Pinout (DE-15)

**Monochrome Video Setup:**

| VGA Pin | Description | Arduino Connection |
| :--- | :--- | :--- |
| Pin 1 (Red) | Red video | Leave unconnected if not used |
| Pin 2 (Green) | Green video | D11 via 510–680 Ω resistor |
| Pin 3 (Blue) | Blue video | Leave unconnected if not used |
| Pin 13 (HSync) | Horizontal Sync | D9 via 470 Ω–1 kΩ resistor |
| Pin 14 (VSync) | Vertical Sync | D8 via 470 Ω–1 kΩ resistor |
| Pins 5, 6, 7, 8, 10 (GND) | Ground | Arduino GND |

**Using Different Colors:**

- For monochrome video, the signal can be connected to any one of the three RGB inputs: Red, Green or Blue.
- Color mixing is possible by driving multiple RGB channels, provided the total electrical load remains within the output capabilities of the driving pins.
- Use a separate series resistor for each RGB channel.
- Do not connect the RGB signal inputs directly together without individual resistors.

The current V47.0 firmware provides a single monochrome video output through D11.

## Arduino Uno R4 Minima Pin Assignment

| Arduino Pin | RA4M1 Port | Function | Description |
| :--- | :--- | :--- | :--- |
| **D9** | P303 | HSync | Hardware GPT7 output |
| **D8** | P304 | VSync | Software GPIO |
| **D11** | P109 | Video | SPI1 MOSI (data) |
| **D2** | P105 | Button 1 | BEG + |
| **D3** | P104 | Button 2 | BEG − |
| **D4** | P103 | Optional button | Control inhibit |
| **D5** | P102 | Button 4 | Window shift left |
| **D6** | P106 | Button 5 | Window shift right |

D4 is optional. When pressed, commands from the other buttons are inhibited. If it is not used, it can remain unconnected because the firmware enables the internal pull-up resistor.

## Electrical Characteristics & Resistor Values

**Board operating voltage:** Arduino UNO R4 Minima operates at 5 V. All resistor values and calculations below refer to the tested configuration with a nominal 5 V logic HIGH level.

If the actual output HIGH voltage differs from 5 V, resistor values must be recalculated for the new operating conditions.

### 1. RGB Video Line (510–680 Ω)

The VGA RGB input is treated as a 75 Ω load.

The approximate voltage at the monitor input can be calculated as:

`V_RGB ≈ V_HIGH × 75 / (R + 75)`

For a nominal 5 V output:

| Series Resistor | Approximate RGB Voltage |
| :---: | :---: |
| 510 Ω | 0.64 V |
| 680 Ω | 0.50 V |

**Actual tested configuration: 510 Ω.**

- **510 Ω** is the resistor installed in the reference build.
- **680 Ω** is an alternative value that produces a lower video signal level.
- 510 Ω is the minimum recommended value for this 5 V configuration.
- 470 Ω is not used in this release.

The calculated voltage is an estimate. Actual signal levels depend on the GPIO output characteristics, wiring and monitor input.

### 2. Sync Lines (470 Ω–1 kΩ)

HSync and VSync are digital synchronization signals.

The following resistor values have been evaluated in the project:

- **250 Ω:** unreliable operation on some tested monitors.
- **470 Ω:** works reliably on tested monitors.
- **750 Ω–1 kΩ:** also works reliably on tested monitors.

**Recommendation:** use 470 Ω–1 kΩ series resistors for HSync and VSync. The final choice can depend on monitor compatibility and the actual signal levels.

### 3. Ground

Use a short, secure ground connection between Arduino GND and VGA connector GND.

At 24 MHz SPI, poor grounding can cause visible interference and noise in the video signal.

---

# Подключение и распиновка (Русская версия)

## Распиновка разъёма VGA (DE-15)

**Монохромный видеорежим:**

| Пин VGA | Назначение | Подключение к Arduino |
| :--- | :--- | :--- |
| Пин 1 (Красный) | Красный видеосигнал | Оставить свободным, если не используется |
| Пин 2 (Зелёный) | Зелёный видеосигнал | D11 через резистор 510–680 Ом |
| Пин 3 (Синий) | Синий видеосигнал | Оставить свободным, если не используется |
| Пин 13 (HSync) | Строчная синхронизация | D9 через резистор 470 Ом–1 кОм |
| Пин 14 (VSync) | Кадровая синхронизация | D8 через резистор 470 Ом–1 кОм |
| Пины 5, 6, 7, 8, 10 (GND) | Земля | GND Arduino |

**Использование других цветов:**

- Для монохромного изображения видеосигнал можно подать на любой из трёх входов RGB: Red, Green или Blue.
- Смешивание цветов возможно при одновременном управлении несколькими каналами, если суммарная электрическая нагрузка не превышает возможностей используемых выходов.
- Для каждого RGB-канала необходимо предусмотреть отдельный последовательный резистор.
- Не следует соединять входы RGB непосредственно друг с другом без индивидуальных резисторов.

Текущая прошивка V47.0 формирует один монохромный видеосигнал через D11.

## Распиновка Arduino Uno R4 Minima

| Пин Arduino | Порт RA4M1 | Функция | Описание |
| :--- | :--- | :--- | :--- |
| **D9** | P303 | HSync | Аппаратный выход GPT7 |
| **D8** | P304 | VSync | Программный GPIO |
| **D11** | P109 | Video | SPI1 MOSI (данные) |
| **D2** | P105 | Кнопка 1 | BEG + |
| **D3** | P104 | Кнопка 2 | BEG − |
| **D4** | P103 | Необязательная кнопка | Блокировка управления |
| **D5** | P102 | Кнопка 4 | Сдвиг окна влево |
| **D6** | P106 | Кнопка 5 | Сдвиг окна вправо |

Кнопка D4 необязательна. При её нажатии команды остальных кнопок блокируются. Если она не используется, вывод можно оставить свободным: прошивка включает внутренний подтягивающий резистор.

## Электрические характеристики и номиналы резисторов

**Рабочее напряжение платы:** Arduino UNO R4 Minima работает при напряжении 5 В. Все приведённые ниже номиналы резисторов и расчёты относятся к испытанной конфигурации с номинальным логическим уровнем HIGH около 5 В.

Если фактическое напряжение логической единицы отличается от 5 В, номиналы резисторов необходимо пересчитать для новых условий работы.

### 1. Видеолиния RGB (510–680 Ом)

Вход RGB монитора рассматривается как нагрузка 75 Ом.

Приближённое напряжение на входе монитора можно рассчитать по формуле:

`V_RGB ≈ V_HIGH × 75 / (R + 75)`

При номинальном напряжении 5 В:

| Последовательный резистор | Расчётное напряжение RGB |
| :---: | :---: |
| 510 Ом | 0,64 В |
| 680 Ом | 0,50 В |

**Фактически испытанная конфигурация: 510 Ом.**

- **510 Ом** — номинал установленного в референсной сборке резистора.
- **680 Ом** — альтернативный номинал, обеспечивающий более низкий уровень видеосигнала.
- 510 Ом — минимальный рекомендуемый номинал для данной конфигурации с питанием 5 В.
- Резистор 470 Ом в данной версии не используется.

Расчётные значения являются приближёнными. Фактический уровень сигнала зависит от характеристик выхода GPIO, соединительных проводов и входа монитора.

### 2. Линии синхронизации (470 Ом–1 кОм)

HSync и VSync являются цифровыми сигналами синхронизации.

В проекте проверены следующие номиналы резисторов:

- **250 Ом:** нестабильная работа с некоторыми испытанными мониторами.
- **470 Ом:** надёжная работа на проверенных мониторах.
- **750 Ом–1 кОм:** также надёжная работа на проверенных мониторах.

**Рекомендация:** использовать последовательные резисторы 470 Ом–1 кОм для HSync и VSync. Окончательный выбор зависит от совместимости с монитором и фактических уровней сигналов.

### 3. Земля

Используйте короткое и надёжное соединение GND Arduino с землёй VGA-разъёма.

На частоте SPI 24 МГц плохое заземление может вызывать видимые помехи и шум на изображении.

---

## Official Documentation

- [Arduino UNO R4 Minima — Official Documentation](https://docs.arduino.cc/hardware/uno-r4-minima)
- [Arduino UNO R4 Minima — Schematic ABX00080](https://docs.arduino.cc/resources/schematics/ABX00080-schematics.pdf)
- [Renesas RA4M1 Group — Datasheet](https://www.renesas.com/en/document/dst/ra4m1-group-datasheet)

# UnoR4-VGA-V47-Preview

> **DRAFT / PREVIEW:** This is a working intermediate proof-of-concept, not a final library. The final project will have colors, double buffering, and a different API.

Bare-metal monochrome VGA engine for **Arduino Uno R4 Minima (Renesas RA4M1)**. 
Zero-copy DMA, hardware line start (GPT7), SPI1 @ 24 MHz, and a built-in bouncing square demo.

## 📺 Demo Video

<video src="media/video/vga_demo.mp4" controls width="640"></video>

*If the video does not play inline, open it directly: [media/video/vga_demo.mp4](media/video/vga_demo.mp4).*

## 📷 Photos & Oscilloscope Traces

| VGA Display | HSync (HSYNC pin) | VSync (VSYNC pin) |
| :---: | :---: | :---: |
| ![VGA Display](media/display/vga_display.jpg) | ![HSync](media/oscilloscope/oscilloscope_02.jpg) | ![VSync](media/oscilloscope/oscilloscope_01.jpg) |

## 📐 About the "320×240" Resolution

The engine generates a **standard 640×480 @ 60 Hz VGA timing** (525 total lines per frame, 480 active lines, 31.5 kHz HSync), but only **320×240 pixels of actual content** are streamed. Two things happen:

1. **Horizontally:** only 320 pixels of data are sent, followed by padding. The monitor typically stretches this to the full width of the screen.
2. **Vertically:** only 240 of the 480 active lines contain picture data (the VRAM window). The remaining 240 lines are blanking.

So, **"320×240" refers to the content size and VRAM buffer size, not the full VGA frame format.** The monitor receives a real 640×480 signal.

## ✨ Features (V47.0)
*   **True Bare-Metal:** Direct register access for SPI1, DMAC, and GPT7 (bypassing FSP limitations).
*   **Zero-Copy Scan-out:** DMA reads directly from the VRAM buffer without CPU intervention.
*   **Hardware Line Start:** The first word of each line is triggered by a GPT7 compare-match event via DMAC1, eliminating software jitter (the "jagged pixel" problem).
*   **Fast Graphics:** Byte-mask horizontal span filling (`vram_hspan`) for fast rectangle draws.
*   **Built-in Demo:** A 16×16 square bouncing off a 320×240 frame, updated during V-Blank to prevent tearing.

## 🔌 Hardware Connections

See detailed schematic notes in [`hardware/README.md`](hardware/README.md).

| VGA Pin | Arduino Uno R4 Minima Pin | Note |
| :--- | :--- | :--- |
| **HSync** | **D9** | Hardware GPT7 output |
| **VSync** | **D8** | Software GPIO |
| **Video (Green)** | **D11** | SPI1 MOSI (24 MHz) |
| **GND** | **GND** | Common ground |
*Use a 470–520 Ω resistor on the video line and 470–750 Ω on the sync lines. See `hardware/README.md` for the electrical reasoning.*

## 🚀 How to Flash
1. Open `firmware/v47_demo/v47_demo.ino` in the Arduino IDE.
2. Select **Arduino Uno R4 Minima** as the board.
3. Compile and upload.
4. **Controls:**

| Button | Function |
| :--- | :--- |
| **D2** | Increase line start phase (BEG +) |
| **D3** | Decrease line start phase (BEG −) |
| **D4** | **Reserved** in this preview (no action yet). Reserved for future use. |
| **D5** | Shift window left by 1 word (32 px) |
| **D6** | Shift window right by 1 word (32 px) |

> ⚠️ **Note for future versions:** D6 is currently used for horizontal window shift. If the final project adds color VGA, D6 may need to be reassigned to a different function.

## ⚠️ Limitations & Roadmap
*   **Current Limitations:**
    *   Monochrome only.
    *   Single VRAM buffer — no full-frame double buffering yet (only the bouncing square is updated during V-Blank).
    *   No keyboard/PS2 input (buttons only).
*   **Roadmap (future project):**
    *   Color support (bit-banging, external shift registers, or SPI multi-channel).
    *   Full double buffering for the entire frame.
    *   Text mode (e.g. 64×35 grid).
    *   PS/2 keyboard support.
    *   Packaging into a proper Arduino Library.

## 📄 License
This project is licensed under a **Custom Non-Commercial License with Attribution**.
You may use, modify, and share it for personal or educational purposes, provided you credit the author (**alohachelny**) and link back to this repository. Commercial use is prohibited without prior written permission.

---
---

# 🇷🇺 Русская версия

> **ЧЕРНОВИК / ПРЕВЬЮ:** Это промежуточный рабочий прототип, а не финальная библиотека. В итоговом проекте будут цвета, двойная буферизация и другой API.

Голый (bare-metal) движок монохромного VGA для **Arduino Uno R4 Minima (Renesas RA4M1)**. 
Zero-copy DMA, аппаратный старт строки (GPT7), SPI1 на 24 МГц, встроенная демо-игра с прыгающим квадратом.

## 📺 Демо-видео

<video src="media/video/vga_demo.mp4" controls width="640"></video>

*Если видео не воспроизводится прямо в окне, откройте файл напрямую: [media/video/vga_demo.mp4](media/video/vga_demo.mp4).*

## 📷 Фото и осциллограммы

| Изображение | HSync | VSync |
| :---: | :---: | :---: |
| ![VGA Display](media/display/vga_display.jpg) | ![HSync](media/oscilloscope/oscilloscope_02.jpg) | ![VSync](media/oscilloscope/oscilloscope_01.jpg) |

## 📐 Про разрешение «320×240»

Движок формирует **стандартный сигнал VGA 640×480 @ 60 Гц** (525 строк в кадре, 480 активных строк, HSync 31.5 кГц), но по факту передаёт только **320×240 пикселей данных**:

1. **По горизонтали:** передаётся 320 пикселей данных, остальное — паддинг. Монитор обычно растягивает картинку на всю ширину экрана.
2. **По вертикали:** только 240 из 480 активных строк содержат данные (окно VRAM). Остальные 240 строк — гашение.

Таким образом, **«320×240» — это размер контента и видеобуфера, а не полный формат кадра VGA**. Монитор получает полноценный сигнал 640×480.

## ✨ Особенности (V47.0)
*   **Настоящий bare-metal:** прямой доступ к регистрам SPI1, DMAC, GPT7 (в обход FSP).
*   **Zero-Copy вывод:** DMA читает данные напрямую из VRAM без участия CPU.
*   **Аппаратный старт строки:** первое слово каждой строки запускается по событию GPT7 через DMAC1 — устранён программный джиттер.
*   **Быстрая графика:** заливка прямоугольников через байтовые маски (`vram_hspan`).
*   **Встроенная демо-игра:** квадрат 16×16, отскакивающий от рамки 320×240. Обновление в зоне V-Blank для предотвращения тиринга.

## 🔌 Подключение

Подробности — в [`hardware/README.md`](hardware/README.md).

| Пин VGA | Пин Arduino | Примечание |
| :--- | :--- | :--- |
| **HSync** | **D9** | Аппаратный выход GPT7 |
| **VSync** | **D8** | Программный GPIO |
| **Video** | **D11** | SPI1 MOSI (24 МГц) |
| **GND** | **GND** | Общая земля |

*На видеолинию — резистор 470–520 Ом, на линии синхронизации — 470–750 Ом. Расчёт и обоснование — в `hardware/README.md`.*

## 🚀 Прошивка
1. Открой `firmware/v47_demo/v47_demo.ino` в Arduino IDE.
2. Выбери плату **Arduino Uno R4 Minima**.
3. Скомпилируй и загрузи.
4. **Управление:**

| Кнопка | Функция |
| :--- | :--- |
| **D2** | Увеличить фазу старта строки (BEG +) |
| **D3** | Уменьшить фазу старта строки (BEG −) |
| **D4** | **Зарезервировано** в этой версии. Задел на будущее. |
| **D5** | Сдвиг окна влево на слово (32 пикс.) |
| **D6** | Сдвиг окна вправо на слово (32 пикс.) |

> ⚠️ **На будущее:** D6 сейчас занят сдвигом окна. Если появится цветной VGA, назначение D6 придётся пересмотреть.

## ⚠️ Ограничения и планы
*   **Сейчас:** только монохром, один VRAM-буфер (нет полной двойной буферизации кадра), управление кнопками.
*   **Планы:** цвета, полная двойная буферизация, текстовый режим 64×35, PS/2 клавиатура, оформление в библиотеку.

## 📄 Лицензия
Кастомная лицензия (**Non-Commercial** с обязательным указанием авторства).
Личное и учебное использование, изменение и распространение — разрешены при указании автора **alohachelny** и ссылки на репозиторий. Коммерческое использование — только с письменного разрешения.

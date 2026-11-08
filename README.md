# UnoR4-VGA-V47-Preview

> **DRAFT / PREVIEW:** This is a working intermediate proof-of-concept, not a final library. The final project will have colors, double buffering, and a different API.

Bare-metal monochrome VGA 320x240 engine for **Arduino Uno R4 Minima (Renesas RA4M1)**. 
Achieved via Zero-copy DMA, hardware line start (GPT7), and SPI1 @ 24 MHz. Includes a built-in bouncing square demo.

## 📺 Demo
*(Placeholder: Add a GIF or short video of the bouncing square here later)*
![Demo Placeholder](https://via.placeholder.com/640x480.png?text=Video+GIF+Coming+Soon)

## ✨ Features (V47.0)
*   **True Bare-Metal:** Direct register access for SPI1, DMAC, and GPT7 (bypassing FSP limitations).
*   **Zero-Copy Scan-out:** DMA reads directly from the VRAM buffer without CPU intervention.
*   **Hardware Line Start:** The first word of each line is triggered by a GPT7 compare match via DMAC1, eliminating software jitter (the "jagged pixel" problem).
*   **Fast Graphics:** Byte-mask horizontal span filling (`vram_hspan`) for fast rectangle draws.
*   **Built-in Game Piece:** A 16x16 square bouncing off a 320x240 frame, updated during V-Blank to prevent tearing.

## 🔌 Hardware Connections
| VGA Pin | Arduino Uno R4 Minima Pin | Note |
| :--- | :--- | :--- |
| **HSync** | **D9** | Hardware GPT7 output |
| **VSync** | **D8** | Software GPIO |
| **Video (Green)** | **D11** | SPI1 MOSI (24 MHz) |
| **GND** | **GND** | Common ground |
*(Note: A 470 Ohm resistor on the Video line is recommended for VGA protection).*

## 🚀 How to Flash
1. Open `firmware/v47_demo/v47_demo.ino` in the Arduino IDE.
2. Select **Arduino Uno R4 Minima** as the board.
3. Compile and upload.
4. **Controls:**
   *   `D2` / `D3` — Adjust the start phase (BEG) of the line.
   *   `D5` / `D6` — Shift the active 320px window left/right (in 32-pixel steps).

## ⚠️ Limitations & Roadmap
*   **Current Limitation:** Monochrome only, single VRAM buffer (no hardware double buffering for the whole frame yet).
*   **Roadmap (Future Project):**
    *   Color support (via bit-banging or external shift registers).
    *   Double buffering for VRAM.
    *   Text mode (64x35 grid).
    *   PS/2 keyboard support.
    *   Packaging into a proper Arduino Library.

## 📄 License
This project is licensed under a **Custom Non-Commercial License with Attribution**. 
You are free to use, modify, and share it for personal or educational purposes, provided you credit the author (**alohachelny**) and link back to this repository. Commercial use is strictly prohibited without prior written permission.

---
---

# 🇷🇺 Русская версия

> **ЧЕРНОВИК / ПРЕВЬЮ:** Это промежуточный рабочий прототип, а не финальная библиотека. В итоговом проекте будут цвета, двойная буферизация и другой API.

Голый (bare-metal) движок монохромного VGA 320x240 для **Arduino Uno R4 Minima (Renesas RA4M1)**. 
Реализован через Zero-copy DMA, аппаратный старт строки (GPT7) и SPI1 на частоте 24 МГц. Включает встроенную демо-игру с прыгающим квадратом.

## ✨ Особенности (V47.0)
*   **Настоящий bare-metal:** Прямой доступ к регистрам SPI1, DMAC и GPT7 (в обход ограничений FSP).
*   **Zero-Copy вывод:** DMA читает данные напрямую из видеопамяти (VRAM) без участия процессора.
*   **Аппаратный старт строки:** Первое слово каждой строки запускается по событию сравнения GPT7 через DMAC1. Это полностью устранило программный джиттер (проблему «зубчатых» пикселей).
*   **Быстрая графика:** Заполнение прямоугольников через байтовые маски (`vram_hspan`).
*   **Встроенный кусок игры:** Квадрат 16x16 пикселей отскакивает от рамки 320x240. Обновление происходит в зоне кадрового гашения (V-Blank), чтобы избежать разрывов изображения (тиринга).

## 🔌 Подключение к VGA
| Пин VGA | Пин Arduino Uno R4 Minima | Примечание |
| :--- | :--- | :--- |
| **HSync** | **D9** | Аппаратный выход GPT7 |
| **VSync** | **D8** | Программный GPIO |
| **Video (Зеленый)** | **D11** | SPI1 MOSI (24 МГц) |
| **GND** | **GND** | Общая земля |
*(Рекомендуется поставить резистор на 470 - 520 Ом на видеолинию для защиты VGA).*

## 🚀 Как прошить
1. Откройте файл `firmware/v47_demo/v47_demo.ino` в Arduino IDE.
2. Выберите плату **Arduino Uno R4 Minima**.
3. Скомпилируйте и загрузите.
4. **Управление:**
   *   `D2` / `D3` — Регулировка фазы старта строки (BEG).
   *   `D5` / `D6` — Сдвиг активного окна 320px влево/вправо (с шагом 32 пикселя).

## ⚠️ Ограничения и Планы
*   **Текущее ограничение:** Только монохромный режим, один буфер VRAM (пока нет полной двойной буферизации кадра).
*   **Планы (Будущий проект):**
    *   Поддержка цветов.
    *   Двойная буферизация VRAM.
    *   Текстовый режим (сетка 64x35).
    *   Поддержка PS/2 клавиатуры.
    *   Оформление в полноценную Arduino библиотеку.

## 📄 Лицензия
Проект распространяется по **Кастомной лицензии (Non-Commercial с указанием авторства)**. 
Вы можете свободно использовать, изменять и распространять код в личных или образовательных целях при условии указания автора (**alohachelny**) и ссылки на этот репозиторий. Коммерческое использование строго запрещено без письменного разрешения автора.

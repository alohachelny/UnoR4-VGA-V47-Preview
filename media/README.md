# Media Files

This folder contains screenshots, oscilloscope traces, and a short demo video of the V47.0 engine running on Arduino Uno R4 Minima.

## 📁 Contents

### `display/`
Actual photos of a VGA monitor showing the V47.0 output.

| File | Description |
| :--- | :--- |
| `vga_display.jpg` | Full-screen capture: frame, text ("VGA 320X240 / FRAME + HW START / V47.0"), and the bouncing square. |

### `oscilloscope/`
Oscilloscope traces of the VGA timing signals, captured on the corresponding Arduino pins.

| File | Signal | Expected |
| :--- | :--- | :--- |
| `oscilloscope_01.jpg` | HSync on D9 | ~4 µs low pulse, period ~31.77 µs (~31.5 kHz) |
| `oscilloscope_02.jpg` | VSync on D8 | ~64 µs low pulse, period ~16.67 ms (60 Hz) |

### `video/`
Short demo video of the engine running.

| File | Description |
| :--- | :--- |
| `vga_demo.mp4` | Bouncing square inside a 320×240 frame on a real VGA monitor. Shows smooth animation without tearing (V-Blank updates). |

## 📌 How to View

*   **Photos** (`.jpg`) — open directly in a browser, or view them inline in the main [`README.md`](../README.md).
*   **Video** (`.mp4`) — the main `README.md` embeds it with a `<video>` tag. If your browser does not play it inline, download the file and open it in VLC / any video player.

---
---

# Медиа-файлы (Русская версия)

В этой папке — скриншоты, осциллограммы и короткое демо-видео движка V47.0 на Arduino Uno R4 Minima.

## 📁 Содержимое

### `display/`
Реальные фотографии VGA-монитора с выводом V47.0.

| Файл | Описание |
| :--- | :--- |
| `vga_display.jpg` | Снимок экрана: рамка, текст («VGA 320X240 / FRAME + HW START / V47.0») и движущийся квадрат. |

### `oscilloscope/`
Осциллограммы сигналов VGA, снятые с соответствующих пинов Arduino.

| Файл | Сигнал | Ожидание |
| :--- | :--- | :--- |
| `oscilloscope_01.jpg` | HSync на D9 | Низкий импульс ~4 мкс, период ~31.77 мкс (31.5 кГц) |
| `oscilloscope_02.jpg` | VSync на D8 | Низкий импульс ~64 мкс, период ~16.67 мс (60 Гц) |

### `video/`
Короткое демо-видео работы движка.

| Файл | Описание |
| :--- | :--- |
| `vga_demo.mp4` | Квадрат, отскакивающий от рамки 320×240 на реальном VGA-мониторе. Плавная анимация без разрывов (обновление в V-Blank). |

## 📌 Как посмотреть

*   **Фото** (`.jpg`) — открываются прямо в браузере, встроены в главный [`README.md`](../README.md).
*   **Видео** (`.mp4`) — встроено в главный `README.md` через тег `<video>`. Если браузер не воспроизводит — скачайте файл и откройте в VLC или любом видеоплеере.

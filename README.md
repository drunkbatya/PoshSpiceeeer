# PoshSpiceeeer

## About
This it a gift to my ex-girlfriend for her 21st bithday. The idea is to visualize the core moments of her life. This device based on STM32F401 MCU, uses ST7920-based 128x64 LCD and my own driver library, perifery driven by STM32 LL library. Most of the code (like SceneManager) are taken from and/or inspired by [Flipper Zero Official Firmware](https://github.com/flipperdevices/flipperzero-firmware).

## Board
[Main schematics PDF](/github-assets/main_schematic.pdf)

EasyEDA 3D render:

<img src="/github-assets/board_easy_eda_render.png" width=60% />

Board IRL:

<img src="/github-assets/board_irl.jpg" width=60% />


## Scenes

For uploading to Github images was converted to GIFs by [Animated GIF Maker](https://ezgif.com/maker) and upscaled by [Pixel Art Scaler](https://lospec.com/pixel-art-scaler/). Most of scenes contain an animated text ([example](/src/scenes/app_scene_stars.c#L26)) which is  not present is GIFs below because the text output is done programmatically by firmware and animated [using timer](/src/string_animation.c).

7 of 9 scenes are created by artist [Svetlana Pimenova](https://instagram.com/arnica.tattoo).

Scene Start:

![Scene Start](/github-assets/scenes/scene_start_scaled_5x_minified.gif)

Scene Dance:

![Scene Dance](/github-assets/scenes/scene_dance_scaled_5x_minified.gif)

Scene Lera:

![Scene Lera](/github-assets/scenes/scene_lera_scaled_5x_minified.gif)

Scene Sorry:

![Scene Sorry](/github-assets/scenes/scene_sorry_scaled_5x_minified.gif)

Scene Stars:

![Scene Stars](/github-assets/scenes/scene_stars_scaled_5x_minified.gif)

Scene March:

![Scene March](/github-assets/scenes/scene_march_scaled_5x_minified.gif)

Scene Cookie:

![Scene Cookie](/github-assets/scenes/scene_cookie_scaled_5x_minified.gif)

Scene Current Moment:

![Scene Current Moment](/github-assets/scenes/scene_current_moment_scaled_5x_minified.gif)

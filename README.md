# PoshSpiceeeer

## About
This it a gift for a 21st bithday of my ex girlfriend. Main idea is a visualize a the core important moments of her life which was important for her soul development. This device based on STM32F401 MCU, uses ST7920-based 128x64 LCD with self-written library, perifery driven by STM32 LL library. Finally it contains 9 scenes with pixelart animations drawn by [Svetlana Pimenova](https://instagram.com/arnica.tattoo). Most part of code (like SceneManager) taken or/and inspired by [Flipper Zero Official Firmware](https://github.com/flipperdevices/flipperzero-firmware).

## Board
[Main schematics PDF](/github-assets/main_schematic.pdf)

EasyEDA 3D render:

<img src="/github-assets/board_easy_eda_render.png" width=60% />

Board IRL:

<img src="/github-assets/board_irl.jpg" width=60% />


## Scenes
For uploading to Github images was converted to GIFs by [Animated GIF Maker](https://ezgif.com/maker) and upscaled by [Pixel Art Scaler](https://lospec.com/pixel-art-scaler/). Most of scenes contains an animated text ([example](/src/scenes/app_scene_stars.c#L26)) was not present on GIFs below cause it prints by firmware and animated by [using timer](/src/string_animation.c). Also i would'n add scene's comments and descriptions due the privacy.

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

typedef struct {
    uint8_t* const frames;
    uint16_t frames;
    uint16_t current_frame;
} Animation;

void draw_callback(Animation* animation) {
    display_draw_picture(animation->frames[animation->current_frame]);
    display_draw_text("Test", 0, 0);
    display_draw_button_left("Back");
    display_draw_button_right("Next");
}

void input_callback(Button button) {
    if(button == BUTTON_LEFT) {
        scene_manager_previous_scene();
    } else if(button == BUTTON_RIGHT) {
        scene_manager_next_scene(SceneSecond);
    }
}

void animation_timer_callback(Animation* animation) {
    if(animation->current_frame > (animation->frames + 1))
        animation->current_frame = 0;
    else
        animation->current_frame++;
}

while(true) {
    draw_callback();
    display_sync_framebuffer();
}

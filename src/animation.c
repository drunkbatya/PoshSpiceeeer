#include <stdlib.h>
#include "animation.h"

Animation* animation_alloc(void) {
    Animation* animation = malloc(sizeof(Animation));
    animation->icon = NULL;
    animation->current_frame = 0;
    animation->direction = AnimationDirectionForward;
    return animation;
}

void animation_free(Animation* animation) {
    free(animation);
}

uint8_t animation_get_current_frame(Animation* animation) {
    return animation->current_frame;
}

void animation_draw_current_frame(Animation* animation, Display* display) {
    display_draw_icon_animation(display, animation->icon, animation->current_frame);
}

void animation_set_animation(Animation* animation, const Icon* icon, bool reverse_cycle) {
    animation->icon = icon;
    animation->current_frame = 0;
    animation->reverse_cycle = reverse_cycle;
}

void animation_reset_animation(Animation* animation) {
    animation->icon = NULL;
    animation->current_frame = 0;
    animation->direction = AnimationDirectionForward;
}

void animation_toggle_direction(Animation* animation) {
    if(animation->direction == AnimationDirectionForward)
        animation->direction = AnimationDirectionBackward;
    else
        animation->direction = AnimationDirectionForward;
}

void animation_switch_frame(Animation* animation) {
    if(animation->current_frame == 0 && animation->direction == AnimationDirectionBackward) {
        animation_toggle_direction(animation);
    } else if(
        animation->current_frame == (icon_get_frame_count(animation->icon) - 1) &&
        animation->direction == AnimationDirectionForward) {
        if(animation->reverse_cycle) {
            animation_toggle_direction(animation);
        } else {
            animation->current_frame = 0;
            return;
        }
    }
    if(animation->direction == AnimationDirectionForward) {
        animation->current_frame++;
    } else if(animation->direction == AnimationDirectionBackward) {
        animation->current_frame--;
    }
}

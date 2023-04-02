#include "my_main.h"
#include "st7920.h"

void my_main(void) {
    st7920_init();
    st7920_graphic_mode(true);
    while(true) {
        st7920_update();
    }
}

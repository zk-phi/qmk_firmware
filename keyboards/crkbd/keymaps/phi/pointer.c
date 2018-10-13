#include "pointing_device.h"
#include "crptr.h"

/* static char scroll_mode = 0; */

void pointer_init (void) {
    crptr_init();
}

void pointer_task (void) {
    report_mouse_t r = pointing_device_get_report();
    crptr_t value = crptr_read();

    r.x = value.x;
    r.y = value.y;

    pointing_device_set_report(r);
    pointing_device_send();
}

void pointer_process_record (uint16_t keycode, keyrecord_t *record) {
    /* TODO: Implement click & scroll-mode */
}

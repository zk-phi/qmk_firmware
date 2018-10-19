#include "pointing_device.h"
#include "crptr.h"

/* static char scroll_mode = 0; */

void pointer_init (void) {
    crptr_init();
}

#ifdef CRPTR_DEBUG_MODE
#include <stdio.h>
char crptr_status[20];
void pointer_task (void) {
    if (i2c_start_read(CRPTR_ADDR)) {
        sprintf(crptr_status, "ERROR");
    } else {
        uint8_t data[4];
        data[0] = i2c_master_read(1);
        data[1] = i2c_master_read(1);
        data[2] = i2c_master_read(1);
        data[3] = i2c_master_read(0);
        i2c_master_stop();
        sprintf(crptr_status, "0x%x,0x%x,0x%x,0x%x", data[0], data[1], data[2], data[3]);
    }
}
#else
void pointer_task (void) {
    report_mouse_t r = pointing_device_get_report();
    crptr_t value = crptr_read();

    r.x = value.x;
    r.y = value.y;

    pointing_device_set_report(r);
    pointing_device_send();
}
#endif

void pointer_process_record (uint16_t keycode, keyrecord_t *record) {
    /* TODO: Implement click & scroll-mode */
}

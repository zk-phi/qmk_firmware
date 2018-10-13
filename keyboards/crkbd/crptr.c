#ifdef CRPTR

#include <stdbool.h>
#include "crptr.h"

bool carib_completed = false;
crptr_t center = { .x = 0, .y = 0 };

void crptr_init (void) {
    i2c_master_init();
}

crptr_t crptr_read (void) {
    uint8_t bytes[4];
    crptr_t res;

    if (i2c_start_read(CRPTR_ADDR)) {
        res.x = 0;
        res.y = 0;
    } else {
        bytes[0] = i2c_master_read(0);
        bytes[1] = i2c_master_read(0);
        bytes[2] = i2c_master_read(0);
        bytes[3] = i2c_master_read(1);

        i2c_master_stop();

        /* uint8 000000xx, uint8 yyyyyyyy
           => uint8 xxyyyyyyyy
           => int16 00000000xxyyyyyyyy
        */
        res.x = (int16_t)((bytes[0] << 6) | (bytes[1] >> 2)) - 128;
        res.y = (int16_t)((bytes[2] << 6) | (bytes[3] >> 2)) - 128;


        if (!carib_completed) {
            center.x = res.x;
            center.y = res.y;
            res.x = 0;
            res.y = 0;
            carib_completed = true;
        } else {
            int16_t x = res.x - center.x;
            int16_t y = res.y - center.y;
            res.x = x < -128 ? -128 : x > 127 ? 127 : x;
            res.y = y < -128 ? -128 : y > 127 ? 127 : y;
        }
    }

    return res;
}

#endif

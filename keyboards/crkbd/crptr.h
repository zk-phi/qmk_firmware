#include "i2c.h"

#define CRPTR_ADDR 0x0A

typedef struct { int8_t x, y; } crptr_t;

void crptr_init (void);
crptr_t crptr_read (void);

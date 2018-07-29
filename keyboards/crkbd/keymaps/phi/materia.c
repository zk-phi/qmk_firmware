#include <stdlib.h>
#include "timer.h"
#include "ssd1306.h"
#include "serial.h"
#include "materiafont.h"

// shell
#define MATERIA_SURFACE_SIZE 24
static uint8_t surface_offset[] = {0x00, 0x18, 0x30, 0x48, 0x60, 0x78, 0x90, 0xA8, 0xC0, 0xD8};
static uint8_t blinked_surface[][10] = {
  {0x06, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};
// talk
#define MATERIA_NUM_TALKS 7
#define MATERIA_BALLOON_SIZE 40
// \1\s1さくら！それはあびさる・でぃじょんのしわざや！\0\s0あびさる・でぃじょん？\e
static uint8_t talk00[] = {
	0x0B, 0x01, 0x34, 0x2E, 0x68, 0x14, 0x3C, 0x6B, 0x4E, 0x21, 0x52, 0x34, 0x6A, 0x16, 0x46, 0x22, 0x37, 0x66, 0x72, 0x4D, 0x36, 0x6E, 0x35, 0x63, 0x14, 0x0A, 0x00, 0x21, 0x52, 0x34, 0x6A, 0x16, 0x46, 0x22, 0x37, 0x66, 0x72, 0x13, 0x0D, 
};
// \1\s1はじめまして。　ちよのちちです。\0\s0・・・・。\e
static uint8_t talk01[] = {
	0x0B, 0x01, 0x4E, 0x37, 0x60, 0x5D, 0x36, 0x45, 0x12, 0x10, 0x40, 0x67, 0x4D, 0x40, 0x40, 0x46, 0x38, 0x12, 0x0A, 0x00, 0x16, 0x16, 0x16, 0x16, 0x12, 0x0D, 
};
// \0\s1くるみわりにんぎょうってあるでしょ。　　\1\s0ああ。　\0\s0くるみにんぎょうわりってのはどうかな？\1\s0じしんなさそうにいうなよ。\e
static uint8_t talk02[] = {
	0x0A, 0x01, 0x2E, 0x6A, 0x5E, 0x6E, 0x69, 0x4A, 0x72, 0x2D, 0x66, 0x25, 0x42, 0x45, 0x21, 0x6A, 0x46, 0x36, 0x66, 0x12, 0x10, 0x10, 0x0B, 0x00, 0x21, 0x21, 0x12, 0x10, 0x0A, 0x00, 0x2E, 0x6A, 0x5E, 0x4A, 0x72, 0x2D, 0x66, 0x25, 0x6E, 0x69, 0x42, 0x45, 0x4D, 0x4E, 0x48, 0x25, 0x2A, 0x49, 0x13, 0x0B, 0x00, 0x37, 0x36, 0x72, 0x49, 0x34, 0x3C, 0x25, 0x4A, 0x23, 0x25, 0x49, 0x67, 0x12, 0x0D, 
};
// \0\s0でぃあなかうんたーってむねきゅん？\1\s0むねきゅんやね。\e
static uint8_t talk03[] = {
	0x0A, 0x00, 0x46, 0x22, 0x21, 0x49, 0x2A, 0x25, 0x72, 0x3E, 0x15, 0x42, 0x45, 0x5F, 0x4C, 0x2C, 0x64, 0x72, 0x13, 0x0B, 0x00, 0x5F, 0x4C, 0x2C, 0x64, 0x72, 0x63, 0x4C, 0x12, 0x0D, 
};
// \0\s0ぐりふぃすからてづくりべんとうがとどきました。\1\s0・・・・なんだよ。\e
static uint8_t talk04[] = {
	0x0A, 0x00, 0x2F, 0x69, 0x54, 0x22, 0x38, 0x2A, 0x68, 0x45, 0x44, 0x2E, 0x69, 0x58, 0x72, 0x47, 0x25, 0x2B, 0x47, 0x48, 0x2C, 0x5D, 0x36, 0x3E, 0x12, 0x0B, 0x00, 0x16, 0x16, 0x16, 0x16, 0x49, 0x72, 0x3F, 0x67, 0x12, 0x0D, 
};
// \0\s0とうちゅうかそうごっこしよう。\1\s0・・・・むりや。\e
static uint8_t talk05[] = {
	0x0A, 0x00, 0x47, 0x25, 0x40, 0x64, 0x25, 0x2A, 0x3C, 0x25, 0x33, 0x42, 0x32, 0x36, 0x67, 0x25, 0x12, 0x0B, 0x00, 0x16, 0x16, 0x16, 0x16, 0x5F, 0x69, 0x63, 0x12, 0x0D, 
};
// \0\s0けろぴーにはがんのよくせいぶっしつがはいってるらしいよ。\1\s0・・・・また、みのか？\0\s0うん。みの。\e
static uint8_t talk06[] = {
	0x0A, 0x00, 0x30, 0x6C, 0x53, 0x15, 0x4A, 0x4E, 0x2B, 0x72, 0x4D, 0x67, 0x2E, 0x3A, 0x23, 0x55, 0x42, 0x36, 0x43, 0x2B, 0x4E, 0x23, 0x42, 0x45, 0x6A, 0x68, 0x36, 0x23, 0x67, 0x12, 0x0B, 0x00, 0x16, 0x16, 0x16, 0x16, 0x5D, 0x3E, 0x11, 0x5E, 0x4D, 0x2A, 0x13, 0x0A, 0x00, 0x25, 0x72, 0x12, 0x5E, 0x4D, 0x12, 0x0D, 
};
static uint8_t *talk[] = { // to save memory usage...
  talk00,
  talk01,
  talk02,
  talk03,
  talk04,
  talk05,
  talk06
};
// timer
#define MATERIA_TALK_INTERVAL 20000
#define MATERIA_CHAR_INTERVAL 100
#define MATERIA_BLINK_INTERVAL 500
#define MATERIA_BLINK_LONG 500
// serial commands
#define MATERIA_SCMD_NOP 0x0
#define MATERIA_SCMD_START_TALK 0x1
#define MATERIA_SCMD_CLEAR_TALK 0x2
#define MATERIA_SCMD_BOSS_INCOMING 0x3
// sakura script
#define MATERIA_SS_SURFACE0 0x00
#define MATERIA_SS_SURFACE1 0x01
#define MATERIA_SS_SURFACE2 0x02
#define MATERIA_SS_SURFACE3 0x03
#define MATERIA_SS_SURFACE4 0x04
#define MATERIA_SS_SURFACE5 0x05
#define MATERIA_SS_SURFACE6 0x06
#define MATERIA_SS_SURFACE7 0x07
#define MATERIA_SS_SURFACE8 0x08
#define MATERIA_SS_SURFACE9 0x09
#define MATERIA_SS_SCOPE_MASTER 0x0a
#define MATERIA_SS_SCOPE_SLAVE 0x0b
#define MATERIA_SS_WAIT 0x0c
#define MATERIA_SS_END 0x0d
#define MATERIA_SS_CHAR_WS 0x10

struct talk_status {
  uint8_t *position;
  uint8_t scope;
  bool end;
  uint8_t count;
  uint16_t start_timer;
  uint16_t forward_timer;
};

struct shell_status {
  uint8_t id;
  uint8_t id_prev;
  uint8_t id_drawing;
  bool need_drawing;
  uint16_t blink_timer;
  uint16_t after_blinked_timer;
};

struct script_status {
  struct talk_status talk;
  struct shell_status shell;
};

static uint8_t myscope;
static struct script_status status;
uint8_t surface_buf[MATERIA_SURFACE_SIZE];
uint8_t balloon_buf[MATERIA_BALLOON_SIZE];

void init_balloon(void) {
  for (int i = 0; i < sizeof(balloon_buf); i++) {
    balloon_buf[i] = MATERIA_SS_CHAR_WS;
  }
}

void init_shell(void) {
  uint8_t c = surface_offset[0];
  for (int i = 0; i < sizeof(surface_buf); i++) {
    surface_buf[i] = c;
    c++;
  }
}

void init_materia(bool master) {
  if (master) {
    display.shell = font_shell_master;
    myscope = 0;
  } else {
    display.shell = font_shell_slave;
    myscope = 1;
  }
  status.talk.position = talk[0];
  status.talk.scope = 0;
  status.talk.end = true;
  status.talk.count = 0;
  status.talk.start_timer = timer_read();
  status.talk.forward_timer = timer_read();
  status.shell.id = 0;
  status.shell.id_prev = 0;
  status.shell.id_drawing = 0;
  status.shell.need_drawing = true;
  status.shell.blink_timer = timer_read();
  init_balloon();
  init_shell();
}

void update_serial_command(bool new_talk, uint8_t talk_id) {
  uint8_t serial_command = MATERIA_SCMD_NOP;
  if (new_talk) {
    serial_command = MATERIA_SCMD_START_TALK | (talk_id << 2);
  }
  serial_master_buffer[SERIAL_MASTER_BUFFER_LENGTH - 1] = serial_command;
}

uint8_t get_new_talk(void) {
  return (uint8_t)(rand() % MATERIA_NUM_TALKS);
}

void init_talk(uint8_t id) {
  status.talk.position = talk[id];
  status.talk.scope = 0;
  status.talk.end = false;
  status.talk.count = 0;
  status.talk.forward_timer = timer_read();
}

void forward_script(void) {
  uint8_t current = *status.talk.position;
  switch (current) {
    case MATERIA_SS_SURFACE0:
    case MATERIA_SS_SURFACE1:
    case MATERIA_SS_SURFACE2:
    case MATERIA_SS_SURFACE3:
    case MATERIA_SS_SURFACE4:
    case MATERIA_SS_SURFACE5:
    case MATERIA_SS_SURFACE6:
    case MATERIA_SS_SURFACE7:
    case MATERIA_SS_SURFACE8:
    case MATERIA_SS_SURFACE9:
      if (status.talk.scope == myscope) {
        status.shell.id_prev = status.shell.id;
        status.shell.id = current - MATERIA_SS_SURFACE0;
      }
      break;
    case MATERIA_SS_SCOPE_MASTER:
    case MATERIA_SS_SCOPE_SLAVE:
      status.talk.scope = current - MATERIA_SS_SCOPE_MASTER;
      break;
    case MATERIA_SS_WAIT: // TODO: implement
      break;
    case MATERIA_SS_END:
      status.talk.end = true;
      break;
    default:
      if (status.talk.scope == myscope) {
        balloon_buf[status.talk.count] = current;
        status.talk.count++;
      }
      break;
  }
  status.talk.position++;
}

void update_master_talk_status(void) {
  bool new_talk = false;
  uint8_t talk_id = 0;
  if ((timer_elapsed(status.talk.start_timer) >= MATERIA_TALK_INTERVAL)) {
    status.talk.start_timer = timer_read();
    talk_id = get_new_talk();
    init_talk(talk_id);
    init_balloon();
    new_talk = true;
  }
  update_serial_command(new_talk, talk_id);
}

void update_slave_talk_status(void) {
  uint8_t serial_command = serial_master_buffer[SERIAL_MASTER_BUFFER_LENGTH - 1];
  uint8_t command = serial_command & 0x3;
  uint8_t talk_id = serial_command >> 2;
  switch (command) {
    case MATERIA_SCMD_START_TALK: // TODO: implement
      init_talk(talk_id);
      init_balloon();
      break;
    case MATERIA_SCMD_CLEAR_TALK: // TODO: implement
    case MATERIA_SCMD_BOSS_INCOMING: // TODO: implement
    case MATERIA_SCMD_NOP:
    default:
      break;
  }
}

void update_script_status(void) {
  if ((timer_elapsed(status.talk.forward_timer) >= MATERIA_CHAR_INTERVAL)) {
    status.talk.forward_timer = timer_read();
    if (!status.talk.end) {
      forward_script();
    }
  }
}

bool has_blinked_surface(uint8_t id) {
  return (blinked_surface[myscope][id] != 0xFF);
}

void update_shell_status(void) {
  status.shell.need_drawing = false;
  if (status.shell.id_prev != status.shell.id) {
    status.shell.id_drawing = status.shell.id;
    status.shell.need_drawing = true;
    status.shell.blink_timer = timer_read();
  } else {
    if (has_blinked_surface(status.shell.id)) {
      if (status.shell.id != status.shell.id_drawing) { // blinked
        if ((timer_elapsed(status.shell.after_blinked_timer) >= MATERIA_BLINK_LONG)) {
          status.shell.id_drawing = status.shell.id;
          status.shell.need_drawing = true;
          status.shell.blink_timer = timer_read();
        }
      } else { // not blinked
        if (timer_elapsed(status.shell.blink_timer) >= MATERIA_BLINK_INTERVAL) {
          if (rand() % 4 == 0) {
            status.shell.id_drawing = blinked_surface[myscope][status.shell.id];
            status.shell.need_drawing = true;
            status.shell.after_blinked_timer = timer_read();
          } else {
            status.shell.blink_timer = timer_read();
          }
        }
      }
    } else {
      if (status.shell.id != status.shell.id_drawing) {
        status.shell.id_drawing = status.shell.id;
        status.shell.need_drawing = true;
      }
    }
  }
}

void update_surface_buf(void) {
  if (status.shell.need_drawing) {
    uint8_t c = surface_offset[status.shell.id_drawing];
    for (int i = 0; i < sizeof(surface_buf); i++) {
      surface_buf[i] = c;
      c++;
    }
  }
}

void render_materia(struct CharacterMatrix *matrix) {
  for (int i = 0; i < sizeof(balloon_buf); i++) {
    matrix_write_char(matrix, balloon_buf[i]);
  }
  for (int i = 0; i < sizeof(surface_buf); i++) {
    matrix_write_char(matrix, surface_buf[i]);
  }
}

void materia_task(struct CharacterMatrix *matrix, bool master) {
  if (master) {
    update_master_talk_status();
  } else {
    update_slave_talk_status();
  }
  update_script_status();
  update_shell_status();
  update_surface_buf();
  render_materia(matrix);
}

#ifndef MAILBOX_H
#define MAILBOX_H

#include "peripherals/base.h"
#include <stdint.h>

#define Mail_BASE (PBASE + 0xB880)
#define MAIL_EMPTY 0x40000000
#define MAIL_FULL 0x80000000

// registers

#define READ REG32(Mail_BASE + 0x00)
#define WRITE REG32(Mail_BASE + 0x20)
#define STATUS REG32(Mail_BASE + 0x18)
#define SENDER REG32(Mail_BASE + 0x14)
#define CONFIG REG32(Mail_BASE + 0x1c)
#define POLL REG32(Mail_BASE + 0x10)

//  propety tags
#define RPI_FIRMWARE_STATUS_SUCCESS 0x80000000
#define RPI_FIRMWARE_STATUS_REQUEST 0
#define MAILBOX_TAG_SET_POWER 0x28001
#define MAILBOX_TAG_SET_CLK_RATE 0x38002
#define MAILBOX_TAG_SET_PHYS_WH 0x48003
#define MAILBOX_TAG_SET_VIRT_WH 0x48004
#define MAILBOX_TAG_SET_VIRT_OFFSET 0x48009
#define MAILBOX_TAG_SET_DEPTH 0x48005
#define MAILBOX_TAG_SET_PIXEL_ORDER 0x48006
#define MAILBOX_TAG_ALLOCATE_BUFFER 0x40001
#define MAILBOX_TAG_GET_PITCH 0x40008
#define MAILBOX_TAG_LAST 0

// channels

#define MAIL_POWER 0x0   // Mailbox Channel 0: Power Management Interface
#define MAIL_FB 0x1      // Mailbox Channel 1: Frame Buffer
#define MAIL_VUART 0x2   // Mailbox Channel 2: Virtual UART
#define MAIL_VCHIQ 0x3   // Mailbox Channel 3: VCHIQ Interface
#define MAIL_LEDS 0x4    // Mailbox Channel 4: LEDs Interface
#define MAIL_BUTTONS 0x5 // Mailbox Channel 5: Buttons Interface
#define MAIL_TOUCH 0x6   // Mailbox Channel 6: Touchscreen Interface
#define MAIL_COUNT 0x7   // Mailbox Channel 7: Counter
#define MAIL_TAGS 0x8    // Mailbox Channel 8: Tags (ARM to VC)

typedef struct {
  uint32_t id;
  uint32_t buffer_size;
  uint32_t value_length;
} mailbox_tag;

typedef struct {
  uint32_t size;
  uint32_t code;
  uint8_t tags[0];
} property_buffer;

typedef struct {
  mailbox_tag tag;
  uint32_t width;
  uint32_t height;
} mailbox_fb_size;

typedef struct {
  mailbox_tag tag;
  uint32_t bpp;
} mailbox_fb_depth;

typedef struct {
  mailbox_tag tag;
  uint32_t pitch;
} mailbox_fb_pitch;

typedef struct {
  mailbox_tag tag;
  uint32_t base; // framebuffer address
  uint32_t screen_size;
} mailbox_fb_buffer;

typedef struct {
  mailbox_fb_size res;
  mailbox_fb_size vres; // virtual resolution..
  mailbox_fb_depth depth;
  mailbox_fb_buffer buff;
  mailbox_fb_pitch pitch;
  uint32_t end_tag;
} mailbox_fb_request;

// extern volatile struct mailbox_fb_request fb1;

#endif

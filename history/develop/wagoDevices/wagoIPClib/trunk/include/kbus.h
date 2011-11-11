#ifndef _KBUS_H
#define _KBUS_H

#include <stdint.h>

#define KBUS_TOTAL_SIZE   0x8001

#define KBUS_PI_INPUT_START 0x0000
#define KBUS_PI_OUTPUT_START  0x0200
#define KBUS_PI_SIZE    0x01F4

#define KBUS_CONFIG2_REG  0x03FC
#define KBUS_CONFIG1_REG  0x03FD
#define KBUS_RESPONSE_REG 0x03FE
#define KBUS_REQUEST_REG  0x03FF

#define KBUS_FW_STRING_LEN  256
#define ORDERNUMBER_STRING_LEN 64

#define KBUS_MAX_DATA_BYTES KBUS_CONFIG2_REG


#define KBUS_CTL_BIT_RUNSTOP  0x01
#define KBUS_CTL_BIT_D4   0x02  /* 0 = bootstrap mode */
#define KBUS_CTL_BIT_ICOM 0x04
#define KBUS_CTL_BIT_NOT_RESET  0x08  /* 1 = reset */

#define KBUS_SHM_NAME                   "/wago_kbus"
#define KBUS_RETAIN_SHM_NAME            "/wago_retain"
#define KBUS_PFC_INPUT_SHM_NAME         "/wago_plc_input"
#define KBUS_PFC_OUTPUT_SHM_NAME        "/wago_plc_output"

#define MAX_RETAIN_SEGMENT_SIZE         24576

#define KBUS_QKEY_PI_ACCESS             ((key_t)80)

#define KBUS_MAX_LENGTH_SHM_NAME        40

#define KBUS_MEMORY_TYPE_UNKNOWN        0
#define KBUS_MEMORY_TYPE_IO             1
//#define KBUS_MEMORY_TYPE_PFC            2
#define KBUS_MEMORY_TYPE_RETAIN         3
#define KBUS_MEMORY_TYPE_PFC_INPUT      4
#define KBUS_MEMORY_TYPE_PFC_OUTPUT     5

#define KBUS_MIN_BYTE_MEMORY_TYPE_IO    0
#define KBUS_MAX_BYTE_MEMORY_TYPE_IO    511
#define KBUS_MIN_BYTE_MEMORY_TYPE_PFC   512
#define KBUS_MAX_BYTE_MEMORY_TYPE_PFC   1022

#define KBUS_PI_ACCESS_READ             1
#define KBUS_PI_ACCESS_WRITE            2



/*
 * Note: The K-Bus interface controller can be reset selectively by setting
 *       the NOT_RESET bit to "1". I_COM, C165_D4 and RUN/STOP are control
 *       ports to the K-BUS controller and are used depending on firmware
 *       implementation. The K-Bus RUN/STOP port is not to mix with the
 *       onboard switch described in the MISC control block.
 */

/* This bit is only active for HW Index/02 or newer. The DEFAULT value of
 * this bit sets the routing, which was implemented for Index/01 or older
 * baseboards. So, this modification is upwards compatible.
 *
 * 0 = K-Bus routed to DSUB
 *     COM1, COM2 of XBoard isolated
 *
 * 1 = (DEFAULT)
 *     COM1 of XBoard routed to DSUB
 *     COM2 of XBoard routed to KBUS
 */
#define KBUS_CTL_BIT_COM_MUX  0x10

enum kbus_host_requests {
  KBUS_REQUEST_WRITE_OUTPUT_IMAGE   = 0x01,
  KBUS_REQUEST_READ_INPUT_IMAGE   = 0x02,
  KBUS_REQUEST_EXCHANGE_PROCESS_IMAGE = 0x07,
  KBUS_REQUEST_GET_TERMINAL_DIAG    = 0x71,
  KBUS_REQUEST_READ_EXT_CFG   = 0x72,
  KBUS_REQUEST_WRITE_EXT_CFG    = 0x79,
  KBUS_REQUEST_READ_WDI_CFG   = 0x7D,
  KBUS_REQUEST_READ_BITLENGTH   = 0x80,
  KBUS_REQUEST_WRITE_WDI_CFG    = 0x81,
  KBUS_REQUEST_READ_BIT_CONFIG    = 0x82,
  KBUS_REQUEST_CLEAR_DPRAM    = 0x85,
  KBUS_REQUEST_READ_CLAMP_TYPES   = 0x86,
  KBUS_REQUEST_READ_DIO_OFFSETS   = 0x87,
  KBUS_REQUEST_READ_BUS_STATUS    = 0x88,
  KBUS_REQUEST_READ_FIRMWARE_ID   = 0x89,
  KBUS_REQUEST_READ_OUTPUT_IMAGE    = 0x90,
  KBUS_REQUEST_READ_ADD_PI_INFO   = 0x91,
  KBUS_REQUEST_REGCOM_READ_WRITE    = 0x92,
  KBUS_REQUEST_SOFTWARE_RESET   = 0xAA,
  KBUS_REQUEST_RESET_WDI_CFG    = 0xBB,
  KBUS_REQUEST_SET_ORD_NUM = 0x7A,
  KBUS_REQUEST_ENALBE_SYNC  = 0x7F,
  KBUS_REQUEST_DISABLE_SYNC = 0x7E,
};

enum kbus_device_response {
  KBUS_RESPONSE_NONE    = 0x00,
  KBUS_RESPONSE_IN_PROGRESS = 0x11,
  KBUS_RESPONSE_PROCESSED   = 0x55,
  KBUS_RESPONSE_INVALID   = 0x66,
  KBUS_RESPONSE_ERROR   = 0xAA,
};

struct kbus_bus_status {
  uint16_t  bitlength;
  uint8_t   nr_clamps;
  uint8_t   error_type;
  uint8_t   error_arg;
  uint8_t   clamp_with_error;
  uint16_t  led_error_code;
  uint16_t  led_error_arg;
} __attribute__((packed));

struct kbus_digio_offsets {
  uint16_t  input_offset;
  uint16_t  output_offset;
} __attribute__((packed));

struct kbus_bit_config_entry {
  uint16_t  pos;
  uint8_t   type;
  uint8_t   size;
} __attribute__((packed));

#define KBUS_MAX_BIT_CONFIG_ENTRIES \
  ((KBUS_MAX_DATA_BYTES - 2) / sizeof(struct kbus_bit_config_entry))

struct kbus_bit_config {
  uint16_t  nr_io;
  struct kbus_bit_config_entry entries[KBUS_MAX_BIT_CONFIG_ENTRIES];
} __attribute__((packed));


#define KBUS_MAX_CLAMP_TYPE_ENTRIES ((KBUS_MAX_DATA_BYTES - 2) >> 1)

struct kbus_clamp_types {
  uint16_t  nr_io;
  uint16_t  type[KBUS_MAX_CLAMP_TYPE_ENTRIES];
} __attribute__((packed));

struct kbus_firmware_id {
  char    coupler_id[KBUS_FW_STRING_LEN];
  char    firmware_version[KBUS_FW_STRING_LEN];
} __attribute__((packed));


struct kbus_ordernumber {
  uint8_t padData[KBUS_PI_OUTPUT_START];
  uint8_t len;
  char    ordernumber_string[ORDERNUMBER_STRING_LEN];
}__attribute__((packed));

struct kbus_wdi_cfg {
  uint8_t   config2;
  uint8_t   config1;
} __attribute__((packed));


struct kbus_ext_cfg {
  uint8_t   ext_config2;
  uint8_t   ext_config1;
} __attribute__((packed));


struct kbus_bitlength_descr {
  uint16_t  nr_analog_out;
  uint16_t  nr_analog_in;
  uint16_t  nr_digital_out;
  uint16_t  nr_digital_in;
} __attribute__((packed));

struct kbus_process_data {
  uint8_t   data[KBUS_PI_SIZE];
} __attribute__((packed));

struct kbus_process_image {
  struct kbus_process_data inputs;
  uint8_t   padinp[KBUS_PI_OUTPUT_START - KBUS_PI_SIZE];
  struct kbus_process_data outputs;
} __attribute__((packed));

struct kbus_iec_process_image {
  struct kbus_process_data inputs;
  uint8_t   padinp[KBUS_PI_OUTPUT_START - KBUS_PI_SIZE];
  struct kbus_process_data outputs;
  uint8_t   padout[KBUS_PI_OUTPUT_START - KBUS_PI_SIZE];
} __attribute__((packed));

struct kbus_regcom_header {
  union {
    struct {
      uint8_t  terminal;
      uint8_t  table;
      uint16_t offset : 7;
      uint16_t write  : 1;
      uint16_t amount : 8;
      uint8_t  response;
    } bitf;
    uint8_t bytes[5];
  };
  uint16_t *data;
} __attribute__((packed));

struct kbus_add_pi_info {
  uint8_t terminals;
  uint8_t position;
  uint16_t terminalname;
  uint16_t bitposPAA;
  uint16_t bitposPAE;
  uint8_t sizePAA;
  uint8_t sizePAE;
  uint8_t channels;
  uint8_t dataformat;
} __attribute__((packed));

struct kbus_diag_data {
  uint16_t terminal;
  uint16_t channel;
  uint16_t data;
} __attribute__((packed));

struct GET_TERMINALDIAGtyp {
  uint8_t  EN;
  uint8_t  ENO;
  uint16_t TERMINAL;
  uint16_t CHANNEL;
  uint16_t DIAGDATA;
};

struct kbus_info {
  struct kbus_bus_status status;
  struct kbus_process_image image;
  struct kbus_digio_offsets offsets;
  struct kbus_bit_config bit_config;
  struct kbus_firmware_id firmware_id;
  struct kbus_bitlength_descr bitlength_descr;
  struct kbus_ext_cfg ext_cfg;
  struct kbus_wdi_cfg wdi_cfg;
  struct kbus_clamp_types clamp_types;
  struct kbus_ordernumber ordernumber;
  char firmware_string[KBUS_FW_STRING_LEN];
  int kbus_in_len;
  int kbus_out_len;
};

struct kbus_dpram {
  union {
    struct kbus_bus_status status;
    struct kbus_process_image image;
    struct kbus_digio_offsets offsets;
    struct kbus_bit_config bit_config;
    struct kbus_bitlength_descr bitlength_descr;
    struct kbus_clamp_types clamp_types;
    struct kbus_ordernumber ordernumber;
    char firmware_string[KBUS_FW_STRING_LEN];
    /*
     * data ensures that the union is using up the
     * full space up to CONFIG2_REG
     */
    uint8_t data[KBUS_MAX_DATA_BYTES];
  };
        union {
        struct kbus_wdi_cfg wdi_cfg;
        struct kbus_ext_cfg ext_cfg;
  };
  uint8_t   response;
  uint8_t   request;
} __attribute__((packed));

struct kbus_map {
  struct kbus_dpram dpram;        /* 0x0000 */
  uint8_t unused[0x8000 - sizeof(struct kbus_dpram)]; /* 0x0400 */
  /* ctlKBUS interface control */
  uint8_t ctl;            /* 0x8000 */
} __attribute__((packed));


// structure for the shared memory area to exchange the values of a PI segment (e.g. retain)
typedef struct
{
  // indicates, that the copy of the retain area was acutalized
  uint32_t  changeIndicator;

  // indicates an error occur
  uint8_t   status;

  // entire size of shm
  uint32_t  piSegmentSize;

  // copy of the retain segment
  uint8_t   aPiSegmentCopy[1];

} kbus_tPiAccessShm;



// structure for the message-queue to access process values
typedef struct 
{
  // content not required here, but necessairy as first element for message-queue-structures
  uint32_t  messageType;

  // access type
  uint8_t   accessType;
  
  // kind of memory, which should be accessed
  uint8_t   memoryType;

  // start-address (byte offset) for writing
  uint32_t  byteOffset;

  // bit-address for writing
  uint8_t   bitOffset;

  // count of bytes to copy
  uint16_t  byteCount;

  // bytes to copy
  uint32_t  value;

} kbus_tstPiAccessMessage;



extern int kbus_init(void);
extern void kbus_shutdown(void);
extern struct kbus_info *kbus_get_info(void);
extern struct kbus_add_pi_info *kbus_get_add_pi_info(void);
extern void kbus_clear_add_pi_info(void);

extern int kbus_trigger_io_update(void);
extern int kbus_check_io_update(void);
extern int kbus_read_bitlength(void);
extern int kbus_read_clamp_types(void);
extern int kbus_read_bit_config(void);
extern int kbus_read_bus_status(void);
extern int kbus_reset_wdi_cfg(void);
extern int kbus_write_wdi_cfg(void);
extern int kbus_read_wdi_cfg(void);
extern int kbus_exchange_process_image(void);
extern int kbus_read_input_image(void);
extern int kbus_read_output_image(void);
extern int kbus_write_output_image(void);
extern int kbus_read_digio_offsets(void);
extern int kbus_read_firmware_id(void);
extern int kbus_read_add_pi_info(uint8_t terminal);
extern int kbus_regcom_request(struct kbus_regcom_header *regcom);
extern int kbus_get_terminal_diag(struct GET_TERMINALDIAGtyp *pstInst);
extern int kbus_clear_dpram(void);
extern int kbus_reset(void);
extern int kbus_read_ext_cfg (void);
extern int kbus_write_ext_cfg (void);
extern int kbus_set_ordernumber (void);
extern int kbus_enable_sync_mode (void);
extern int kbus_disable_sync_mode (void);

extern void *kbus_lock_input_image(void);
extern void *kbus_lock_output_image(void);
extern void kbus_release_io_image(void);
extern uint16_t kbus_get_dig_input_offset(void);
extern uint16_t kbus_get_dig_output_offset(void);

extern void kbus_set_ctl(uint8_t ctl);
extern uint8_t kbus_get_ctl(void);

#endif

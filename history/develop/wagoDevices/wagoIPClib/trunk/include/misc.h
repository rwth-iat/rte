#ifndef _MISC_H
#define _MISC_H

#include <stdint.h>

//struct cif;

enum wago_misc_io_type {
	WAGO_MISC_IO_MMAP,
	WAGO_MISC_IO_PORT,
};

/* Identifier of the mapping type */
#define IPX_SPECIMEN 		"8" ///< at index 15 in string an "8" is documented as GEODE every other number is an other processor
#define GEODE_IDENTIFIER 	"Wago-IPC PLX"	///< This string identifies the processor type string in the uio directories 

extern uint8_t misc_get_low_power(void);
extern uint8_t misc_get_battery_ok(void);
extern uint8_t misc_get_run_stop_switch(void);
extern uint8_t misc_get_leds_num(void);
extern int8_t misc_set_led_ctrl(uint8_t nr, uint8_t userctrl);
extern int8_t misc_set_led_on(uint8_t nr);
extern int8_t misc_set_led_off(uint8_t nr);
extern int8_t misc_set_led_blinkcode(int8_t nr, uint16_t freq);
extern uint8_t misc_get_num_onboard_inputs(void);
extern uint8_t misc_get_num_onboard_outputs(void);
extern int8_t misc_get_one_onboard_input(uint8_t nr);
extern int8_t misc_get_one_onboard_output(uint8_t nr);
extern int8_t misc_set_one_onboard_output(uint8_t nr, uint8_t on);
extern int8_t misc_set_hwwdg_settime(unsigned long ulTimeOutMs);
extern int8_t misc_trigger_hwwdg(void);
extern uint8_t misc_get_fieldbus_type(void);
extern void misc_set_com_mux(uint8_t route);
extern uint8_t misc_get_dip_switch(void);
extern void misc_close(void);
extern int8_t misc_init(void);
extern unsigned long misc_ipc_get_targetid(void);

extern enum wago_misc_io_type wago_ipc_get_misc_io_type(void);
extern uint8_t * wago_ipc_get_misc_baseaddr(void);

#endif /* _MISC_H */

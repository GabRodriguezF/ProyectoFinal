#include "gpio_if.h"
#include "gpio_driver.h"

/* ===== Wrappers TFT ===== */
static void tft_cs_low_impl(void)    { TFT_CS_LOW(); }
static void tft_cs_high_impl(void)   { TFT_CS_HIGH(); }
static void tft_dc_cmd_impl(void)    { TFT_DC_COMMAND(); }
static void tft_dc_data_impl(void)   { TFT_DC_DATA(); }
static void tft_rst_low_impl(void)   { TFT_RST_LOW(); }
static void tft_rst_high_impl(void)  { TFT_RST_HIGH(); }

/* ===== Wrappers Motor dir ===== */
static void motor_left_impl(void)    { left_motor(); }
static void motor_right_impl(void)   { right_motor(); }

/* ===== Wrappers RGB ===== */
static void r_on_impl(void)          { LED_R_ON(); }
static void r_off_impl(void)         { LED_R_OFF(); }
static void g_on_impl(void)          { LED_G_ON(); }
static void g_off_impl(void)         { LED_G_OFF(); }
static void b_on_impl(void)          { LED_B_ON(); }
static void b_off_impl(void)         { LED_B_OFF(); }
static void r_tog_impl(void)         { blink_R(); }
static void g_tog_impl(void)         { blink_G(); }
static void b_tog_impl(void)         { blink_B(); }

/* ===== Wrappers BTN ===== */
static uint8_t btn_0_impl(void)		 { return (READ_BUTTON_0 == 0) ? 1 : 0; }
static uint8_t btn_1_impl(void)		 { return (READ_BUTTON_1 == 0) ? 1 : 0; }
static uint8_t btn_2_impl(void)		 { return (READ_BUTTON_2 == 0) ? 1 : 0; }
static uint8_t btn_3_impl(void)		 { return (READ_BUTTON_3 == 0) ? 1 : 0; }
static uint8_t btn_4_impl(void)		 { return (READ_BUTTON_4 == 0) ? 1 : 0; }

/* ===== ESTA ES LA "INSTANCIA" QUE SE INYECTA ===== */
const gpio_if_t GPIO_STM32 = {
    .tft = {
        .init       = TFT_ctrl_gpio_init,
        .cs_low     = tft_cs_low_impl,
        .cs_high    = tft_cs_high_impl,
        .dc_command = tft_dc_cmd_impl,
        .dc_data    = tft_dc_data_impl,
        .rst_low    = tft_rst_low_impl,
        .rst_high   = tft_rst_high_impl,
    },
    .motor = {
        .init  = gpio_pa2_pa3_output_init,
        .left  = motor_left_impl,
        .right = motor_right_impl,
    },
    .rgb = {
        .init     = GPIOB_Init_PB12_13_14_Output,
        .r_on     = r_on_impl,
        .r_off    = r_off_impl,
        .g_on     = g_on_impl,
        .g_off    = g_off_impl,
        .b_on     = b_on_impl,
        .b_off    = b_off_impl,
        .r_toggle = r_tog_impl,
        .g_toggle = g_tog_impl,
        .b_toggle = b_tog_impl,
    },
	.btn = {
		.init   = gpio_pc0_pc1_pc2_pc3_pc4_init,
		.btn_0  = btn_0_impl,
		.btn_1  = btn_1_impl,
		.btn_2  = btn_2_impl,
		.btn_3  = btn_3_impl,
		.btn_4  = btn_4_impl,
	}
};

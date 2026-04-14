// =========================== Archivo cabecera ===========================
#define STM32F405xx
#include "stm32f4xx.h"
// ============================ INCLUDES C ============================
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

// ============================ INCLUDES PROPIOS ============================
#include "gpio_if.h"
#include "spi_if.h"
#include "system_clock_if.h"
#include "pwm_if.h"
#include "quad_enc_if.h"
#include "usart_if.h"

#include "timer_driver.h"


#include "ILI9341_driver.h"
#include "mmcr.h"

/* Definición + inicialización de variables para el motor*/
volatile v_motor_t g_motor = V_MOTOR_DEFAULTS;
/* Definición + inicialización de variables para el RLS */
volatile variables_RLS_t g_RLS = V_MMCr_DEFAULTS;

float dbg_rpmr = 0.0f;
float dbg_rpme = 0.0f;

uint8_t flag_LED = 0;
uint8_t flag_multtimer = 0;
uint8_t cont_multtimer = 0;
uint8_t flag_cMotor = 0;
uint32_t intentos = 0;

volatile uint8_t rx_data;


int AntiRebote(gpio_btn_read_fn_t leer_btn);
void MostrarColor(uint8_t color);
void ApagarTodo();
uint8_t LeerBoton();

uint16_t LeerUsart(void);


void FPU_Enable(void);
void clear_fault_flags(void);
void ini_pantalla(void);


typedef union
{
    float f;
    uint8_t bytes_t[4];
}desc_data_t;


//Lectura
typedef struct __attribute__((packed)) {
    uint16_t PWM;
} Leer_PWM_t;

typedef struct __attribute__((packed)) {
    uint16_t Mayor, Menor, T_Mayor, T_Menor;
} Leer_Cuadrada_t;

typedef struct __attribute__((packed)) {
    uint16_t Mayor, Menor, Pendiente;
} Leer_Rampa_t;

typedef struct __attribute__((packed)) {
    uint16_t Mayor, Menor, Pendiente;
} Leer_Diente_t;

typedef struct __attribute__((packed)) {
    uint16_t Mayor, Menor, Pendiente;
} Leer_Triangulo_t;

typedef struct __attribute__((packed)) {
    uint16_t Engranes = 90, Pulsos = 22, Max_RPM = 110;
} Leer_DatosMotor_t;

Leer_PWM_t g_PWM;
Leer_Cuadrada_t g_Cuad;
Leer_Rampa_t g_Ramp;
Leer_Diente_t g_Diente;
Leer_Triangulo_t g_Triangulo;
Leer_DatosMotor_t g_DatosMotor;

int main(void)
{
    // Locales de arranque
    clear_fault_flags();
    FPU_Enable();
    //uint8_t aumento_vel = 0;
    //uint8_t estado = 0;


    SYSCLK_STM32.init();
    ini_pantalla();
    quad_STM32.init();
    PWM_STM32.motor.init(PWM_TARGET_HZ);
    usart3_STM32.init();//Inicialización de USART3 (PB10 Rx - PB11 Tx)
    GPIO_STM32.motor.init();
    GPIO_STM32.rgb.init();
    GPIO_STM32.btn.init();

    crear_C(g_RLS.C,10.10f);
    g_RLS.lambda = g_RLS.fhi * g_RLS.fhi; 
    g_RLS.inv_lambda = 1.0f / g_RLS.lambda;
    timer4_init();

    while (1) 
    {
        GPIO_STM32.motor.right();
        if (flag_multtimer == 1)
        {
        	PWM_STM32.motor.set_duty_permille(g_motor.d);/*
        	if (aumento_vel == 0) {
        		g_motor.d = g_motor.d + 1;
        	}
        	if (aumento_vel == 1) {
        		g_motor.d = g_motor.d - 1;
        	}
        	if (g_motor.d >= 990) {
        		aumento_vel = 1;
        	}
        	if (g_motor.d <= 200) {
        		aumento_vel = 0;
        	}

			//g_motor.d = 999;*/

            flag_multtimer = 0;
            if(g_RLS.ye > max_valor_eje_y )
            {
                g_RLS.ye = max_valor_eje_y;
            }
            if (g_RLS.ye < 0)
            {
                g_RLS.ye = 0;
            }
            if (g_motor.rpm > max_valor_eje_y)
            {
                g_motor.rpm = max_valor_eje_y;
            }
            if (g_motor.rpm < 0)
            {
                g_motor.rpm = 0;
            }
            
            ili_draw_graph(g_RLS.ye, g_motor.rpm, 100);
        }
    }
}

void MostrarColor(uint8_t color) {
    ApagarTodo();
    if (color == 1) GPIO_STM32.rgb.r_on();
    if (color == 2) GPIO_STM32.rgb.g_on();
    if (color == 3) GPIO_STM32.rgb.b_on();
}

void ApagarTodo() {
    GPIO_STM32.rgb.r_off();
    GPIO_STM32.rgb.g_off();
    GPIO_STM32.rgb.b_off();
}

uint8_t LeerBoton() {
    if (AntiRebote(GPIO_STM32.btn.btn_1)) { while(GPIO_STM32.btn.btn_1()); return 1; }
    if (AntiRebote(GPIO_STM32.btn.btn_2)) { while(GPIO_STM32.btn.btn_2()); return 2; }
    if (AntiRebote(GPIO_STM32.btn.btn_3)) { while(GPIO_STM32.btn.btn_3()); return 3; }
    return 0;
}

int AntiRebote(gpio_btn_read_fn_t leer_btn)
{
    // El botón parece estar presionado
    if (leer_btn() == 1)
    {
        // 2. Esperamos 20ms
        SYSCLK_STM32.delay_ms(20);

        // Sigue presionado?
        if (leer_btn() == 1)
        {
            return 1; //Real
        }
    }

    return 0; // Ruido
}

void ini_pantalla(void)
{
    GPIO_STM32.tft.init();
    PWM_STM32.tft_bl.init(10000, 450); // PWM Usado para brillo TFT
    SPI1_STM32.gpio_init();
    SPI1_STM32.init();
    GPIO_STM32.tft.cs_high();
    reset_TFT();
    ILI9341_init();
    ili_plantilla_grafica();
    ili_fill_graphs();
}
void FPU_Enable(void)
{
    SCB->CPACR |= (0xF << 20);  // Habilita CP10 y CP11 (FPU)
    __DSB();
    __ISB();
}
void clear_fault_flags(void)
{
    SCB->CFSR = 0xFFFFFFFF;   // limpia UFSR/BFSR/MMFSR
    SCB->HFSR = 0xFFFFFFFF;   // limpia hardfault status (w1c)
    SCB->DFSR = 0xFFFFFFFF;   // opcional
}
void TIM4_IRQHandler(void)
{
    //GPIO_STM32.rgb.r_toggle();//GPIO_STM32.rgb.r_on();
    uint8_t i = 0;
    if (TIM4->SR & TIM_SR_UIF) 
    { 
        TIM4->SR &= ~TIM_SR_UIF;   // clear UIF (escritura 0)
        g_motor.pos = quad_STM32.get_count();
        // 44 pulsos por vuelta directo en motor, muestra cada 10ms, relación engranes = 4.4:1, rpm max = 1360 
        g_motor.rpm = g_motor.pos / 22.0f * 100.0f * 60.0f / 90.0f; // rpm = (pulsos / pulsos_por_vuelta) * 100 (para pasar a segundos) * 60 (para pasar a minutos) / relación de engranajes
        g_motor.pos = 0;
        quad_STM32.reset();
        shift_right(g_motor.hist_rpm, 10, g_motor.rpm);
        if(intentos < 20)
        {
            g_RLS.hist_error[intentos] = 100.0f; // llenar el historial de error con un valor alto para evitar falsas activaciones del RLS al inicio
            intentos++;
        }
        else
        {
            g_motor.rpm = promedio_float(g_motor.hist_rpm, 10);
            dbg_rpmr = g_motor.rpm;

             //------------------------------------------------------------------------------------
            //------------------Se ejecula la estimación del modelo ARX---------------------------
            //------------------------------------------------------------------------------------
            g_RLS.ut = g_motor.d /1000.0f; // entrada normalizada (0.0 a 1.0)
            build_z(g_RLS.z, g_RLS.ut, g_RLS.ut_k_1, g_RLS.ut_k_2, g_RLS.y_k_1, g_RLS.y_k_2);       // z = vector de regresores
            
            g_RLS.ut_k_2 = g_RLS.ut_k_1;
            g_RLS.ut_k_1 = g_RLS.ut;
            g_RLS.y_k_2 = g_RLS.y_k_1;
            g_RLS.y_k_1 = g_motor.rpm/1000.0f; // y_k_1 = salida de la planta (rpm medida)

            g_RLS.ye = producto_punto(g_RLS.Pe, g_RLS.z, MAX_DIMX);           //===== Predicción con parámetros estimados ===== ye(k) = Pe.' * z;
            dbg_rpme = g_RLS.ye;
            g_RLS.e = g_motor.rpm - g_RLS.ye;                                   //===== Error a priori ===== e = y(k) - ye(k);
            shift_right(g_RLS.hist_error, 20, g_RLS.e);
            if(intentos == 20)
            {
                g_RLS.error_promedio = promedio_float(g_RLS.hist_error, 20);
                if (fabsf(g_RLS.error_promedio) > 5.0f && flag_cMotor == 0) // umbral de error promedio, ajustar según sea necesario
                {
                    multiplicar_matriz_vector(g_RLS.C, g_RLS.z, g_RLS.g);             // ======= RLS estable ======= g = C*z

                    g_RLS.zgpp = producto_punto(g_RLS.z, g_RLS.g, MAX_DIMX);          // ======= alpha^2 = lambda + z'*g =======
                    g_RLS.alfa2 = g_RLS.fhi * g_RLS.fhi + g_RLS.zgpp;
                                                                    
                    for (int i = 0; i < MAX_DIMX; i++)              // ======= Ganancia K = g / alfa2; =======
                    {
                        g_RLS.k_gain[i] = g_RLS.g[i] / g_RLS.alfa2;
                    }
                                
                    for (i = 0; i < MAX_DIMX; i++)                  // ======= Actualización de parámetros ======= Pe = Pe + K*e; =======
                    {
                        g_RLS.Pe[i] = g_RLS.Pe[i] + (g_RLS.k_gain[i] * g_RLS.e);
                    }
                                                                    // ======= Actualización covarianza (Joseph stabilized form) I = eye(size(C)); =======
                                                                    // ======= C = (1/lambda) * (I - K*z.') * C * (I - K*z.').' ; =======
                    crear_C(g_RLS.I,1.0f);                                        // identidad
                    producto_exterior(g_RLS.k_gain,g_RLS.z,g_RLS.kg_Z);                       // kg_Z = K*z.'
                    restar_matrices(g_RLS.I, g_RLS.kg_Z, g_RLS.I_minus_KZ);                   // I - K*z.'
                    escalar_matriz_out(g_RLS.I_minus_KZ, g_RLS.inv_lambda, g_RLS.A_scaled);   // A_scaled = (1/lambda) * (I - K*z.')
                    multiplicar_matrices(g_RLS.A_scaled, g_RLS.C, g_RLS.R);                   // R = A_scaled * C
                    // (1) Transpuesta
                    transponer_matriz(g_RLS.I_minus_KZ, g_RLS.I_minus_KZ_T);
                    // (2) C_new = R * (I-Kz^T)^T
                    multiplicar_matrices(g_RLS.R, g_RLS.I_minus_KZ_T, g_RLS.C_new);
                    // (3) C = C_new
                    copiar_matriz(g_RLS.C_new, g_RLS.C);
                    simetrizar(g_RLS.C);
                    clamp_covariance(g_RLS.C, 1e-3f, 1e3f);
                }
                if (fabs(g_RLS.error_promedio) < 5.0f)
                {
                    if(flag_cMotor == 0)
                    {
                        crear_C(g_RLS.C,10.10f);
                        g_RLS.alfa2 = 0.0f;
                        for(i = 0; i < MAX_DIMX; i++)
                        {
                            g_RLS.k_gain[i] = 0.0f;
                            g_RLS.g[i] = 0.0f;
                        }
                    }
                    flag_cMotor = 1;
                }
                else
                {
                    if (flag_cMotor == 1)
                    {
                    }
                    
                    flag_cMotor = 0;
                }
                
                
            }
            //------------------------------------------------------------------------------------
            //------------------------------------------------------------------------------------
        }

        desc_data_t desc_bytes_t;
        desc_bytes_t.f = g_motor.rpm;
		usart3_STM32.w_byte('i');
		usart3_STM32.w_byte(desc_bytes_t.bytes_t[3]);
		usart3_STM32.w_byte(desc_bytes_t.bytes_t[2]);
		usart3_STM32.w_byte(desc_bytes_t.bytes_t[1]);
		usart3_STM32.w_byte(desc_bytes_t.bytes_t[0]);
		desc_bytes_t.f = g_RLS.ye;
		usart3_STM32.w_byte(desc_bytes_t.bytes_t[3]);
		usart3_STM32.w_byte(desc_bytes_t.bytes_t[2]);
		usart3_STM32.w_byte(desc_bytes_t.bytes_t[1]);
		usart3_STM32.w_byte(desc_bytes_t.bytes_t[0]);
		desc_bytes_t.f = (float)g_motor.d;
		usart3_STM32.w_byte(desc_bytes_t.bytes_t[3]);
		usart3_STM32.w_byte(desc_bytes_t.bytes_t[2]);
		usart3_STM32.w_byte(desc_bytes_t.bytes_t[1]);
		usart3_STM32.w_byte(desc_bytes_t.bytes_t[0]);

        cont_multtimer = cont_multtimer + 1;
        if(cont_multtimer >= 10) // cada 100ms
        {
        	flag_LED = flag_LED + 1;
            flag_multtimer = 1;
            cont_multtimer = 0;
        }
        if(flag_LED >= 5)
        {
        	if(g_motor.d == 0){
        		GPIO_STM32.rgb.r_toggle();
        	}
        	else {
        		GPIO_STM32.rgb.r_off();
        	}
        	flag_LED = 0;
        }
    }
}

/*
void USART3_IRQHandler(void)
{
    if (USART3->SR & USART_SR_RXNE)
    {
    	rx_data = (uint8_t)USART3->DR;
        static uint8_t estado = 0;
        static uint8_t msb = 0;

        switch(estado) {
            case 0:
                if (rx_data == 'P') estado = 1;
                //if (rx_data == 'C') estado = 0;
				break;
            case 1:
                msb = rx_data;
                estado = 2;
                break;
            case 2:
            	//Combina los primeros 8 bits con los ultimos 8 bits
                uint16_t resultado = (uint16_t)((msb << 8) | rx_data);
                if (resultado <= 1000) { //Por si acaso
                    g_motor.d = resultado;
                }
                estado = 0;
                break;
            default:
                estado = 0;
                break;
        }
    }
}*/

void USART3_IRQHandler(void)
{
    if (USART3->SR & USART_SR_RXNE)
    {
        //rx_data = (uint8_t)USART3->DR;
    	rx_data = (uint8_t)usart3_STM32.r_byte();
        static uint8_t estado = 0;
        static uint8_t sub_estado = 0;
        static uint8_t limite_bytes = 0;
        static uint8_t *ptr_destino = NULL; // Puntero mágico

        switch(estado) {
            case 0: // BUSCAR ETIQUETA
                if (rx_data == 'P') {
                    ptr_destino = (uint8_t*)&g_motor.d; // Apunta al duty
                    limite_bytes = 2;
                    estado = 1;
                }
                else if (rx_data == 'C') {	//Cuadrado
                    ptr_destino = (uint8_t*)&g_Cuad;
                    limite_bytes = sizeof(Leer_Cuadrada_t);
                    estado = 1;
                }
                else if (rx_data == 'R') {	//Rampa
					ptr_destino = (uint8_t*)&g_Ramp;
					limite_bytes = sizeof(Leer_Rampa_t);
					estado = 1;
				}
                else if (rx_data == 'D') {	//Diente
					ptr_destino = (uint8_t*)&g_Diente;
					limite_bytes = sizeof(Leer_Diente_t);
					estado = 1;
				}
                else if (rx_data == 'T') {	//Triangulo
					ptr_destino = (uint8_t*)&g_Triangulo;
					limite_bytes = sizeof(Leer_Triangulo_t);
					estado = 1;
				}
                else if (rx_data == 'I') {	//Datos Motor
					ptr_destino = (uint8_t*)&g_DatosMotor;
					limite_bytes = sizeof(Leer_DatosMotor_t);
					estado = 1;
				}
                sub_estado = 0;
                break;

            case 1:
                if (ptr_destino != NULL) {
                    ptr_destino[sub_estado] = rx_data;
                    sub_estado++;

                    if (sub_estado >= limite_bytes) {
                    	uint16_t *datos16 = (uint16_t*)ptr_destino;
						uint8_t num_elementos = limite_bytes / 2;

						for(uint8_t i = 0; i < num_elementos; i++) {
							// Intercambio de bytes
							datos16[i] = (uint16_t)((datos16[i] << 8) | (datos16[i] >> 8));
						}

						estado = 0;
                    }
                }
                break;
        }
    }
}


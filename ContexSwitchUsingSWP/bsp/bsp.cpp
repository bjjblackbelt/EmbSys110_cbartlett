/**
 *******************************************************************************
 *  @filename   bsp.cpp
 *  @brief      Implements board specific functions
 *  @author     Chad Bartlett   <cbartlet@uw.edu>
 *******************************************************************************
 */
#include "bsp.h"
#include "lpc2378.h"
#include "pins.h"

/*
 *******************************************************************************
 * Uses SWP instruction to set the lock for a critical section.
 *
 * @param lock - A reference to the lock variable
 * @returns Returns the value of LOCKED_STATE if the lock was NOT acquired.
 *******************************************************************************
*/
CSStatus_t BSP_CSLock(int& lock)
{
	CSStatus_t lockStatus;

	asm volatile ("MOV r1, %2;"
				  "MOV r2, %1;"
				  "SWP %0, r1, [r2];"
			      : "=r" (lockStatus) 					/* Outputs: %0     */
				  : "r" (&lock), "I" (LOCKED_STATE)    	/* Inputs:  %1, %2 */
                  : "r1", "r2"                          /* Clobbered Regs  */
				  );

    return lockStatus;
}

/*
 *******************************************************************************
 *  This init is specific to the 2300 family of devices.
 *
 *  Initialize PLL (Configured for a 12.000 MHz crystal) to
 *  boost processor clock to 72.000 MHz
 *
 * @returns Nothing
 *******************************************************************************
*/
static void initPLL(void)
{
    /* [1] Check if PLL connected, disconnect if yes. */

    if (VOLATILE32(PLLSTAT)&PLLSTAT_PLLC)
    {
        VOLATILE32(PLLCON) = PLLCON_PLLE;
        /* Enable PLL, disconnected ( PLLC = 0)*/
        VOLATILE32(PLLFEED) = 0xaa;
        VOLATILE32(PLLFEED) = 0x55;
    }

    /* [2] Disable the PLL once it has been disconnected. */

    VOLATILE32(PLLCON)  = 0;
    VOLATILE32(PLLFEED) = 0xaa;
    VOLATILE32(PLLFEED) = 0x55;

    /* Enable the main oscillator */

    VOLATILE32(SCS) |= SCS_OSCEN;

    /* Wait until main OSC is usable */

    do {} while ((VOLATILE32(SCS) & SCS_OSCSTAT) == 0);

    /* Select the main osc as the PLL clk source. */

    VOLATILE32(CLKSRCSEL) = CLKSRC_MAIN_OSC;

    /*
     * NOTES:
     *
     * Set multiplier and divider values.
     *
     *  PLLCFG = ((N - 1) << 16) + (M - 1)
     *
     *  F_cco = (2 * M *F_in)/N
     *      M = 12, N = 1, F_in = 12.000 MHz -> F_cco = 288.000 MHz
     *
     */

    VOLATILE32(PLLCFG) = PLLCFG_VALUE;
    VOLATILE32(PLLFEED) = 0xAA;
    VOLATILE32(PLLFEED) = 0x55;

    /* Enable the PLL. */

    VOLATILE32(PLLCON)  = PLLCON_PLLE;
    VOLATILE32(PLLFEED) = 0xAA;
    VOLATILE32(PLLFEED) = 0x55;

    /* Divide F_cco down to get the CCLK output. (288 / 6 = 48) */

    VOLATILE32(CCLKCFG) = CCLKCFG_VALUE;

    /* Wait for the PLL to lock to set frequency */

    do {} while((VOLATILE32(PLLSTAT)&PLLSTAT_PLOCK)==0);

    do {} while((VOLATILE32(PLLSTAT)&0x00FF7FFF)!=PLLCFG_VALUE);

    /* Enable and connect the PLL as the clock source */

    VOLATILE32(PLLCON) = (PLLCON_PLLE | PLLCON_PLLC);
    VOLATILE32(PLLFEED) = 0xAA;
    VOLATILE32(PLLFEED) = 0x55;

    /* Check connect bit status and wait for connection. */

    do {} while((VOLATILE32(PLLSTAT)&PLLSTAT_PLLC)==0);
}

/*
 *******************************************************************************
 * Enable the MAM accelerator and sets number of clocks used for
 * flash memory fetch.
 *
 * @returns Nothing
 *******************************************************************************
*/
static void initMAM(void)
{
    VOLATILE32(MAMCR)=MAMCR_OFF;
    VOLATILE32(MAMTIM)=MAMTIM_3_CLK;
    VOLATILE32(MAMCR)=MAMCR_PART;
}

/*
 *******************************************************************************
 * Set the pclk for each peripheral. The default PDIV_4, but we do that
 * explicitly here to make it easier to change them later.
 *
 * @returns Nothing
 *******************************************************************************
*/

static void initPCLK(void)
{
    VOLATILE32(PCLKSEL0)=
    (
          SET_PCLK (PCLK_WDT,     PDIV_4)
        | SET_PCLK (PCLK_TIMER0,  PDIV_4)
        | SET_PCLK (PCLK_TIMER1,  PDIV_4)
        | SET_PCLK (PCLK_UART0,   PDIV_1)
        | SET_PCLK (PCLK_UART1,   PDIV_1)
        | SET_PCLK (PCLK_PWM1,    PDIV_4)
        | SET_PCLK (PCLK_I2C0,    PDIV_4)
        | SET_PCLK (PCLK_SPI,     PDIV_4)
        | SET_PCLK (PCLK_RTC,     PDIV_4)
        | SET_PCLK (PCLK_SSP1,    PDIV_4)
        | SET_PCLK (PCLK_DAC,     PDIV_4)
        | SET_PCLK (PCLK_ADC,     PDIV_4)
        | SET_PCLK (PCLK_CAN1,    PDIV_4)
        | SET_PCLK (PCLK_CAN2,    PDIV_4)
        | SET_PCLK (PCLK_ACF,     PDIV_4)
    );

    VOLATILE32(PCLKSEL1)=
    (
          SET_PCLK (PCLK_BAT_RAM, PDIV_4)
        | SET_PCLK (PCLK_GPIO,    PDIV_4)
        | SET_PCLK (PCLK_PCB,     PDIV_4)
        | SET_PCLK (PCLK_I2C1,    PDIV_4)
        | SET_PCLK (PCLK_SSP0,    PDIV_4)
        | SET_PCLK (PCLK_TIMER2,  PDIV_4)
        | SET_PCLK (PCLK_TIMER3,  PDIV_4)
        | SET_PCLK (PCLK_UART2,   PDIV_1)
        | SET_PCLK (PCLK_UART3,   PDIV_1)
        | SET_PCLK (PCLK_I2C2,    PDIV_4)
        | SET_PCLK (PCLK_I2S,     PDIV_4)
        | SET_PCLK (PCLK_MCI,     PDIV_4)
        | SET_PCLK (PCLK_SYSCON,  PDIV_4)
    );
}

/*
 *******************************************************************************
 * Setups all the pins of the LPC2378
 *
 * @returns Nothing
 *******************************************************************************
 */

void BSP_PinConfigurator(void)
{
    uint32_t n;
    uint32_t i;
    uint32_t pattern;

    n=0;

    while (mcuPins[n].pinSelect!=0)
    {
        pattern=0x00000000;

        for (i=0; i<16; i++)
        {
            pattern|=(mcuPins[n].pin[i].multiplex<<(mcuPins[n].pin[i].position*2));
        }

        VOLATILE32(mcuPins[n].pinSelect)=pattern; /* set the complete world */
        n ++;
    }

    /*
     * NOTE:
     *
     * Complete by setting the ETM to be disabled on PINSEL10
     *
     */
    VOLATILE32(PINSEL10) = ~(BIT(3)) & PINSEL10_MASK;   /* disable ETM, mask reserved bit to 0 */
}

/*
 *******************************************************************************
 * Sets up the GPIO pins
 *
 * @returns Nothing
 *******************************************************************************
 */

static void initGPIO(void)
{
    BSP_PinConfigurator();

    /*
     * Old method:
     *
     * VOLATILE32(U0_TX_PINSEL_REG) = (VOLATILE32(U0_TX_PINSEL_REG) & ~U0_TX_PINMASK ) | U0_TX_PINSEL;
     * VOLATILE32(U0_RX_PINSEL_REG) = (VOLATILE32(U0_RX_PINSEL_REG) & ~U0_RX_PINMASK ) | U0_RX_PINSEL;
     */

    /*
     * NOTE:
     *
     *  SCS controls whether the LPC2378 is set to use the legancy registers or the new
     *  fast GPIO control registers.
     *
     */

    VOLATILE32(SCS) |= BIT(0);  /* Fast GPIO / new registers */
}

/*
 *******************************************************************************
 * Sets up UART0 to be 38.4 K Baud, 8 data bits, no parity, and no
 * flow control
 *
 * @note experimental
 *
 * @returns Nothing
 *******************************************************************************
 */

void BSP_InitUART0(uint16_t baud, uint8_t mode, uint8_t fmode)
{
    VOLATILE8(U0LCR) = ULCR_DLAB_ENABLE;
    VOLATILE8(U0DLL) = (uint8_t) baud;
    VOLATILE8(U0DLM) = (uint8_t)(baud >> 8);
    VOLATILE8(U0LCR) = (mode & ~ULCR_DLAB_ENABLE);
    VOLATILE8(U0FCR) = fmode;
}

/*
 *******************************************************************************
 * Initializes all the hardware components for the UART0 to function.
 *
 * @note experimental
 *
 * @returns Nothing
 *******************************************************************************
 */
void BSP_InitHardware(void)
{
    /*
     * intialize main hardware components
     */

    initPLL();
    initMAM();
    initPCLK ();
    initGPIO();

    /* intialize specific hardware components for UART0 */
    BSP_InitUART0(UART_BAUD(HOST_BAUD_U0),UART_8N1,UART_FIFO_OFF);

    /* Turn off MCIPWR SD LED (near bottom left corner of LCD) */
    VOLATILE32(LED1_DIR) |= LED1_BIT;
    VOLATILE32(LED1_CLR)  = LED1_BIT;

    /* MEMMAP Choices are:
    BOOTLOADERMODE      0x00
    USERFLASHMODE       0x01
    USERRAMMODE         0x02
    EXTERNALMEMORYMODE  0x03
    */
    VOLATILE32(MEMMAP) = USERFLASHMODE;
}

/*
 *******************************************************************************
 * Dummy function for the exception vectors
 *
 * @returns Nothing
 *******************************************************************************
 */

void handlerDummy(void)
{
    /* forever */
    while(1) continue;
}


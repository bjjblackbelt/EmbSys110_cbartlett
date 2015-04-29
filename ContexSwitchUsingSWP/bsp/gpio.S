/* file lab2.s
 * DESCRIPTION: Assembly language functions to initialize and use gpio.
 * AUTHOR:      Chad Bartlett
 * CREATED:     20141122
 * REVISION HISTORY:
 *
 * 20141122 cb Initial version created.
 *
 */
.global     gpio_init       /* Make function name visible to linker   */
.global     gpio_read
.global     led_set
.global     led_clr
.global     delayMS
.code 32
.extern     printString     /* Make external function visible to assembler */

/* Constant defines */
USER_NONE   = 0
USER_BUT1   = 1
USER_BUT2   = 2
USER_CENTER = 3
USER_UP     = 4
USER_DOWN   = 5
USER_LEFT   = 6
USER_RIGHT  = 7
TOTAL_BTNS  = 8

/* Register Addresses */
GPIO0PIN     = 0xE0028000
GPIO0SET     = 0xE0028004
GPIO0DIR     = 0xE0028008
GPIO0CLR     = 0xE002800C
GPIO1PIN     = 0xE0028010
GPIO1SET     = 0xE0028014
GPIO1DIR     = 0xE0028018
GPIO1CLR     = 0xE002801C

SCS      = 0xE01FC1A0

/* Masks */
BTN1_MASK      = 0x20000000      /* bit 29 set for P0.29 */
BTN2_MASK      = 0x00040000      /* bit 18 set for P0.18 */
CENTER_MASK    = 0x02000000      /* bit 25 set for P1.25 */
UP_MASK        = 0x00040000      /* bit 18 set for P1.18 */
DOWN_MASK      = 0x00080000      /* bit 19 set for P1.19 */
LEFT_MASK      = 0x08000000      /* bit 27 set for P1.27 */
RIGHT_MASK     = 0x00400000      /* bit 22 set for P1.22 */
SLOW_GPIO_MODE = 0x00000001
LED_MASK       = 0x00002000      /* bit 13 set for P0.13 */

/* Register Aliases */
reg_addr   .req r0
reg_data   .req r1
reg_mask   .req r2
return_val .req r3



/* FUNCTION:    gpio_init
 * DESCRIPTION: Initializes GPIO for the user buttons
 * C PROTOTYPE: void gpio_init( void );
 * PARAMS:      none
 * RETURN:      none
 * NOTE:        none
 */
gpio_init:
    /* configure port for GPIO */
    LDR     reg_addr, =SCS                  // System Control & Status register address
    LDR     reg_data, [reg_addr]            // read: load scs register
    LDR     reg_mask, =SLOW_GPIO_MODE       // gpio ports 0 & 1 access mode
    MVN     reg_mask, reg_mask              // invert port mask
    AND     reg_data, reg_data, reg_mask    // modify: clear bit 0 to enable GPIO
    STR     reg_data, [reg_addr]

    /* configure GPIO port 0 (Buttons) */
    LDR     reg_addr, =GPIO0DIR             // PO direction register address
    LDR     reg_data, [reg_addr]            // read: load PO direction register
    MOV     reg_mask, #BTN1_MASK            // get first button mask bit and ...
    ORR     reg_mask, reg_mask, #BTN2_MASK  //      OR it to create a buttons mask
    MVN     reg_mask, reg_mask
    AND     reg_data, reg_data, reg_mask    // modify: set BUT1 and BUT2 pins (p0.29/18) to input
    STR     reg_data, [reg_addr]

    /* configure GPIO port 0 (LED) */
    LDR     reg_addr, =GPIO0DIR             // PO direction register address
    LDR     reg_data, [reg_addr]            // read: load PO direction register
    ORR     reg_data, reg_data, #LED_MASK   // modify: set LED pin (p0.13) to output
    STR     reg_data, [reg_addr]

    /* configure GPIO port 1 (Joystick) */
    LDR     reg_addr, =GPIO0DIR             // PO direction register address
    LDR     reg_data, [reg_addr]            // read: load PO direction register
    MOV     reg_mask, #CENTER_MASK          // get first joystick mask bit and ...
    ORR     reg_mask, reg_mask, #UP_MASK    //      OR it to create a joystick mask
    ORR     reg_mask, reg_mask, #DOWN_MASK  //      OR it to create a joystick mask
    ORR     reg_mask, reg_mask, #LEFT_MASK  //      OR it to create a joystick mask
    ORR     reg_mask, reg_mask, #RIGHT_MASK //      OR it to create a joystick mask
    MVN     reg_mask, reg_mask
    AND     reg_data, reg_data, reg_mask    // modify: set joystick pins to input
    STR     reg_data, [reg_addr]

    MOV    pc, lr                          // return

/* FUNCTION:    led_set
 * DESCRIPTION: Turn on the LED
 * C PROTOTYPE: void led_set( void );
 * PARAMS:      none
 * RETURN:      none
 * NOTE:        none
 */
led_set:
    /* turn on LED */
     LDR    reg_addr, =GPIO0CLR
     MOV    reg_mask, #LED_MASK
     STR    reg_mask, [reg_addr]

     MOV    pc, lr                          // return

/* FUNCTION:    led_set
 * DESCRIPTION: Turn off the LED
 * C PROTOTYPE: void led_set( void );
 * PARAMS:      none
 * RETURN:      none
 * NOTE:        none
 */
led_clr:
    /* turn off LED */
     LDR    reg_addr, =GPIO0SET
     MOV    reg_mask, #LED_MASK
     STR    reg_mask, [reg_addr]

     MOV    pc, lr                          // return

/* FUNCTION:    gpio_read
 * DESCRIPTION: Reads GPIO for the user buttons and joystick.
 * C PROTOTYPE: int gpio_read( void );
 * PARAMS:      none
 * RETURN:      Returns a unique value based on the button pressed. If more than
 *              one button is pressed it will return the first it detects.
 *              Possible return values are:
 *                  0,1,2,3,4,5,6
 * NOTE:        Refer to the constants defined at the top of this file for
 *              button number assignment.
 * OTHER R:     r4 - local return address
 *              r5 - masked data register value
 */
gpio_read:
    LDR     reg_addr, =GPIO0PIN             // set up address to read P0 pins
    LDR     reg_data, [reg_addr]            // read P0 pins
    MVN     reg_data, reg_data              // negate data register

    /* Load BUT1 data */
    MOV     reg_mask, #BTN1_MASK            // get first button mask bit and ...
    MOV     return_val, #USER_BUT1
    LDR     r4, =gpio_read_load_btn2
    B       gpio_read_compare_masks

gpio_read_load_btn2:
    MOV     reg_mask, #BTN2_MASK
    MOV     return_val, #USER_BUT2
    LDR     r4, =gpio_read_load_port1
    B       gpio_read_compare_masks

gpio_read_load_port1:
    LDR     reg_addr, =GPIO1PIN             // set up address to read P1 pins
    LDR     reg_data, [reg_addr]            // read P1 pins
    MVN     reg_data, reg_data              // negate data register

gpio_read_load_center:
    MOV     reg_mask, #CENTER_MASK
    MOV     return_val, #USER_CENTER
    LDR     r4, =gpio_read_load_up
    B       gpio_read_compare_masks

gpio_read_load_up:
    MOV     reg_mask, #UP_MASK
    MOV     return_val, #USER_UP
    LDR     r4, =gpio_read_load_down
    B       gpio_read_compare_masks

gpio_read_load_down:
    MOV     reg_mask, #DOWN_MASK
    MOV     return_val, #USER_DOWN
    LDR     r4, =gpio_read_load_left
    B       gpio_read_compare_masks

gpio_read_load_left:
    MOV     reg_mask, #LEFT_MASK
    MOV     return_val, #USER_LEFT
    LDR     r4, =gpio_read_load_right
    B       gpio_read_compare_masks

gpio_read_load_right:
    MOV     reg_mask, #RIGHT_MASK
    MOV     return_val, #USER_RIGHT
    LDR     r4, =gpio_read_no_button
    B       gpio_read_compare_masks

gpio_read_compare_masks:
    ANDS    r5, reg_data, reg_mask          // mask out all but buttons
    CMP     r5, reg_mask                    // to set Z flag if no buttons pressed
    MOVEQ   r0, return_val
    BEQ     gpio_read_return                // if button pressed return mask number
    MOVNE   pc, r4                          // go to next instruction set by previous button

gpio_read_no_button:
    MOV     r0, #USER_NONE

gpio_read_return:
    MOV     pc, lr                          // return



/* FUNCTION:    delay
 * DESCRIPTION: delays for xx milliseconds
 * C PROTOTYPE: void delay( uint32_t ms );
 * PARAMS:      ms - the number of ms to delay
 * RETURN:      none
 * NOTE:        Clock is assumed to run at 72 MHz
 * OTHER R:     r1: loop counter
 *              r2: multiplication constant for converting ms to us
 *              r3: modified delay time
 */
delayMS:
    /* 1. Initialization                                                  */
    MOV r1, #0                  /* initialize loop counter                */
    MOV r2, #3
    MOV r2, r2, LSL #11         /* r2 = r2 * 2^11 = 3 * 2048 = 6144       */
    ADD r2, r2, #700            /* r2 = r2 + 700 = 6844                   */
    MUL r3, r2, r0
delay_loop:
    CMP r1, r3
    ADDLT r1, #1
    BLT delay_loop
    MOV pc, lr                  /* return                                 */

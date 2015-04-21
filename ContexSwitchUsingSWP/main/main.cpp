/*
 * MODULE
 *
 *   main.c
 *
 * DESCRIPTION
 *
 *  Sets up the Olimex LPC2378-STK board and continuously outputs a "Hello
 *  World" banner through the UART0 port.
 *
 *  Used to verify the at the Eclipse IDE, Cygwin, CDT, and GNUARM
 *  have been installed correctly.
 *
 * OS
 *
 *  n/a
 *
 * PLATFORM
 *
 *  Olimex LPC2378-STK
 *
 * HISTORY
 *
 *   2007/10/01: Mitch Ishihara
 *     split init routines from main.c
 *     changed cEntry to main
 *   2007/09/12: Andrew N. Sloss:
 *     convert the main.c to use VOLATILExx(<address>) format
 *     added 1st draft comments
 *   2007/08/21: Andrew N. Sloss:
 *     inspired from James Lynch's example code demo2106_blink_flash.
 *   2014/11/16: Chad Bartlett
 *       Cleaned up previous code for labs and homework
 *       Added BubbleSort function
 *       Added unit tests for BubbleSort function
 *   2014/12/14: Chad Bartlett
 *       Completed Lab08
 *   2014/12/16: Chad Bartlett
 *       Completed HW08
****************************************************************************/
#define __MAIN_C__

//#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "lpc2378.h"
#include "init.h"
#include "print.h"
//#include "uarts.h"


/*=========================================================================*/
/*  DEFINE: All Structures and Common Constants                            */
/*=========================================================================*/
#define ENABLE_GPIO_READ 0

#if ENABLE_GPIO_READ
typedef enum {
    USER_NONE   = 0,
    USER_BUT1   = 1,
    USER_BUT2   = 2,
    USER_CENTER = 3,
    USER_UP     = 4,
    USER_DOWN   = 5,
    USER_LEFT   = 6,
    USER_RIGHT  = 7,
    TOTAL_BTNS  = 8,
} USER;

char* msgTemplate = {"\nButton pressed: "};
char* id[TOTAL_BTNS] = {"NONE", "BUT1", "BUT2", "CENTER", "UP", "DOWN", "LEFT", "RIGHT"};
#endif

/*=========================================================================*/
/*  DEFINE: Prototypes                                                     */
/*=========================================================================*/
//extern void InitXa( int X[], int N, int V);
#if ENABLE_GPIO_READ
extern void gpio_init(void);
extern int gpio_read(void);
extern void delayMS(uint32_t ms);
#endif

/*=========================================================================*/
/*  DEFINE: Definition of all local Data                                   */
/*=========================================================================*/

/*=========================================================================*/
/*  DEFINE: Definition of all local Procedures                             */
/*=========================================================================*/

/*=========================================================================*/
/*  DEFINE: All code exported                                              */
/*=========================================================================*/
/***************************************************************************/
/*  main                                                                   */
/***************************************************************************/
/*
 * NAME
 *
 *  main
 *
 * DESCRIPTION
 *
 *  Tests Prefetch and Data exceptions
 *
 *   "Chad Bartlett's HW 08: Prefetch Abort Exceptions"
 *
 * PARAMETERS
 *
 *  none...
 *
 * EXAMPLE
 *
 * from crt.S
 *
 *  B  main
 *
 * NOTES
 *
 *  This routine never terminates...
 *
 */
int main(void)
{
    /* initialize */
    initHardware();

#if ENABLE_GPIO_READ
    gpio_init();
#endif

    printString("\033[2J"); /* Clear entire screen */
    printString("\nChad Bartlett's HW 08: Prefetch Abort Exceptions");

    uint32_t count = 0;
    while(count < 10) {
        printString("\n-----\n\nLoop: ");
        printHex(count + 1);
        printString("\n\nTesting Data Abort\n");
        int *p = 0;   // null pointer
        *p = 0;       // boom! (null pointer dereferenced)

        printString("\nTesting Prefetch Abort\n");
        void (*fpVoid)(void);
        fpVoid = (void(*)(void))0x10000000;
        fpVoid();

        count++;
    }
    printString("\n-----\n\n> Test Success!\n");

    while (1)
    {
#if ENABLE_GPIO_READ
        int button = gpio_read();
        if ((button != USER_NONE) && (button < TOTAL_BTNS))
        {
            printString(msgTemplate);
            printString(id[button]);
        }

        delayMS(100);
#endif
    }
    /* never terminates, but put this here to make compiler happy ... */
    return(0);
}

/*** EOF ***/

/*
 * MODULE
 *
 *   debugger.c
 *
 * DESCRIPTION
 *
 *      Useful debugging functions.
 *
 * OS
 *
 *      n/a
 *
 * PLATFORM
 *
 *      Olimex LPC2378-STK
 *
 * HISTORY
 *
 *      20141214: Chad Bartlett
 *          Initially created. Copied from Jeff Bosch's Lab08 Notes in EMBSYS100
****************************************************************************/
 #include "print.h"

 /* Example abort handler that prints the type of abort and
  * the address where it happened. */
extern "C" void abortPrint(unsigned int type, unsigned int address)
 {
    if (type == 0) {
        printString("Data Abort at address: 0x");
    } else {
        printString("Prefetch Abort at address: 0x");
    }
    printHex(address);
    printString("\n");
 }

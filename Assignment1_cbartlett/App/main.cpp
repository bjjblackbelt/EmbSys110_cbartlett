#include <stddef.h>
#include "Bsp.h"
#include "DUartInFc.h"
#include "DUart.h"

DUartInFc* g_pUart = NULL;

int main(void)
{
    DUart uart;
    uart.Init();

    // Initialize global objects
    g_pUart = &uart;

    App::InitHardware();

    App::PrintStr("> Chad Bartlett's Assignment 1.\n\n");
    App::PrintStr("> Hardware initialization took: ");
    App::PrintHex(App::GetSysTick());
    App::PrintStr("-milliseconds\n");

    // Main loop
    while (1)
    {
    }
}


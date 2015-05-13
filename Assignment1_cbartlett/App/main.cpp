#include <stddef.h>
#include "Bsp.h"
#include "DUartIF.h"
#include "DUart.h"
#include "OS.h"

DUartIF* g_pUart = NULL;

int main(void)
{
    DUart uart;
    uart.Init();

    // Initialize global objects
    g_pUart = &uart;

    App::InitHardware();

    g_pUart->PrintStr("> Chad Bartlett's Assignment 1.\n\n");
    g_pUart->PrintStr("> Hardware initialization took: ");
    g_pUart->PrintUInt(App::GetSysTick());
    g_pUart->PrintStr("-milliseconds\n");

    // Main loop
    while (1)
    {
    }
}


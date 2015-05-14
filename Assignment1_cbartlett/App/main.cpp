#include <stddef.h>
#include "Bsp.h"
#include "IUart.h"
#include "DUart.h"
#include "OS.h"

namespace Bsp {
IUart* g_pUart = NULL;
}

int main(void)
{
    DUart uart;
    uart.Init();

    // Initialize global objects
    Bsp::g_pUart = &uart;

    Bsp::InitHardware();

    Bsp::g_pUart->PrintStr("> Chad Bartlett's Assignment 1.\n\n");
    Bsp::g_pUart->PrintStr("> Hardware initialization took: ");
    Bsp::g_pUart->PrintUInt(Bsp::GetSysTick());
    Bsp::g_pUart->PrintStr("-milliseconds\n");

    // Main loop
    while (1)
    {
    }
}


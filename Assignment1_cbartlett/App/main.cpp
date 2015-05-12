#include "Bsp.h"
#include "DUart.h"

int main(void)
{
    DUart uart;
    App::InitHardware(uart);

    App::PrintStr("> Chad Bartlett's Assignment 1.\n\n");
    App::PrintStr("> Hardware initialization took: ");
    App::PrintHex(App::GetSysTick());
    App::PrintStr("-milliseconds\n");

    // Main loop
    while (1)
    {
    }
}


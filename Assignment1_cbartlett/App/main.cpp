#include "Bsp.h"
#include "DUart.h"

int main(void)
{
    DUart uart;
    App::InitHardware(uart);

    App::PrintStr("> Chad Bartlett's Assignment 1.\n\n");
    App::PrintStr("> Hardware initialization took: ");
    // App::PrintDec(App::GetSysTick());
    App::PrintDec(32);
    App::PrintStr("-milliseconds\n");

    // Main loop
    while (1)
    {
    }
}


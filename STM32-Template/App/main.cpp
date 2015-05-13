#include <stddef.h>
#include "Bsp.h"
#include "DUartIF.h"
#include "DUart.h"

DUartIF* g_pUart = NULL;

int main(void)
{
    DUart uart;
    uart.Init();

    // Initialize global objects
    g_pUart = &uart;

    Bsp::InitHardware();

    Bsp::ClrLed(Bsp::PIN_LED_GREEN);

    //TODO: CMB - Remove
    int temp = Bsp::UNLOCKED_STATE;
    g_pUart->PrintStr("Temp A: ");
    g_pUart->PrintUInt(temp);
    Bsp::CSStatus_t state = Bsp::CSLock(&temp);
    g_pUart->PrintStr(" Temp B: ");
    g_pUart->PrintUInt(temp);
    g_pUart->PrintStr(" State: ");
    g_pUart->PrintUInt(state);
    g_pUart->PrintStr("\n");

    if ((temp != Bsp::LOCKED_STATE) || (state != Bsp::UNLOCKED_STATE))
    {
        g_pUart->PrintStr(" > !!!FAILED!!! \n");
    }

    // Main loop
    while (1)
    {
        switch (Bsp::ReadUserBtn())
        {
            case Bsp::BTN_PRESSED:
            {
                Bsp::TglLed(Bsp::PIN_LED_GREEN);
                break;
            }
            case Bsp::BTN_RELEASED:
            default :
            {
                //!< Do nothing
                break;
            }
        }

        g_pUart->PrintStr("Hello World. Number: ");
        g_pUart->PrintHex(0xC0DE);
        g_pUart->PrintStr("\n");

        Bsp::DelayMs(250);  // wait 250ms
    }
}


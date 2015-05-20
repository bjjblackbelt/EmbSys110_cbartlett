#include <stddef.h>
#include "Bsp.h"
#include "IUart.h"
#include "DUart.h"
#include "DTimer.h"

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

    Bsp::ClrLed(Bsp::PIN_LED_GREEN);

    //TODO: CMB - Remove
    int temp = Bsp::UNLOCKED_STATE;
    Bsp::g_pUart->PrintStr("Temp A: ");
    Bsp::g_pUart->PrintUInt(temp);
    Bsp::CSStatus_t state = Bsp::CSLock(&temp);
    Bsp::g_pUart->PrintStr(" Temp B: ");
    Bsp::g_pUart->PrintUInt(temp);
    Bsp::g_pUart->PrintStr(" State: ");
    Bsp::g_pUart->PrintUInt(state);
    Bsp::g_pUart->PrintStr("\n");

    if ((temp != Bsp::LOCKED_STATE) || (state != Bsp::UNLOCKED_STATE))
    {
        Bsp::g_pUart->PrintStr(" > !!!FAILED!!! \n");
    }

    DTimer timer;
    timer.Open();
    timer.Start();

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

        Bsp::g_pUart->PrintStr("Hello World. Number: ");
        Bsp::g_pUart->PrintHex(0xC0DE);
        Bsp::g_pUart->PrintStr("\n");

        Bsp::DelayMs(250);  // wait 250ms
    }
}


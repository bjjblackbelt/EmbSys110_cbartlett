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

    App::InitHardware();

    App::ClrLed(App::PIN_LED_GREEN);

    //TODO: CMB - Remove
    int temp = App::UNLOCKED_STATE;
    g_pUart->PrintStr("Temp A: ");
    g_pUart->PrintUInt(temp);
    App::CSStatus_t state = App::CSLock(&temp);
    g_pUart->PrintStr(" Temp B: ");
    g_pUart->PrintUInt(temp);
    g_pUart->PrintStr(" State: ");
    g_pUart->PrintUInt(state);
    g_pUart->PrintStr("\n");

    if ((temp != App::LOCKED_STATE) || (state != App::UNLOCKED_STATE))
    {
        g_pUart->PrintStr(" > !!!FAILED!!! \n");
    }

    // Main loop
    while (1)
    {
        switch (App::ReadUserBtn())
        {
            case App::BTN_PRESSED:
            {
                App::TglLed(App::PIN_LED_GREEN);
                break;
            }
            case App::BTN_RELEASED:
            default :
            {
                //!< Do nothing
                break;
            }
        }

        g_pUart->PrintStr("Hello World. Number: ");
        g_pUart->PrintHex(0xC0DE);
        g_pUart->PrintStr("\n");

        App::DelayMs(250);  // wait 250ms
    }
}


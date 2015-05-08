#include "Bsp.h"
#include "DUart.h"

int main(void)
{
    DUart uart;

    App::InitHardware(uart);

    App::ClrLed(App::PIN_LED_BLUE);
    App::ClrLed(App::PIN_LED_GREEN);

    //TODO: CMB - Remove
    int temp = App::UNLOCKED_STATE;
    App::PrintStr("Temp A: ");
    App::PrintHex(temp);
    App::CSStatus_t state = App::CSLock(temp);
    App::PrintStr(" Temp B: ");
    App::PrintHex(temp);
    App::PrintStr(" State: ");
    App::PrintHex(state);

    // Main loop
    while (1)
    {
        switch (App::ReadUserBtn())
        {
            case App::BTN_PRESSED:
            {
                App::TglLed(App::PIN_LED_BLUE);
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

        App::PrintStr("Hello World. Number: ");
        App::PrintHex(0xC0DE);
        App::PrintStr("\n");

        App::DelayMs(250);  // wait 250ms
    }
}


#include "Bsp.h"

int main(void)
{
    App::InitHardware();

    App::ClrLed(App::PIN_LED_BLUE);
    App::ClrLed(App::PIN_LED_GREEN);

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


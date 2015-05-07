#include "Bsp.h"

int main(void)
{
    App::InitHardware();

    App::ClrLed(App::LED_BLUE);
    App::ClrLed(App::LED_GREEN);

    // Main loop
    while (1)
    {
        switch (App::ReadUserBtn())
        {
            case App::BTN_PRESSED:
            {
                App::TglLed(App::LED_BLUE);
                App::TglLed(App::LED_GREEN);
                break;
            }
            case App::BTN_RELEASED:
            default :
            {
                //!< Do nothing
                break;
            }
        }

        App::DelayMs(100);  // wait 250ms
    }
}


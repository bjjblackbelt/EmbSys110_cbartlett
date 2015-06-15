#include <stddef.h>
#include "Bsp.h"
#include "IUart.h"
#include "DUart.h"
#include "Threads.h"
#include "TimingMeasurements.h"
#include "OS.h"

namespace Bsp {
IUart* g_pUart = NULL;
}

// Prototypes
void PrintOsError(OS::Error_t err, int line);

int main(void)
{
    // Mark end of initialization
    TIME_MEAS_SYS_INIT_STOP();

    // Enable double word stack alignment (recommended in Cortex-M3 r1p1)
    SCB->CCR |= SCB_CCR_STKALIGN_Msk;

    Bsp::InitHardware();

    DUart uart;
    uart.Init();

    // Initialize global objects
    Bsp::g_pUart = &uart;

    // Print initialization time
    Bsp::g_pUart->PrintStr("> Chad Bartlett's Assignment 2.\n\n");

    // Create threads
    const uint32_t THREAD_STACK_SIZE_IDLE      = 128;
    const uint32_t THREAD_STACK_SIZE_THREAD1   = 128;
    const uint32_t THREAD_STACK_SIZE_THREAD2   = 128;
    const uint32_t THREAD_STACK_SIZE_THREAD3   = 128;
    uint32_t stack0[THREAD_STACK_SIZE_IDLE]    = {0xDEADBEEF};
    uint32_t stack1[THREAD_STACK_SIZE_THREAD1] = {0xDEADBEEF};
    uint32_t stack2[THREAD_STACK_SIZE_THREAD2] = {0xDEADBEEF};
    uint32_t stack3[THREAD_STACK_SIZE_THREAD3] = {0xDEADBEEF};

    Thread::GlobalData_t data;
    Thread::Thread_t th0 = {&Thread::Idle, &data,    &stack0[0], Thread::STATE_READY, "Idle"};
    Thread::Thread_t th1 = {&Thread::Thread1, &data, &stack1[0], Thread::STATE_READY, "Producer"};
    Thread::Thread_t th2 = {&Thread::Thread2, &data, &stack2[0], Thread::STATE_READY, "Consumer"};
    Thread::Thread_t th3 = {&Thread::Thread3, &data, &stack3[0], Thread::STATE_READY, "Monitor "};

    // Register Threads
    OS::Error_t status = OS::RegisterThread(th0, THREAD_STACK_SIZE_IDLE);
    PrintOsError(status, (__LINE__ - 1));
    status = OS::RegisterThread(th1, THREAD_STACK_SIZE_THREAD1);
    PrintOsError(status, (__LINE__ - 1));
    status = OS::RegisterThread(th2, THREAD_STACK_SIZE_THREAD2);
    PrintOsError(status, (__LINE__ - 1));
    status = OS::RegisterThread(th3, THREAD_STACK_SIZE_THREAD3);
    PrintOsError(status, (__LINE__ - 1));

    // Start OS; never returns
    OS::Start(&stack0[THREAD_STACK_SIZE_IDLE - 1]);

    while (1)
    {
        //!< Should never get here
    }
}

void PrintOsError(OS::Error_t err, int line)
{
    if (err != OS::ERROR_NONE)
    {
        Bsp::g_pUart->PrintStr("OS::Err: ");
        Bsp::g_pUart->PrintHex(err);
        Bsp::g_pUart->PrintStr(" line ");
        Bsp::g_pUart->PrintUInt(line);
        Bsp::g_pUart->PrintStr("\n");
    }
}

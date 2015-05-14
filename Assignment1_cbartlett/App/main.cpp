#include <stddef.h>
#include "Bsp.h"
#include "IUart.h"
#include "DUart.h"
#include "Threads.h"
#include "OS.h"

namespace Bsp {
IUart* g_pUart = NULL;
OS* g_pOS = NULL;
}

int main(void)
{
    DUart uart;
    uart.Init();

    // Initialize global objects
    Bsp::g_pUart = &uart;

    Bsp::InitHardware();

    // Print initialization time
    Bsp::g_pUart->PrintStr("> Chad Bartlett's Assignment 1.\n\n");
    Bsp::g_pUart->PrintStr("> Hardware initialization took: ");
    Bsp::g_pUart->PrintUInt(Bsp::GetSysTick());
    Bsp::g_pUart->PrintStr("-milliseconds\n");

    // Create threads
    Thread::GlobalData_t data;
    Thread::Thread_t th1 = {&Thread::Idle, &data, Thread::UID_THREAD_IDLE, 0, Thread::STATE_INITIAL, "Idle"};

    // Initialize OS
    OS os(uart);
    Bsp::g_pOS = &os;

    os.RegisterThread(th1);
    os.PrintThreadInfo();
    os.Start();
}


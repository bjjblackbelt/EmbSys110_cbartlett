/**
 *******************************************************************************
 *  @file   OS.cpp
 *  @brief  Implementation of the OS class.
 *  @author Chad Bartlett   <cbartlet@uw.com>
 *******************************************************************************
 */
#include <stddef.h>
#include "OS.h"
#include "Threads.h"

OS::OS(DUartIF& uart)
: m_uart(&uart)
{
}

OS::~OS()
{
}

OS::Error_t OS::RegisterThread(Thread::Thread_t& thread)
{
    OS::Error_t status = OS::ERROR_NONE;

    if (thread.entry == NULL)
    {
        status = OS::ERROR_NULL;
    }

    return status;
}

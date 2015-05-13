/**
 *******************************************************************************
 *  @file   OS.cpp
 *  @brief  Implementation of the OS class.
 *  @author Chad Bartlett   <cbartlet@uw.com>
 *******************************************************************************
 */
#include <stddef.h>
#include <string.h>
#include "OS.h"
#include "Threads.h"

OS::OS(DUartIF& uart)
    : m_uart(&uart),
      m_threads()
{
    for (int i = 0; i < OS::MAX_THREAD_COUNT; ++i)
    {
        m_threads[i] = NULL;
    }
}

OS::~OS()
{
}

OS::Error_t OS::RegisterThread(Thread::Thread_t& thread)
{
    OS::Error_t status = OS::ERROR_NONE;

    if (m_threads[thread.uid] != NULL)
    {
        status = OS::ERROR_UID;
    }
    else if (thread.entry == NULL)
    {
        status = OS::ERROR_NULL;
    }

    if (status == OS::ERROR_NONE)
    {
        m_threads[thread.uid] = &thread;
    }

    return status;
}

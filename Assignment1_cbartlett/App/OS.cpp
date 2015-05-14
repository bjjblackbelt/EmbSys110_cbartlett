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

OS::OS(IUart& uart)
    : m_uart(&uart),
      m_threadQueue(),
      m_currThread(0),
      m_nextThread(0),
      m_nThreads(0)
{
    for (int i = 0; i < OS::MAX_THREAD_COUNT; ++i)
    {
        m_threadQueue[i] = NULL;
    }
}

OS::~OS()
{
}

OS::Error_t OS::RegisterThread(Thread::Thread_t& thread)
{
    OS::Error_t status = OS::ERROR_NONE;

    if (m_nThreads > (OS::MAX_THREAD_COUNT - 1))
    {
        status = OS::ERROR_MAX_THREADS_REGISTERED;
        goto RegisterCleanup;
    }

    if (thread.entry == NULL)
    {
        status = OS::ERROR_NULL;
        goto RegisterCleanup;
    }

    for (int i = 0; i < m_nThreads; ++i)
    {
        if (m_threadQueue[i]->uid == thread.uid)
        {
            status = OS::ERROR_UID_NOT_UNIQUE;
            goto RegisterCleanup;
        }
    }

    m_threadQueue[m_nThreads++] = &thread;

RegisterCleanup:
    return status;
}

OS::Error_t OS::Scheduler()
{
    OS::Error_t status = OS::ERROR_NULL;

    if (m_nThreads > 0)
    {
        SetNextThread();

        ContexSwitch();

        status = OS::ERROR_NONE;
    }

    return status;
}

void OS::ContexSwitch()
{
    //!< Execute next thread
    if (m_currThread < OS::MAX_THREAD_COUNT)
    {
        Thread::Thread_t* nextThread = m_threadQueue[m_currThread];
        nextThread->entry(nextThread->data);
    }

}

void OS::SetNextThread()
{
    // If at the end of the thread queue, wrap to beginning.
    // Okay to cast here because tested for > 0 above.
    if (m_currThread == static_cast<int_fast8_t>(m_nThreads - 1))
    {
        m_currThread = 0;
    }
    else
    {
        m_currThread++;
    }
}

int_fast8_t OS::GetCurrentThreadID()
{
    if (m_nThreads > 0)
    {
        return static_cast<int_fast8_t>(m_threadQueue[m_currThread]->uid);
    }
    else
    {
        return (-1);
    }
}

uint_fast8_t OS::GetNumberOfThreads()
{
    return m_nThreads;
}

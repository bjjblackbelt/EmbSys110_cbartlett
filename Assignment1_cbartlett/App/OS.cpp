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
#include "IUart.h"

#define ENABLE_STATE_CHANGE_PRINTING 1
#if ENABLE_STATE_CHANGE_PRINTING
#define DEBUG_PRINT_STATE_CHANGE()      PrintThreadInfo()
#else
#define DEBUG_PRINT_STATE_CHANGE()
#endif

OS::OS(IUart& uart)
    : m_uart(&uart),
      m_threadQueue(),
      m_currThread(0),
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

    for (uint_fast8_t i = 0; i < m_nThreads; ++i)
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

        if (m_threadQueue[m_currThread]->state != Thread::STATE_BLOCKED)
        {
            m_threadQueue[m_currThread]->state = Thread::STATE_ACTIVE;
            DEBUG_PRINT_STATE_CHANGE();
        }

        status = OS::ERROR_NONE;
    }

    return status;
}

void OS::PrintThreadInfo()
{
    if (m_nThreads > 0)
    {
        m_uart->PrintStr("Name: ");
        m_uart->PrintStr(m_threadQueue[m_currThread]->name);
        m_uart->PrintStr("\n");
        m_uart->PrintStr("State: ");
        m_uart->PrintHex(m_threadQueue[m_currThread]->state);
        m_uart->PrintStr("\n");
    }
    else
    {
        m_uart->PrintStr("There are no registered threads.\n");
    }
}

CriticalSection::Status_t OS::EnterCS(CriticalSection& cs)
{
    CriticalSection::Status_t status = cs.Query(m_threadQueue[m_currThread]->uid);

    if (status == CriticalSection::BUSY)
    {
        m_threadQueue[m_currThread]->state = Thread::STATE_BLOCKED;
        DEBUG_PRINT_STATE_CHANGE();
    }

    status = cs.Enter(m_threadQueue[m_currThread]->uid);

    m_threadQueue[m_currThread]->state = Thread::STATE_READY;
    DEBUG_PRINT_STATE_CHANGE();

    return (status);
}

CriticalSection::Status_t OS::LeaveCS(CriticalSection& cs)
{
    CriticalSection::Status_t status = cs.Leave(m_threadQueue[m_currThread]->uid);
    return (status);
}

CriticalSection::Status_t OS::QueryCS(CriticalSection& cs)
{
    CriticalSection::Status_t status = cs.Query(m_threadQueue[m_currThread]->uid);
    return (status);
}

void OS::Start()
{
    if (m_nThreads > 0)
    {
        for (uint_fast8_t i = 0; i < m_nThreads; ++i)
        {
            m_threadQueue[i]->state = Thread::STATE_READY;
            DEBUG_PRINT_STATE_CHANGE();
        }

        // Start task timer
        // Execute Idle task
    }
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
    if (m_currThread == (m_nThreads - 1))
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

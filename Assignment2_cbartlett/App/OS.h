/**
 *******************************************************************************
 *  @file   OS.h
 *  @brief  The OS class of the tiny kernel.
 *  @author Chad Bartlett   <cbartlet@uw.com>
 *******************************************************************************
 */
#ifndef OS_H
#define OS_H

#include <stdint.h>
#include "CriticalSection.h"
#include "Threads.h"


/**
 * @class OS
 * @brief This class implements scheduling and contex switching duties.
 */
namespace OS {

//!< The Idle task is in the first position of the thread queue
static const uint32_t UID_THREAD_IDLE = 0;
//!< Limit the number of tasks
static const uint32_t MAX_THREAD_COUNT = 4;

/**
 * @enum Error_t
 * @brief Indications for potential errors of this class.
 */
typedef enum
{
    ERROR_NONE,                         //!< Success
    ERROR_NULL,                         //!< Thread function, stack, or data is NULL
    ERROR_MAX_THREADS_REGISTERED,       //!< Thread accessor is out of bounds
} Error_t;

/**
 * @struct State_t
 * @brief This structure defines the current state of the OS
 * @var State_t::Thread_t*
 * Memnber 'threadQueue' is an array of all registered threads
 * @var State_t::currentThread
 * Memnber 'currentThread' is the currently executing thread
 * @var State_t::nextThread
 * Memnber 'nextThread' is the next thread to execute
 * @var State_t::nThreads
 * Memnber 'nThreads' the total number of register threads
 */
struct StateStr
{
    Thread::Thread_t* threadQueue[MAX_THREAD_COUNT];
    uint32_t threadDelay[MAX_THREAD_COUNT][2];      //!< [threadId][0] - delay?; [threadId][1] - delay until ticks
    uint8_t currThread;
    uint8_t nextThread;
    uint8_t nThreads;
};
typedef StateStr State_t;

/**
 * Registers a thread within the OS.
 * @param  thread An instance of the thread structure to be registered.
 * @param  stackSize The size of the task stack (in words)
 * @return        Returns ERROR_NONE upon success, else an error code.
 */
Error_t RegisterThread(Thread::Thread_t& thread, uint32_t stackSize);

/**
 * Determines the next thread to execute.
 * @pre The number of registered threads has to be at least 1.
 */
void SetNextReadyThread();

/**
 * Starts the OS. The first thread is executed.
 */
void Start(uint32_t* topOfIdleStack);

/**
 * Prints the state of the currently running thread.
 */
void PrintThreadInfo();

/**
 * Blocking delay function in milliseconds.
 *
 * @param timeMs The number of milliseconds to delay.
 */
void TimeDlyMs(uint32_t timeMs);

/**
 * Function to perform any OS duties every sys tick.
 */
void TimeTick();

/**
 * A wrapper function to enter a critical section.
 * @param  cs A CriticalSection instance.
 * @return    Returns the status of the critical seciton.
 */
CriticalSection::Status_t EnterCS(CriticalSection& cs);

/**
 * A wrapper function to leave a critical section.
 * @param  cs A CriticalSection instance.
 * @return    Returns the status of the critical seciton.
 */
CriticalSection::Status_t LeaveCS(CriticalSection& cs);

/**
 * A wrapper function to query a critical section.
 * @param  cs A CriticalSection instance.
 * @return    Returns the status of the critical seciton.
 */
CriticalSection::Status_t QueryCS(CriticalSection& cs);

/**
 * Helper function to initialize the individual thread stacks.
 * @param thread The current thread
 * @param stackSize The size of the thread stack
 */
void InitializeThreadStack(Thread::Thread_t& thread, uint32_t stackSize);
};

#endif // #ifndef OS_H

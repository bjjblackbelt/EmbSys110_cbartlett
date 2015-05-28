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

// Forward declarations
class IUart;
class DTimer;

namespace Thread {
struct ThreadStr;
typedef ThreadStr Thread_t;
}

/**
 * @class OS
 * @brief This class implements scheduling and contex switching duties.
 */
class OS
{
  public:
    static const uint8_t MAX_THREAD_COUNT = 4;

    /**
     * @enum Error_t
     * @brief Indications for potential errors of this class.
     */
    typedef enum
    {
        ERROR_NONE,                         //!< Success
        ERROR_NULL,                         //!< Thread function, stack, or data is NULL
        ERROR_UID_NOT_UNIQUE,               //!< Thread with this UID is already registered
        ERROR_MAX_THREADS_REGISTERED,       //!< Thread accessor is out of bounds
    } Error_t;

    /**
     * Constructor
     */
    OS(IUart& uart, DTimer& timer);

    /**
     * Destructor
     */
    ~OS();

    /**
     * Registers a thread within the OS.
     * @param  thread An instance of the thread structure to be registered.
     * @return        Returns ERROR_NONE upon success, else an error code.
     */
    Error_t RegisterThread(Thread::Thread_t& thread);

    /**
     * Determines the next thread to execute.
     * @pre The number of registered threads has to be at least 1.
     */
    void SetNextReadyThread();

    /**
     * Gets the currently running thread UID.
     * @return Returns -1 if no threads are registered, else the current thread ID.
     */
    int_fast8_t GetCurrentThreadID();

    /**
     * Gets the total number of registered threads.
     * @return Returns the total number of registered threads.
     */
    uint_fast8_t GetNumberOfThreads();

    /**
     * Starts the OS. The first thread is executed.
     */
    void Start();

    /**
     * Prints the state of the currently running thread.
     */
    void PrintThreadInfo();

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
     * Sets the current stack pointer (SP) from the ISR.
     * @param sp The current stack pointer.
     */
    inline void SetCurrentSP(uint32_t sp){m_threadStacks[m_currThread].cur = (uint32_t*)sp;}

    /**
     * Gets the current stack pointer.
     * @return The current task's stack pointer
     */
    inline uint32_t GetCurrentSP(){return ((uint32_t)(m_threadStacks[m_currThread].cur));}

  private:
    OS(const OS&);            //!< Intentionally not implemented
    OS& operator=(const OS&); //!< Intentionally not implemented


    /**
     * @struct Stack_t
     * @brief This structure defines shared data between threads.
     * @var Stack_t::THREAD_STACK_SIZE_WORDS
     * Memnber 'THREAD_STACK_SIZE_WORDS' is the size of the stack in words
     * @var Stack_t::stack
     * Memnber 'stack' provides storage for the stack of an individual thread.
     * @var Stack_t::top
     * Memnber 'top' is the address of the top of stack
     * @var Stack_t::bot
     * Memnber 'bot'* is the address of the bottom of stack
     * @var Stack_t::cur
     * Memnber 'cur'* is the address of the next slot in the stack to be written.
     */
    struct StackStr
    {
        static const uint32_t THREAD_STACK_SIZE_WORDS = 64;
        uint32_t stack[THREAD_STACK_SIZE_WORDS];
        uint32_t* top;
        uint32_t* bot;
        uint32_t* cur;
    };
    typedef StackStr Stack_t;

    IUart* m_uart;                                      //!< A pointer to a UART instance
    DTimer* m_timer;                                    //!< A pointer to a timer instance
    Thread::Thread_t* m_threadQueue[MAX_THREAD_COUNT];  //!< Container for the threads of this app
    Stack_t m_threadStacks[MAX_THREAD_COUNT];           //!< Storage for the thread stacks
    volatile uint_fast8_t m_currThread;                 //!< The current thread queue index
    uint_fast8_t m_nThreads;                            //!< The number of registered threads
};

#endif // #ifndef OS_H

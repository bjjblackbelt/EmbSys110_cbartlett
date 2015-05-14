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

// Forward declarations
class IUart;

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
    OS(IUart& uart);

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
     * Switches to the next task to be executed.
     * @return        Returns ERROR_NONE upon success, else an error code.
     */
    Error_t Scheduler();

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

  private:
    OS(const OS&);            //!< Intentionally not implemented
    OS& operator=(const OS&); //!< Intentionally not implemented

    /**
     * Switch to the next thread.
     * @pre The number of registered threads has to be at least 1.
     */
    void ContexSwitch();

    /**
     * Determines the next thread to execute.
     * @pre The number of registered threads has to be at least 1.
     */
    void SetNextThread();

    IUart* m_uart;                                  //!< A pointer to UART instance
    Thread::Thread_t* m_threadQueue[MAX_THREAD_COUNT];  //!< Container for the threads of this app
    uint_fast8_t m_currThread;                      //!< The current thread queue index
    uint_fast8_t m_nextThread;                      //!< The next thread queue index to execute
    uint_fast8_t m_nThreads;                        //!< The number of registered threads
};

#endif // #ifndef OS_H

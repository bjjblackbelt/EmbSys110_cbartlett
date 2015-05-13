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
class DUartIF;

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
    /**
     * @enum Error_t
     * @brief Indications for potential errors of this class.
     */
    typedef enum
    {
        ERROR_NONE,     //!< Success
        ERROR_NULL,     //!< Thread function, stack, or data is NULL
        ERROR_NAME,     //!< Name length beyond MAX_NAME_LENGTH characters
        ERROR_UID,      //!< Thread with this aID is already registered
    } Error_t;

    /**
     * Constructor
     */
    OS(DUartIF& uart);

    /**
     * Destructor
     */
    ~OS();

    /**
     * Registers a thread within the OS.
     * @param  thread An instance of the thread structure to be registered.
     * @return        Returns ERROR_NONE upon successful registration, else
     *                an error code.
     */
    Error_t RegisterThread(Thread::Thread_t& thread);

  private:
    OS(const OS&);            //!< Intentionally not implemented
    OS& operator=(const OS&); //!< Intentionally not implemented

    DUartIF* m_uart;            //!< A pointer to UART instance
};

#endif // #ifndef OS_H

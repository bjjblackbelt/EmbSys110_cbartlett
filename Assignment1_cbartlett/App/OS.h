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
struct Thread_t;

/**
 * @class OS
 * @brief This class implements scheduling and contex switching duties.
 */
class OS
{
  public:
    typedef enum
    {
        ERROR_NONE,
        ERROR_NULL_FUNCTION,
        ERROR_PRIORITY,
        ERROR_NAME,
        ERROR_UID,
    } Error_t;

    /**
     * Constructor
     */
    OS();

    /**
     * Destructor
     */
    ~OS();

    Error_t RegisterThread(Thread_t& thread);

  protected:
    OS(const OS&);            //!< Intentionally not implemented
    OS& operator=(const OS&); //!< Intentionally not implemented

};

#endif // #ifndef OS_H

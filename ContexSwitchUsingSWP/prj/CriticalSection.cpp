/**
 *******************************************************************************
 *  @filename   CriticalSection.cpp
 *  @brief      Implements a critical section interface.
 *  @author     Chad Bartlett   <cbartlet@uw.edu>
 *******************************************************************************
 */
#include "CriticalSection.h"

/**
 * Constructor
 */
CriticalSection::CriticalSection()
{
}

/**
 * Destructor
 */
CriticalSection::~CriticalSection()
{
}

/**
* Take the critical section for this thread. If it is not available then block
* until it is.
*
* This may be entered multiple times within the same thread. The critical
* section will not be released until the corresponding number of
* LeavecriticalSection() calls have been made within the same thread.
*
* @return SUCCESS - Critical section was taken
* @return non-zero - An error code
*/
CriticalSection::Status_t CriticalSection::Enter()
{
    return SUCCESS;
}

/**
* Release the critical section for this thread.
*
* @return SUCCESS - Critical section was released
* @return non-zero An error code
*/
CriticalSection::Status_t CriticalSection::Leave()
{
    return SUCCESS;
}

/**
* Take the critical section for this thread if it is available. The call will
* return immediately if critical section is unavailable.
*
* @param cs Critical section container
* @return SUCCESS - Critical section was taken
* @return BUSY - Critcial section is held by another thread
* @return non-zero An error code
*/
CriticalSection::Status_t CriticalSection::Query()
{
    return SUCCESS;
}


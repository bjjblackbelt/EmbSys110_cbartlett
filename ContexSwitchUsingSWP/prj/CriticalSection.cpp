/**
 *******************************************************************************
 *  @filename   CriticalSection.cpp
 *  @brief      Implements a critical section interface.
 *  @author     Chad Bartlett   <cbartlet@uw.edu>
 *******************************************************************************
 */
#include "CriticalSection.h"
#include "bsp.h"

/**
 * Constructor
 */
CriticalSection::CriticalSection()
    : m_locked(0),
      m_nLocks(0),
      m_threadID(-1)
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
CriticalSection::Status_t CriticalSection::Enter(int threadID)
{
    CriticalSection::Status_t status = BUSY;

    if (BSP_CSLock(m_locked) != LOCKED_STATE)
    {
        m_threadID = threadID;
        m_nLocks++;
        status = SUCCESS;
    }
    else if ((m_threadID == threadID))
    {
        m_nLocks++;
        status = SUCCESS;
    }
    else
    {
        status = BUSY;
    }

    return status;
}

/**
* Release the critical section for this thread.
*
* @return SUCCESS - Critical section was released
* @return non-zero An error code
*/
CriticalSection::Status_t CriticalSection::Leave(int threadID)
{
    CriticalSection::Status_t status;
    return BUSY;
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
CriticalSection::Status_t CriticalSection::Query(int threadID)
{
    CriticalSection::Status_t status;
    return BUSY;
}


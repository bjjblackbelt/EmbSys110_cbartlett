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
    : m_lockedState(UNLOCKED_STATE),
      m_nLocks(0),
      m_currThreadID(-1)
{
}

/**
 * Destructor
 */
CriticalSection::~CriticalSection()
{
}

/**
 * Destructor
 */
void CriticalSection::ResetDataMembers()
{
	m_lockedState = UNLOCKED_STATE;
	m_nLocks = 0;
	m_currThreadID = -1;
}

/**
* Take the critical section for this thread. If it is not available then block
* until it is.
*
* This may be entered multiple times within the same thread. The critical
* section will not be released until the corresponding number of
* LeavecriticalSection() calls have been made within the same thread.
*
* @param threadID - ID of currently running thread
*
* @return SUCCESS - Critical section was taken
* @return BUSY - Critcial section is held by another thread
*/
CriticalSection::Status_t CriticalSection::Enter(int threadID)
{
    CriticalSection::Status_t status = BUSY;
    int state = static_cast<int>(m_lockedState);

    if (BSP_CSLock(state) != LOCKED_STATE)
    {
        m_currThreadID = threadID;
        m_lockedState = static_cast<CSStatus_t>(state);
        m_nLocks++;
        status = SUCCESS;
    }
    else if (m_currThreadID == threadID)
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
* @param threadID - ID of currently running thread
* @return SUCCESS - Critical section was released
* @return BUSY - Critcial section is held by another thread
*/
CriticalSection::Status_t CriticalSection::Leave(int threadID)
{
    CriticalSection::Status_t status = BUSY;

    if (m_currThreadID == threadID)
	{
    	if (m_nLocks > 0)
    	{
    		status = SUCCESS;
    		m_nLocks--;

    	}

		if (m_nLocks == 0)
		{
    		ResetDataMembers();
		}
	}

    return status;
}

/**
* Take the critical section for this thread if it is available. The call will
* return immediately if critical section is unavailable.
*
* @param threadID - ID of currently running thread
* @return SUCCESS - Critical section was taken
* @return BUSY - Critcial section is held by another thread
* @return non-zero An error code
*/
CriticalSection::Status_t CriticalSection::Query(int threadID)
{
    CriticalSection::Status_t status;
    return BUSY;
}


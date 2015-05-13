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
    : m_lockedState(Bsp::UNLOCKED_STATE),
      m_nLocks(0),
      m_currThreadID(THREAD_ID_RESET)
{
}

/**
 * Destructor
 */
CriticalSection::~CriticalSection()
{
}

/**
 * Reset data members
 */
void CriticalSection::ResetDataMembers()
{
	m_lockedState = Bsp::UNLOCKED_STATE;
	m_nLocks = 0;
	m_currThreadID = THREAD_ID_RESET;
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
* @return BUSY - Critical section is held by another thread
*/
CriticalSection::Status_t CriticalSection::Enter(int threadID)
{
	// Block indefinitely
	CriticalSection::Status_t status = BUSY;

	do
	{
		status = Query(threadID);
	} while(status == BUSY);

	return status;
}

/**
* Release the critical section for this thread.
*
* @param threadID - ID of currently running thread
* @return SUCCESS - Critical section was released
* @return RESOURCES - No critical sections held for any threads
* @return BUSY - Critical section is held by another thread
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
    else if (m_currThreadID == THREAD_ID_RESET)
    {
    	status = RESOURCES;
    }
    else
    {
    	status = BUSY;
    }

    return status;
}

/**
* Take the critical section for this thread if it is available. The call will
* return immediately if critical section is unavailable.
*
* @param threadID - ID of currently running thread
* @return SUCCESS - Critical section was taken
* @return BUSY - Critical section is held by another thread
*/
CriticalSection::Status_t CriticalSection::Query(int threadID)
{
    CriticalSection::Status_t status = BUSY;
    int state = static_cast<int>(m_lockedState);

    if (Bsp::CSLock(&state) != Bsp::LOCKED_STATE)
    {
        m_currThreadID = threadID;
        m_lockedState = static_cast<Bsp::CSStatus_t>(state);
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


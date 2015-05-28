/**
 *******************************************************************************
 *  @filename   CriticalSection.cpp
 *  @brief      Implements a critical section interface.
 *  @author     Chad Bartlett   <cbartlet@uw.edu>
 *******************************************************************************
 */
#include "CriticalSection.h"

CriticalSection::CriticalSection()
    : m_lockedState(Bsp::UNLOCKED_STATE),
      m_nLocks(0),
      m_currThreadID(THREAD_ID_RESET)
{
}

CriticalSection::~CriticalSection()
{
}

void CriticalSection::ResetDataMembers()
{
	m_lockedState = Bsp::UNLOCKED_STATE;
	m_nLocks = 0;
	m_currThreadID = THREAD_ID_RESET;
}

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


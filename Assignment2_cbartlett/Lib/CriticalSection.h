/**
 *******************************************************************************
 *  @filename   CriticalSection.h
 *  @brief      See implementation file for class description.
 *  @author     Chad Bartlett   <cbartlet@uw.edu>
 *******************************************************************************
 */
#ifndef CRITICAL_SECTION_H
#define CRITICAL_SECTION_H

#include "Bsp.h"

class CriticalSection
{
  public:
    /**
     * Constructor
     */
    CriticalSection();

    /**
     * Reset data members
     */
    ~CriticalSection();

    typedef enum
    {
        SUCCESS,
        BUSY,
        RESOURCES
    } Status_t;

    /**
    * Take the critical section for this thread. If it is not available then
    * block until it is.
    *
    * This may be entered multiple times within the same thread. The critical
    * section will not be released until the corresponding number of
    * LeavecriticalSection() calls have been made within the same thread.
    *
    * @param threadID - ID of currently running thread
    *
    * @return SUCCESS - Critical section was taken
    */
    Status_t Enter(int threadID);

    /**
    * Release the critical section for this thread.
    *
    * @param threadID - ID of currently running thread
    * @return SUCCESS - Critical section was released
    * @return RESOURCES - No critical sections held for any threads
    * @return BUSY - Critical section is held by another thread
    */
    Status_t Leave(int threadID);

    /**
    * Take the critical section for this thread if it is available. The call
    * will return immediately if critical section is unavailable.
    *
    * @param threadID - ID of currently running thread
    * @return SUCCESS - Critical section was taken
    * @return BUSY - Critical section is held by another thread
    */
    Status_t Query(int threadID);

  private:
    CriticalSection(const CriticalSection&);            //!< Intentionally not implemented
    CriticalSection& operator=(const CriticalSection&); //!< Intentionally not implemented

    /**
     * Reset data members
     */
    void ResetDataMembers();

    static const int THREAD_ID_RESET = -1;

    volatile Bsp::CSStatus_t m_lockedState;
    volatile unsigned int m_nLocks;
    volatile int m_currThreadID;
};

#endif // #ifndef CRITICAL_SECTION_H

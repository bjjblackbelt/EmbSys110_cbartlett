/**
 *******************************************************************************
 *  @filename   CriticalSection.h
 *  @brief      See implementation file for class description.
 *  @author     Chad Bartlett   <cbartlet@uw.edu>
 *******************************************************************************
 */
#ifndef CRITICAL_SECTION_H
#define CRITICAL_SECTION_H

class CriticalSection
{
  public:
    CriticalSection();
    ~CriticalSection();

    typedef enum
    {
        SUCCESS,
        BUSY,
        RESOURCES
    } Status_t;

    Status_t Enter();
    Status_t Leave();
    Status_t Query();

  private:
    bool m_locked;
    int m_nLocks;
};

#endif // #ifndef CRITICAL_SECTION_H

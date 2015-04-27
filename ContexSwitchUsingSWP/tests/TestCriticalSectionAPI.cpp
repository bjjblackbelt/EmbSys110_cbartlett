#include <TestHarness.h>

#include "CriticalSection.h"
#include "bsp.h"

static const int s_threadID1 = 0xC0DE;
static const int s_threadID2 = 0xCAFE;

TEST_GROUP(TestCriticalSectionAPI)
{
    CriticalSection* m_pCS;

    void setup()
    {
        m_pCS = new CriticalSection;
    }

    void teardown()
    {
        delete m_pCS;
    }
};

TEST(TestCriticalSectionAPI, Initialize)
{
    CHECK(m_pCS != NULL);
}

TEST(TestCriticalSectionAPI, ASingleThreadCanTakeMultipleCriticalSections)
{
    int nCriticalSections = 10;
    for (int i = 0; i < nCriticalSections; ++i)
    {
        CriticalSection::Status_t status = m_pCS->Enter(s_threadID1);

        CHECK_EQUAL(CriticalSection::SUCCESS, status);
    }
}

TEST(TestCriticalSectionAPI, ASecondThreadCannotTakeTheCriticalSectionIfTakenByFirst)
{
    CriticalSection::Status_t status = CriticalSection::BUSY;

    status = m_pCS->Enter(s_threadID1);
    CHECK_EQUAL(CriticalSection::SUCCESS, status);

    status = m_pCS->Enter(s_threadID2);
    CHECK_EQUAL(CriticalSection::BUSY, status);
}

TEST(TestCriticalSectionAPI, ASecondThreadCannotTakeLockUntilNumLocksForFirstThreadIsZero)
{
    CriticalSection::Status_t status = CriticalSection::BUSY;
    int nCriticalSections = 3;

    for (int i = 0; i < nCriticalSections; ++i)
    {
    	CriticalSection::Status_t status = m_pCS->Enter(s_threadID1);
        CHECK_EQUAL(CriticalSection::SUCCESS, status);
    }

    status = m_pCS->Enter(s_threadID2);
    CHECK_EQUAL(CriticalSection::BUSY, status);

    for (int i = 0; i < (nCriticalSections - 1); ++i)
    {
        CriticalSection::Status_t status = m_pCS->Leave(s_threadID1);
        CHECK_EQUAL(CriticalSection::SUCCESS, status);
    }

    // Still should have 1 lock left
    status = m_pCS->Enter(s_threadID2);
    CHECK_EQUAL(CriticalSection::BUSY, status);

    // Release last lock
    status = m_pCS->Leave(s_threadID1);
    CHECK_EQUAL(CriticalSection::SUCCESS, status);

    // Second thread acquires lock
    status = m_pCS->Enter(s_threadID2);
    CHECK_EQUAL(CriticalSection::SUCCESS, status);
}

TEST(TestCriticalSectionAPI, ThereAreNoSideEffectsIfLeaveCsCalledWhenNoLocks)
{
	CriticalSection::Status_t status = m_pCS->Enter(s_threadID1);
    CHECK_EQUAL(CriticalSection::SUCCESS, status);}

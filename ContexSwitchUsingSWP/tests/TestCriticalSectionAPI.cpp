#include <TestHarness.h>

#include "CriticalSection.h"

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
        int threadID_1 = 0xC0DE;
        CriticalSection::Status_t status = m_pCS->Enter(threadID_1);

        CHECK_EQUAL(CriticalSection::SUCCESS, status);
    }
}

TEST(TestCriticalSectionAPI, ASecondThreadCannotTakeTheCriticalSection)
{
    CriticalSection::Status_t status = CriticalSection::BUSY;

    int threadID_1 = 0xC0DE;
    status = m_pCS->Enter(threadID_1);
    CHECK_EQUAL(CriticalSection::SUCCESS, status);

    int threadID_2 = 0xCAFE;
    status = m_pCS->Enter(threadID_2);
    CHECK_EQUAL(CriticalSection::SUCCESS, status);
}

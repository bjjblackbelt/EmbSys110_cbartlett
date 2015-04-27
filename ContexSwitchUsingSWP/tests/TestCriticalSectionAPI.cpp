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
    CHECK(true == true);
    CHECK_EQUAL(1,1);
}

#include <TestHarness.h>

TEST_GROUP(Template)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(Template, Initialize)
{
    CHECK(true == true);
    CHECK_EQUAL(1,1);
}

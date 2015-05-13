#include <TestHarness.h>
#include <stddef.h>
#include "OS.h"
#include "StubDUart.h"

#define F()      FAIL("OS Start here")

TEST_GROUP(OS)
{
    OS* p_os;
    StubDUart* p_stubUart;

    void setup()
    {
        p_stubUart = new StubDUart();
        p_os = new OS();
    }

    void teardown()
    {
        delete p_os;
        delete p_stubUart;
    }
};

TEST(OS, Initialize)
{
    CHECK(p_os != NULL);
}

TEST(OS, start)
{
    F();
}

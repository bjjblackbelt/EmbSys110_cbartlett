#include <TestHarness.h>
#include <stddef.h>
#include "StubDUart.h"
#include "OS.h"
#include "Threads.h"

#define F()      FAIL("OS Start here")

TEST_GROUP(OS)
{
    OS* p_os;
    StubDUart* p_stubUart;

    void setup()
    {
        p_stubUart = new StubDUart();
        p_os = new OS(*p_stubUart);
    }

    void teardown()
    {
        delete p_os;
        delete p_stubUart;
    }
};

TEST(OS, Initialize)
{
    CHECK(p_stubUart != NULL);
    CHECK(p_os != NULL);
}

TEST(OS, RegistrationFailsIfEntryFunctionNull)
{
    Thread::Thread_t th1 = {NULL, NULL, Thread::UID_THREAD_1, 0, Thread::STATE_INITIAL, "Thread1"};
    OS::Error_t status = p_os->RegisterThread(th1);
    CHECK_EQUAL(OS::ERROR_NULL, status);
}

#if 0
TEST(OS, start)
{
    F();
}
#endif

#include <TestHarness.h>
#include <stddef.h>
#include "StubDUart.h"
#include "DTimer.h"
#include "OS.h"
#include "Threads.h"

#define F()      FAIL("OS Start here")

namespace Bsp {
OS* g_pOS;
}

TEST_GROUP(OS)
{
    static const uint32_t dummySP = 0;
    OS* p_os;
    StubDUart* p_stubUart;
    DTimer* p_stubTimer;

    void setup()
    {
        p_stubUart = new StubDUart();
        p_stubTimer = new DTimer();
        p_os = new OS(*p_stubUart, *p_stubTimer);
        Bsp::g_pOS = p_os;
    }

    void teardown()
    {
        delete p_os;
        delete p_stubUart;
        delete p_stubTimer;
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

TEST(OS, RegistrationFailsIfAThreadHasAlreadyBeenCreatedWithTheUID)
{
    Thread::Thread_t th1 = {&Thread::Idle, NULL, Thread::UID_THREAD_1, 0, Thread::STATE_INITIAL, "Thread1"};
    p_os->RegisterThread(th1);

    Thread::Thread_t th2 = {&Thread::Idle, NULL, Thread::UID_THREAD_1, 0, Thread::STATE_INITIAL, "Thread2"};
    OS::Error_t status = p_os->RegisterThread(th2);
    CHECK_EQUAL(OS::ERROR_UID_NOT_UNIQUE, status);
}

TEST(OS, RegistrationFailsIfMaxNumberOfAllowedThreadsReached)
{
    Thread::Thread_t th1 = {&Thread::Idle, NULL, Thread::UID_THREAD_IDLE, 0, Thread::STATE_INITIAL, "Idle"};
    Thread::Thread_t th2 = {&Thread::Thread1, NULL, Thread::UID_THREAD_1, 0, Thread::STATE_INITIAL, "Thread1"};
    Thread::Thread_t th3 = {&Thread::Thread2, NULL, Thread::UID_THREAD_2, 0, Thread::STATE_INITIAL, "Thread2"};
    Thread::Thread_t th4 = {&Thread::Thread3, NULL, Thread::UID_THREAD_3, 0, Thread::STATE_INITIAL, "Thread3"};
    Thread::Thread_t th5 = {&Thread::Idle, NULL, (Thread::Uid_t)(0xFFFFFFFF), 0, Thread::STATE_INITIAL, "ExtraThread"};
    p_os->RegisterThread(th1);
    p_os->RegisterThread(th2);
    p_os->RegisterThread(th3);
    p_os->RegisterThread(th4);

    OS::Error_t status = p_os->RegisterThread(th5);
    CHECK_EQUAL(OS::ERROR_MAX_THREADS_REGISTERED, status);
}

#if 0
TEST(OS, start)
{
    F();
}
#endif

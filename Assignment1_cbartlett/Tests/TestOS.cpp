#include <TestHarness.h>
#include <stddef.h>
#include "StubDUart.h"
#include "OS.h"
#include "Threads.h"

#define F()      FAIL("OS Start here")

namespace Bsp {
OS* g_pOS;
}

TEST_GROUP(OS)
{
    OS* p_os;
    StubDUart* p_stubUart;

    void setup()
    {
        p_stubUart = new StubDUart();
        p_os = new OS(*p_stubUart);
        Bsp::g_pOS = p_os;
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

TEST(OS, SchedulerIncrementsOneThreadUponCall)
{
    Thread::Thread_t th1 = {&Thread::Idle, NULL, Thread::UID_THREAD_IDLE, 0, Thread::STATE_INITIAL, "Idle"};
    Thread::Thread_t th2 = {&Thread::Idle, NULL, Thread::UID_THREAD_1, 0, Thread::STATE_INITIAL, "Thread1"};
    p_os->RegisterThread(th1);
    p_os->RegisterThread(th2);

    OS::Error_t status = p_os->Scheduler();
    CHECK_EQUAL(OS::ERROR_NONE, status);
    CHECK_EQUAL(Thread::UID_THREAD_1, p_os->GetCurrentThreadID());

    status = p_os->Scheduler();
    CHECK_EQUAL(Thread::UID_THREAD_IDLE, p_os->GetCurrentThreadID());
}

TEST(OS, SchedulerFailsIfNoThreadsHaveBeenRegistered)
{
    OS::Error_t status = p_os->Scheduler();
    CHECK_EQUAL(OS::ERROR_NULL, status);
}


TEST(OS, SchedulerExecutesTheFirstNonNullThread)
{
    Thread::Thread_t th2 = {&Thread::Idle, NULL, Thread::UID_THREAD_1, 0, Thread::STATE_INITIAL, "Thread1"};
    p_os->RegisterThread(th2);

    OS::Error_t status = p_os->Scheduler();
    CHECK_EQUAL(OS::ERROR_NONE, status);
    CHECK_EQUAL(Thread::UID_THREAD_1, p_os->GetCurrentThreadID());
}

TEST(OS, SchedulerLoopsBackToIdleAfterReachingEndOfThreadQueue)
{
    Thread::Thread_t th1 = {&Thread::Idle, NULL, Thread::UID_THREAD_IDLE, 0, Thread::STATE_INITIAL, "Idle"};
    Thread::Thread_t th2 = {&Thread::Thread1, NULL, Thread::UID_THREAD_1, 0, Thread::STATE_INITIAL, "Thread1"};
    Thread::Thread_t th3 = {&Thread::Thread2, NULL, Thread::UID_THREAD_2, 0, Thread::STATE_INITIAL, "Thread2"};
    Thread::Thread_t th4 = {&Thread::Thread3, NULL, Thread::UID_THREAD_3, 0, Thread::STATE_INITIAL, "Thread3"};
    p_os->RegisterThread(th1); /* Idle */
    p_os->RegisterThread(th2); /* Thread 1 */
    p_os->RegisterThread(th3); /* Thread 2 */
    p_os->RegisterThread(th4); /* Thread 3 */

    p_os->Scheduler(); /* Thread 1 */
    p_os->Scheduler(); /* Thread 2 */
    p_os->Scheduler(); /* Thread 3 */

    p_os->Scheduler(); /* Idle */
    CHECK_EQUAL(Thread::UID_THREAD_IDLE, p_os->GetCurrentThreadID());
}

#if 0
TEST(OS, start)
{
    F();
}
#endif

/**
 *******************************************************************************
 *  @file   Threads.cpp
 *  @brief  Implementations for the threads of this application.
 *  @author Chad Bartlett   <cbartlet@uw.com>
 *******************************************************************************
 */
#include "Threads.h"

Thread::Error_t Thread::Idle(void* pData)
{
    (void)pData;
    return Thread::ERROR_NONE;
}

Thread::Error_t Thread::Thread1(void* pData)
{
    (void)pData;
    return Thread::ERROR_NONE;
}

Thread::Error_t Thread::Thread2(void* pData)
{
    (void)pData;
    return Thread::ERROR_NONE;
}

Thread::Error_t Thread::Thread3(void* pData)
{
    (void)pData;
    return Thread::ERROR_NONE;
}

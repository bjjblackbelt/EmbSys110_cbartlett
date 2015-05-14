/**
 *******************************************************************************
 *  @file   Threads.h
 *  @brief  Definitions for the threads of this application.
 *  @author Chad Bartlett   <cbartlet@uw.com>
 *******************************************************************************
 */
#ifndef THREADS_H
#define THREADS_H

#include <stdint.h>
#include "OS.h"

// Forward declarations
class CriticalSection;

namespace Thread {

/**
 * @enum State_t
 * @brief A thread can be in one of these states.
 */
typedef enum
{
    STATE_INITIAL,
    STATE_ACTIVE,
    STATE_READY,
    STATE_BLOCKED,
} State_t;

/**
 * @enum Uid_t
 * @brief Each thread must have a unique ID.
 *
 * There is some coupling here with the constant OS::MAX_THREAD_COUNT. The
 * fields in this enum are used as array accessors into an array defined in the
 * OS class. Therefore, the numbering must start at '0' and sequentially
 * progress until OS::MAX_THREAD_COUNT is reached.
 */
typedef enum
{
    UID_THREAD_IDLE,
    UID_THREAD_1,
    UID_THREAD_2,
    UID_THREAD_3,
} Uid_t;

/**
 * @enum Error_t
 * @brief Indications for potential errors of this namespace.
 */
typedef enum
{
    ERROR_NONE,         //!< Success
    ERROR_NULL_DATA,    //!< Data passed to thread is NULL
} Error_t;

/** A function pointer defining a thread. */
typedef Error_t (*ThreadEntryFunct)(void* pData);

/**
 * @struct GlobalData_t
 * @brief This structure defines shared data between threads.
 * @var GlobalData_t::guard
 * Memnber 'guard' provides a lock for the manipulation of the shared counter.
 * @var GlobalData_t::inc
 * Memnber 'inc' is a shared counter.
 * @var GlobalData_t::nT1
 * Memnber 'nT1' is the number of times thread 1 has been entered.
 * @var GlobalData_t::nT2
 * Memnber 'nT2' is the number of times thread 2 has been entered.
 */
struct GlobalDataStr
{
    CriticalSection guard;
    uint32_t inc;
    uint32_t nT1;
    uint32_t nT2;
};
typedef GlobalDataStr GlobalData_t;

/**
 * @struct Thread_t
 * @brief This structure defines a thread for this application.
 * @var Thread_t::entry
 * Member 'entry' is a pointer to the main function of this thread.
 * @var Thread_t::data
 * Memnber 'data' is an optional parameter to be passed to the entry function.
 * @var Thread_t::uid
 * Memnber 'uid' is the unique identifier for this thread.
 * @var Thread_t::state
 * Memnber 'state' is the current state of this thread.
 * @var Thread_t::name
 * Memnber 'name' is a string containing the name of this thread.
 */
struct ThreadStr
{
    static const uint8_t MAX_NAME_LENGTH = 12;
    Thread::ThreadEntryFunct entry;
    void* data;
    Thread::Uid_t uid;
    uint8_t priority;
    Thread::State_t state;
    char name[MAX_NAME_LENGTH + 1];
};
typedef ThreadStr Thread_t;

/**
 * This thread puts the application to sleep for 10-seconds and toggles a LED.
 * @param  pData An optional parameter.
 * @return      Returns Thread::ERROR_NONE if successful, else another error code.
 */
Thread::Error_t Idle(void* pData);

/**
 * This thread increments a shared counter.
 * @param  pData A pointer to a shared data structure.
 * @return      Returns Thread::ERROR_NONE if successful, else another error code.
 */
Thread::Error_t Thread1(void* pData);

/**
 * This thread decrements a shared counter.
 * @param  pData A pointer to a shared data structure.
 * @return      Returns Thread::ERROR_NONE if successful, else another error code.
 */
Thread::Error_t Thread2(void* pData);

/**
 * This thread monitors the shared counter.
 * @param  pData A pointer to a shared data structure.
 * @return      Returns Thread::ERROR_NONE if successful, else another error code.
 */
Thread::Error_t Thread3(void* pData);

} // namespace Thread
#endif // #ifndef THREADS_H


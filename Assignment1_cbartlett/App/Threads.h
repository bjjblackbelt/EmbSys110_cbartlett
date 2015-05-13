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

namespace Thread {
typedef void (*ThreadEntryFunct)(void* data);


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
 */
typedef enum
{
    UID_THREAD_IDLE,
    UID_THREAD_1,
    UID_THREAD_2,
    UID_THREAD_3,
} Uid_t;

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

} // namespace Thread
#endif // #ifndef THREADS_H


#pragma once

#include "kmessaged.h"

#include <linux/types.h>

struct kmessaged_dispatch_queue_t {
    unsigned long id;
    const char *qname;
};

/**
 * kmessaged_dispatch_queue_init
 *
 * Instantiates main dispatch queue.
 *
 * @warning This function should be only called once.
 */
KMESSAGED_EXPORT void kmessaged_dispatch_queue_init_main(void);

/**
 * kmessaged_dispatch_queue_create
 *
 * Creates a dispatch queue with given name.
 *
 * @param qptr Pointer to the copy of the dispatch queue struct created.
 * @param qname Name of the queue.
 * 
 * @return 0 if successful, otherwise ENOMEM indicating maximum number of queues is achieved.
 */
KMESSAGED_EXPORT int kmessaged_dispatch_queue_create(struct kmessaged_dispatch_queue_t *qptr, const char *qname);

/**
 * kmessaged_dispatch_get_main_queue
 *
 * Retrieves the main dispatch queue.
 *
 * @return Copy of the global dispatch queue struct.
 */
KMESSAGED_EXPORT struct kmessaged_dispatch_queue_t kmessaged_dispatch_get_main_queue(void);

/**
 * kmessaged_dispatch_msg
 *
 * Enqueues the message on given message queue.
 *
 * @param queue The dispatch queue message going to be enqueued.
 * @param msg A null-terminated message string.
 * @param uid A `uid_t` struct representing the user as a sender of the message.
 *
 * @return 0 if successful, otherwise ENOMEM indicating maximum number of queues is achieved.
 */
KMESSAGED_EXPORT int kmessaged_dispatch_msg(struct kmessaged_dispatch_queue_t queue, const char *msg, uid_t uid, const char *recipient);


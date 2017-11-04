#include "dispatch.h"

#include <linux/kernel.h>

#include "kmessage.h"

#define MIN(a,b) (a < b ? a : b);

//  A linked list node for holding 'kmessage's.
static struct _kmessaged_kmessage_node_t {
    struct kmessage_t data;
    struct _kmessaged_kmessage_node_t *next;
};

//  A dispatch queue for dispatching 'kmessage's.
static struct _kmessaged_dispatch_queue_opaque_t {
    unsigned long id;
    const char *qname;
    /* 4-bytes alignment boundary */
    struct _kmessaged_kmessage_node_t *data;
    struct _kmessaged_dispatch_queue_opaque_t *next;
    struct semaphore dsema;
};

//  Pointer to the first dispatch queue.
static _kmessaged_dispatch_queue_opaque_t *node = (void *)0x0;

static kmessaged_dispatch_queue_t convert_to_opaque_type(struct _kmessaged_dispatch_queue_opaque_t val)
{
    struct kmessaged_dispatch_queue_t opaque;

    memcpy(&opaque, &val, sizeof(struct kmessaged_dispatch_queue_t));

    return opaque;
}

static int dispatch_msg(struct _kmessaged_dispatch_queue_opaque_t *dqueue, const char *msg, uid_t uid)
{
    struct _kmessaged_kmessage_node_t *cur;
    int result;

    if (!dqueue->data) {
        dqueue->data = kmalloc(sizeof(struct _kmessaged_kmessage_node_t), GFP_KERNEL);
        result = kmessaged_msg_create(&dqueue->data->data, msg, uid);

        if (!dqueue->data) {
            return ENOMEM;
        }
    } else {
        for (cur = dqueue->data; cur->next; cur = cur->next)
            ;;

        cur->next = kmalloc(sizeof(struct _kmessaged_kmessage_node_t), GFP_KERNEL);
        cur = cur->next;

        result = kmessaged_msg_create(&cur->data->data, msg, uid);

        if (!cur->next) {
            return ENOMEM;
        }
    }
    
    printk(KERN_DEBUG "kmessaged: message \"%s\" sent by %d is successfully dispatched\n", msg, uid);

    return result;
}

void kmessaged_dispatch_queue_init_main()
{
    node = kmalloc(sizeof(struct _kmessaged_dispatch_queue_opaque_t), GFP_KERNEL);

    //  TODO: Should abort immediately.
    if (!node) {
        return;
    }

    node->id = 0;
    node->qname = kmalloc(sizeof(char) * strlen(KMESSAGED_MAIN_QUEUE_NAME), GFP_KERNEL);
    strcpy(node->qname, KMESSAGED_MAIN_QUEUE_NAME);
    node->data = 0x0;
    node->next = 0x0;
    sema_init(&node->dsema, 1);

    printk(KERN_DEBUG, "kmessaged: successfully initialized main dispatch queue\n");
}

int kmessaged_dispatch_queue_create(struct kmessaged_dispatch_queue_t *qptr, const char *qname)
{
    struct _kmessaged_dispatch_queue_opaque_t *cur;
    unsigned long count;

    //  Get to the last node.
    for (count = 0, cur = node; cur->next; count += 1, cur = cur->next)
        ;;

    cur->next = kmalloc(sizeof(_kmessaged_dispatch_queue_opaque_t));
    cur = cur->next;

    cur->id = count;
    cur->qname = kmalloc(sizeof(char) * MIN(strlen(qname), KMESSAGED_MAX_QUEUE_NAME_LEN), GFP_KERNEL);
    strncpy(cur->qname, qname, KMESSAGED_MAX_QUEUE_NAME_LEN);
    cur->data = 0x0;
    cur->next = 0x0;
    sema_init(&node->dsema, 1);

    printk(KERN_DEBUG, "kmessaged: successfully initialized queue #%d", cur->id);

    return 0;
}

kmessaged_dispatch_queue_t kmessaged_dispatch_get_main_queue()
{
    return convert_to_opaque_type(*node);
}

int kmessaged_dispatch_msg(struct kmessaged_dispatch_queue_t queue, const char *msg, uid_t uid)
{
    struct _kmessaged_dispatch_queue_opaque_t *cur;
    int result;

    for (cur = node; cur->next; cur = cur->next) {
        if (cur->id == queue.id && (strcmp(cur->qname, queue->qname) == 0)) {
            result = dispatch_msg(cur, msg, uid);

            return result;
        }
    }

    result = EINVAL;

    return result;
}

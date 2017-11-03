#include "query.h"

int kmessaged_delete_messages_with_query(kmessaged_query_type_t qtype, void *arg)
{
    if (!capable(CAP_SYS_ADMIN)) {
        return -EPERM;
    }

    //  do sth here...

    return 0;
}

#include "message_limit.h"

msglmt_t kmessaged_message_limit = KMESSAGED_DEFAULT_MESSAGE_LIMIT;

int kmessaged_set_message_limit(msglmt_t msglmt)
{
    //  Check for admin privileges
    if (!capable(CAP_SYS_ADMIN)) {
        return -EPERM;
    }

    kmessaged_message_limit = msglmt;

    return 0;
}

msglmt_t kmessaged_get_message_limit()
{
    return kmessaged_message_limit;
}


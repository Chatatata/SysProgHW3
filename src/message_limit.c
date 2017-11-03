#include "message_limit.h"

msglmt_t kmessaged_message_limit = KMESSAGED_DEFAULT_MESSAGE_LIMIT;

KMESSAGED_EXPORT int kmessaged_set_message_limit(msglmt_t msglmt)
{
    //  Check for admin privileges
    if (!capable(CAP_SYS_ADMIN)) {
        return -EPERM;
    }

    kmessaged_message_limit = msglmt;

    return 0;
}


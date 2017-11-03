#pragma once

#include "kmessaged.h"

typedef enum kmessaged_query_type_t {
    KMESSAGED_QUERY_NAME = 0,
    KMESSAGED_QUERY_CONTENT = 1,
} kmessaged_query_type_t;

KMESSAGED_EXPORT int kmessaged_delete_messages_with_query(kmessaged_query_type_t qtype, void *arg);


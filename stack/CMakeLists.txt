add_library(stack INTERFACE)

target_sources(stack
        INTERFACE FILE_SET HEADERS FILES
        "lock_free_stack.h"
)

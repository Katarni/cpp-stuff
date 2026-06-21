add_library(coroutines INTERFACE)

target_sources(coroutines
        INTERFACE FILE_SET HEADERS FILES
        "simple.h"
)

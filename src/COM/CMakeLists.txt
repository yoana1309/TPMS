set(This COM)

set(Headers
    COM.h
)

set(Sources
    COM.c
)

add_library(${This} STATIC ${Headers} ${Sources})
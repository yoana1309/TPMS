set(This OS)

set(Headers
    OS.h
)

set(Sources
    OS.c
)

add_library(${This} STATIC ${Headers} ${Sources})
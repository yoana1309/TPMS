set(This MCUWakeUp)

set(Headers
    MCUWakeUp.h
)

set(Sources
    MCUWakeUp.c
)

add_library(${This} STATIC ${Headers} ${Sources})
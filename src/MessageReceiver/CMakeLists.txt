set(This MessageReceiver)

set(Headers
    MessageReceiver.h
)

set(Sources
    MessageReceiver.c
)

add_library(${This} STATIC ${Headers} ${Sources})
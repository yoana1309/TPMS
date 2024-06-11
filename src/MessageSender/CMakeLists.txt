set(This MessageSender)

set(Headers
    MessageSender.h
)

set(Sources
    MessageSender.c
)

add_library(${This} STATIC ${Headers} ${Sources})
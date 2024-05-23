set(This SignalReceiver)

set(Headers
    SignalReceiver.h
)

set(Sources
    SignalReceiver.c
)

add_library(${This} STATIC ${Headers} ${Sources})
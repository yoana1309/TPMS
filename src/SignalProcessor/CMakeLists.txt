set(This SignalProcessor)

set(Headers
    SignalProcessor.h
)

set(Sources
    SignalProcessor.c
)

add_library(${This} STATIC ${Headers} ${Sources})
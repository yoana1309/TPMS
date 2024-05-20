set(This PressureMonitor)

set(Headers
    PressureMonitor.h
)

set(Sources
    PressureMonitor.c
)

add_library(${This} STATIC ${Headers} ${Sources})
set(This TPMSComponent)

set(Headers
    TPMS.h
)

set(Sources
    TPMS.c
)

add_library(${This} STATIC ${Headers} ${Sources})
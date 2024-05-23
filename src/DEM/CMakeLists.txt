set(This DEM)

set(Headers
    DEM.h
)

set(Sources
    DEM.c
)

add_library(${This} STATIC ${Headers} ${Sources})
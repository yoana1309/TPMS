#ifndef TYPES_H
#define TYPES_H

#ifndef TRUE
    #define TRUE      1
#endif
#ifndef FALSE
    #define FALSE     0
#endif

typedef unsigned char boolean;

/* Data types that have exact the specified width */
typedef signed char         sint8;        //        -128 .. +127        
typedef unsigned char       uint8;        //           0 .. 255         
typedef signed short        sint16;       //      -32768 .. +32767      
typedef unsigned short      uint16;       //           0 .. 65535       
typedef signed long         sint32;       // -2147483648 .. +2147483647 
typedef unsigned long       uint32;       //           0 .. 4294967295  

#define E_OK            (0u)
#define E_NOT_OK        (1u)

typedef uint8 returnType;

#define NULL_PTR  ((void *)0)

#endif /* TYPES_H */
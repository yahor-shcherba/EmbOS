#ifndef _STDINT_H
#define _STDINT_H

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;

typedef signed char         int8_t;
typedef signed short        int16_t;
typedef signed int          int32_t;
typedef signed long long    int64_t;

_Static_assert(sizeof(uint8_t)  == 1, "uint8_t must be 1 byte on this platform");
_Static_assert(sizeof(uint16_t) == 2, "uint16_t must be 2 byte on this platform");
_Static_assert(sizeof(uint32_t) == 4, "uint32_t must be 4 byte on this platform");
_Static_assert(sizeof(uint64_t) == 8, "uint64_t must be 8 byte on this platform");
_Static_assert(sizeof(int8_t)  == 1, "int8_t must be 1 byte on this platform");
_Static_assert(sizeof(int16_t) == 2, "int16_t must be 2 byte on this platform");
_Static_assert(sizeof(int32_t) == 4, "int32_t must be 4 byte on this platform");
_Static_assert(sizeof(int64_t) == 8, "int64_t must be 8 byte on this platform");

#endif /* NOT _STDINT_H */

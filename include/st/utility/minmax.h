#ifndef ST_UTILITY_MINMAX_H
#define ST_UTILITY_MINMAX_H

#define st_min(a, b) ({            \
    const __typeof__(a) __a = (a); \
    const __typeof__(b) __b = (b); \
    __a < __b ? __a : __b; })

#define st_max(a, b) ({            \
    const __typeof__(a) __a = (a); \
    const __typeof__(b) __b = (b); \
    __a > __b ? __a : __b; })

#endif // ST_UTILITY_MINMAX_H

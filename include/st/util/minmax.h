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

#define st_minmax(a, v, b) ({      \
    const __typeof__(a) ___a = (a); \
    const __typeof__(v) ___v = (v); \
    const __typeof__(b) ___b = (b); \
    st_max(___a, st_min(___v, ___b)); })

#endif // ST_UTILITY_MINMAX_H

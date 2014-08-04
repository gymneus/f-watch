#include <stdarg.h>

extern int pp_sprintf(char *s, const char *fmt, ...)
        __attribute__((format(printf,2,3)));

extern int pp_vsprintf(char *buf, const char *, va_list)
        __attribute__ ((format (printf, 2, 0)));


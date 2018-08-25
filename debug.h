#ifdef DEBUG
#define debug(fmt, ...) fprintf(stderr, "%s:" fmt, __func__, ##__VA_ARGS__)
#else
#define debug(fmt, ...)
#endif


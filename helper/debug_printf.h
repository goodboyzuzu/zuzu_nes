/* #define DEBUG */

#ifndef DEBUG_PRINTF_H
#define DEBUG_PRINTF_H

#ifdef DEBUG
    #define debug_printf printf
#else
    void dummy_printf(const char * fmt, ...);
    #define debug_printf dummy_printf
#endif

#endif /* DEBUG_printf */
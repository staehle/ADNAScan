/**
 * @file color.c
 * @author Jake Staehle
 * @brief Prints pretty messages
 *
 * adna - the Asynchronous-process DNA fastq checker and trimmer
 *  ~ an Awesome DNA utility
 *
 */

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "color.h"

/**
 * @brief printf - taste the rainbow! (note: adds newline)
 * @param procname process name
 * @param color from the color_code enum
 * @param format printf format
 * @param ... printf format args
 *
 * Just like the regular printf, except now I'm gonna shove pretty colors down your throat and you're gonna like it!
 * This is also a thread-safe print function, so that's useful.
 */
int printc(const char *procname, color_code color, const char *format, ...) {
    pthread_mutex_lock(&printc_mutex);
    va_list arg;
    int done;
    size_t s;
    size_t len = strlen(procname);
    for (s=0; s < RPT_MAX-len; s++) {
        printf(" ");
    }
    printf(M_BOLD RPT_FMT M_CLEAR, procname);
    switch(color) {
#ifndef IM_BORING
    case MASTER: printf(M_BOLD B_BLACK C_CYAN);
        break;
    case START: printf(M_BOLD B_BLACK C_GREEN);
        break;
    case ERROR: printf(M_BOLD B_RED C_YELLOW);
        break;
    case INFO: printf(M_BOLD B_BLACK C_YELLOW);
        break;
    case DEBUG:
#ifdef PRINT_DEBUG
        printf(M_BOLD B_BLACK C_CYAN);
        break;
#else
        return 0;
#endif
    case BOLD: printf(M_BOLD);
        break;
    case RED: printf(M_BOLD C_RED);
        break;
    case ORANGE: printf(C_YELLOW);
        break;
    case BLUE: printf(M_BOLD C_BLUE);
        break;
    case PINK: printf(M_BOLD C_PINK);
        break;
    case CYAN: printf(C_CYAN);
        break;
    case PLAIN:
#endif
    default: printf(M_CLEAR); 
    }
    va_start(arg, format);
    done = vfprintf(stdout, format, arg);
    va_end(arg);
    printf(M_CLEAR"\n");
    fflush(stdout);
    pthread_mutex_unlock(&printc_mutex);
    return done;
}

int printa(const char *format, ...) {
    va_list arg;
    int done;
#ifndef IM_BORING
    printf(M_BOLD B_BLACK C_CYAN);
#endif
    va_start(arg, format);
    done = vfprintf(stdout, format, arg);
    va_end(arg);
    printf(M_CLEAR"\n");
    return done;
}

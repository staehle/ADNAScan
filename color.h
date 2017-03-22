#ifndef COLOR_H
#define COLOR_H

//If the colors actually bother you that much, uncomment this:
//#define IM_BORING

#define P_START printc(procname, START, "Reporting for duty!")

#define RPT_FMT "[ %s ] : "
#define RPT_MAX 24

#define C_RED "\033[31m"
#define C_GREEN "\033[32m"
#define C_YELLOW "\033[33m"
#define C_BLUE "\033[34m"
#define C_PINK "\033[35m"
#define C_CYAN "\033[36m"
#define C_WHITE "\033[37m"
#define B_BLACK "\033[40m"
#define B_RED "\033[41m"
#define M_BOLD "\033[1m"
#define M_CLEAR "\033[0m"

pthread_mutex_t printc_mutex;

typedef enum {
    PLAIN = 0,
    BOLD,
    MASTER,
    START,
    ERROR,
    INFO,
    DEBUG,
    RED,
    ORANGE,
    BLUE,
    PINK,
    CYAN,
} color_code;

int printc(const char *procname, color_code color, const char *format, ...);
int printa(const char *format, ...);

#endif

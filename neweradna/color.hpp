#ifndef COLOR_H
#define COLOR_H

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

#define P_ERROR(text) M_BOLD << B_RED << C_YELLOW << text << M_CLEAR
#define P_ADNA(text) M_BOLD << B_BLACK << C_CYAN << text << M_CLEAR
#define P_START(text) M_BOLD << B_BLACK << C_GREEN << text << M_CLEAR
#define P_INFO(text) M_BOLD << B_BLACK << C_YELLOW << text << M_CLEAR
#define P_DEBUG(text) M_BOLD << C_PINK << text << M_CLEAR
#define P_EXIT(text) M_BOLD << B_BLACK << C_CYAN << text << M_CLEAR

#define P_RED(text) M_BOLD << C_RED << text << M_CLEAR
#define P_ORANGE(text) C_YELLOW << text << M_CLEAR
#define P_BLUE(text) M_BOLD << C_BLUE << text << M_CLEAR
#define P_PINK(text) M_BOLD << C_PINK << text << M_CLEAR
#define P_CYAN(text) C_CYAN << text << M_CLEAR
#define P_BOLD(text) M_BOLD << text << M_CLEAR

#endif


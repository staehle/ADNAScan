/**
 * @file StringTinder.c
 * @author Jake Staehle
 * @brief The second _RomanEmpire_ thread is StringTinder, which takes in the two hordes, and creates matches from individual nomad IDs and sends them back to the empire. 
 *
 * adna - the Asynchronous-process DNA fastq checker and trimmer
 *  ~ an Awesome DNA utility
 *
 */

#include <ctype.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adna.h"
#include "color.h"

void *StringTinder(void *vargp) {
    adna_ret ret = OK;
    char procname[] = "StringTinder";
    
    P_START;
    
    die(procname, ret);
    return vargp;
}

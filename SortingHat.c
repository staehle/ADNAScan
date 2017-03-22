/**
 * @file SortingHat.c
 * @author Jake Staehle
 * @brief The third _RomanEmpire_ task is SortingHat, which takes in a bulletclip, and sends individual matches to each Legion process as they report OK. This is not a threaded function.
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

adna_ret SortingHat(bulletclip *clip) {
    adna_ret ret = OK;
    char procname[] = "SortingHat";

    if (clip == NULL) return ERR_INTERNAL;
    
    P_START;
    
    return die(procname, ret);
}

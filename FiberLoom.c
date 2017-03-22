/**
 * @file FiberLoom.c
 * @author Jake Staehle
 * @brief The first _RomanEmpire_ thread is FiberLoom, which scans a single fastq file, and creates a Nomad struct for each read and attaches it to RomanEmpire's main horde struct list. 
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

char *fq_stage_tostring(fastq_stage stage) {
    switch(stage) {
    case ID: return "ID";
    case DNA: return "DNA";
    case COMMENT: return "COMMENT";
    case QUAL: return "QUAL";
    default: return "UNK";
    }
}

void *FiberLoom(void *vargp) {
    fl_args *args = vargp;
    char procname[MAX_STR_LEN];
    char line[MAX_STR_LEN];
    char *cptr;
    FILE *fp;
    horde *work;
    fastq_stage stage = ID;
    size_t lineno = 0;
    size_t nomadct = 0;

    sprintf(procname, "Fiber Loom %d", args->id);
    P_START;

    fp = fopen(args->fn, "r");
    if (fp == NULL) {
        args->ret = ERR_FILE_NOT_FOUND;
        printc(procname, ERROR, "file handle NULL for '%s'", args->fn);
        return NULL;
    }

    // create head for horde
    args->h = calloc(1, sizeof(horde));
    if (args->h == NULL) {
        args->ret = ERR_INTERNAL;
        printc(procname, ERROR, "can't calloc a horde?");
        return NULL;
    }
    args->h->cur = calloc(1, sizeof(nomad));
    if (args->h->cur == NULL) {
        args->ret = ERR_INTERNAL;
        printc(procname, ERROR, "can't calloc a horde->nomad?");
        return NULL;
    }
    work = args->h;
    while (fgets(line, sizeof(line), fp) != NULL) {
        lineno++; // 1-based line numbers
        if (line[0] == '\n') {
            printc(procname, ERROR, "Warning: line %u is BLANK! stage=%s",
                   lineno, fq_stage_tostring(stage));
        }
        //printc(procname, INFO, "On stage: %s", fq_stage_tostring(stage));

        switch(stage) {
        case ID:
            // need to truncate to common segment, either a ' '(space) or a '#'
            cptr = strchr(line, '#');
            if (cptr == NULL) {
                cptr = strchr(line, ' ');
                if (cptr == NULL) {
                    args->ret = ERR_GENERAL;
                    printc(procname, ERROR, "ID lineno %u: Can't find common portion in '%s'",
                           lineno, line);
                    return NULL;
                }
            }
            *cptr = '\0'; // truncate to found spot
            strcpy(work->cur->id, line);
            break;
        case DNA:
            strcpy(work->cur->string, line);
            break;
        case COMMENT:
            strcpy(work->cur->comment, line);
            break;
        case QUAL:
            strcpy(work->cur->score, line);
            break;
        }

        if (stage < QUAL) {
            stage++;
        } else {
            work->next = calloc(1, sizeof(horde));
            if (work->next == NULL) {
                args->ret = ERR_INTERNAL;
                printc(procname, ERROR, "can't calloc a next->horde?");
                return NULL;
            }
            work->next->cur = calloc(1, sizeof(nomad));
            if (work->next->cur == NULL) {
                args->ret = ERR_INTERNAL;
                printc(procname, ERROR, "can't calloc a next->horde->nomad?");
                return NULL;
            }
            printc(procname, INFO, "Created nomad %u with id '%s'", nomadct, work->cur->id);
            stage = ID;
            args->ret = READY;
            nomadct++;
            work = work->next; // move to next horde slot
        }
    }
    printc(procname, INFO, "Reached end of file! Created %u nomads!", nomadct);
    
    if (stage == ID) {        
        die(procname, OK);
        args->ret = OK;

    } else { // exited file loop without finishing last nomad
        printc(procname, ERROR, "uh, exited file in stage %s, should be zero",
               fq_stage_tostring(stage));
        args->ret = ERR_INTERNAL;
    }
    return NULL;
}

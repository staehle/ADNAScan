/**
 * @file RomanEmpire.c
 * @author Jake Staehle
 * @brief The first _adna_ process, RomanEmpire, scans both fastq reads, matches them, and sorts them to Legion processes
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

void print_help(const char *progpath) {
    fprintf(stderr,
            "Usage:\n    %s"
            " <read1.fastq>"
            " <read2.fastq>"
            " <adapter_list.fasta>"
            " <number_of_legion_processes>"
            " [output_dir]\n"
            "    ('output_dir' will default to '"DEF_ODP"')\n",
            progpath);
}


adna_ret die(const char *procname, adna_ret errorcode) {
    switch(errorcode) {
    case OK:
        printc(procname, MASTER, "Reporting job done! See ya next time!");
        break;
    case HELP:
        print_help(procname);
        break;
    case PREINIT:
        printc(procname, INFO, "Exiting in a pre-init state?");
        break;
    case READY:
        printc(procname, INFO, "Exiting in a ready state?");
        break;
    case ERR_GENERAL:
        printc(procname, ERROR, "Exiting with 'general' error");
        break;
    case ERR_INTERNAL:
        printc(procname, ERROR, "Exiting with an internal error");
        break;
    case ERR_INVALID_ARGS:
        printc(procname, ERROR, "Exiting because of invalid arguments");
        break;
    case ERR_FILE_NOT_FOUND:
        printc(procname, ERROR, "File not found!");
        break;
    }
    return errorcode;
}


/**
 * @brief Roman Empire (Main)
 * @param argv[1] read1.fastq
 * @param argv[2] read2.fastq
 * @param argv[3] adapter_list.fasta
 * @param argv[4] number of Legion processes
 * @param argv[5] (optional) output directory location
 */
int main(int argc, char **argv) {
    adna_ret ret = OK;
    short unsigned int num_proc = 0;
    char procname[] = "Roman Empire";
    char fap[MAX_STR_LEN], odp[MAX_STR_LEN];
    pthread_t fiberloom1, fiberloom2; //, cupid;
    fl_args fla1, fla2;
    //bulletclip *clip;
    //bool *switchboard;

    printa(ADNA);
    printa(COPYRIGHT);
    printf("\n");
    
    if (argc < 5) {
        return die(argv[0], HELP);
    }

    fla1.ret = PREINIT;
    fla1.id = 1;
    fla2.ret = PREINIT;
    fla2.id = 2;

    realpath(argv[1], fla1.fn);
    realpath(argv[2], fla2.fn);
    realpath(argv[3], fap);
    if (argc < 6) {
        realpath(DEF_ODP, odp);
    } else {
        realpath(argv[5], odp);
    }

    num_proc = atoi(argv[4]);
    if (num_proc < 1 || num_proc > MAX_PROCS) {
        printc(procname, ERROR,
               "Error: %u number of processes is outside of acceptable range of 1-%u",
               num_proc, MAX_PROCS);
        return die(procname, ERR_INVALID_ARGS);
    }

    printc(procname, INFO, "adna initialized with %u processes", num_proc);
    printc(procname, INFO, "   fastq read 1 path: %s", fla1.fn);
    printc(procname, INFO, "   fastq read 2 path: %s", fla2.fn);
    printc(procname, INFO, " fasta adapters path: %s", fap);
    printc(procname, INFO, " results output path: %s", odp);

    P_START;

    // initialize two fiber looms, send horde1 or 2
    pthread_create(&fiberloom1, NULL, FiberLoom, &fla1);
    pthread_create(&fiberloom2, NULL, FiberLoom, &fla2);

    // wait for READY signal from both
    printc(procname, INFO, "Waiting for Fiber Looms to report READY");
    while (fla1.ret < READY || fla2.ret < READY);    
    if (fla1.ret != READY) return die("Fiber Loom 1", fla1.ret);
    if (fla2.ret != READY) return die("Fiber Loom 2", fla2.ret);

    // check that FASTA file exists here, don't want to get too far ahead
    // TODO

    printc(procname, INFO, "Fiber Looms report READY, continuing");
    

    // initialize stringtinder, pass in horde1 and 2, pass in match list
    // wait for ok signal

    // execvp Legion processes * num_proc
    // ...

    // run sortinghat function, pass in matchlist and switchboard

    pthread_join(fiberloom1, NULL);
    pthread_join(fiberloom2, NULL);
    return die(procname, ret);
}


// horde *horde1, *h1temp;
// nomad *person;
// 
// person = calloc(1, sizeof(nomad));
// strcpy(person->id, "Marcus");
// strcpy(person->string, "ASDFASDF");
// strcpy(person->score, "12345678");
// strcpy(person->comment, "hello");
// 
// horde1 = calloc(1, sizeof(horde));
// horde1->cur = person;
// horde1->next = calloc(1, sizeof(horde));
// 
// person = NULL;
// person = calloc(1, sizeof(nomad));
// strcpy(person->id, "Bobbeth");
// strcpy(person->string, "ABCDEFGH");
// strcpy(person->score, "01234567");
// strcpy(person->comment, "howdy");
// 
// horde1->next->cur = person;
// 
// person = NULL;
// 
// printf("horde roster:\n");
// while(horde1 != NULL) {
//     printf("  name: %s\n", horde1->cur->id);
//     printf("  stng: %s\n", horde1->cur->string);
//     printf("  scor: %s\n", horde1->cur->score);
//     printf("  cmnt: %s\n", horde1->cur->comment);
//     h1temp = horde1;
//     horde1 = horde1->next;
//     free(h1temp);
// 
//     printf("\nnext:\n");
// }

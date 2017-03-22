/**
 * @file adna.h
 * @author Jake Staehle
 * @brief Definitions for commonly used objects
 *
 * adna - the Asynchronous-process DNA fastq checker and trimmer
 *  ~ an Awesome DNA utility
 *
 */

#ifndef ADNA_H
#define ADNA_H

#define ADNA "adna - the Asynchronous-process DNA fastq checker and trimmer"
#define COPYRIGHT "(C) 2017 Jake Staehle <jacob@staehle.us>"

#define DEF_ODP "./adna_output/"

#define PRINT_DEBUG

#define MAX_PROCS 8
#define MAX_STR_LEN 256

/*
 * Utilities
 */
/**
 * @brief standard status return enum
 */ 
typedef enum {
    HELP,
    PREINIT,
    READY,
    ERR_GENERAL,
    ERR_INTERNAL,
    ERR_INVALID_ARGS,
    ERR_FILE_NOT_FOUND,
    OK
} adna_ret;

/**
 * @brief enum for fastq reading stages
 */ 
typedef enum {
    ID,
    DNA,
    COMMENT,
    QUAL
} fastq_stage;

/*
 * Structs for adna Data Types
 */
/**
 * @brief a _nomad_ is a single FASTQ read
 */
typedef struct {
    char id[MAX_STR_LEN];
    char string[MAX_STR_LEN];
    char score[MAX_STR_LEN];
    char comment[MAX_STR_LEN];
} nomad;

/**
 * @brief a _horde_ is a list of _nomads_ from a single FASTQ file
 */
struct _horde;
typedef struct _horde {
    nomad *cur;
    struct _horde *next;
} horde;

/**
 * @brief a _match_ is a pair of _nomads_ which have matching IDs across reads
 */ 
typedef struct {
    nomad *one;
    nomad *two;
} match;

/**
 * @brief a _bulletclip_ is a list of _matches_ which are distributed to an open Legion process
 */
struct _bulletclip;
typedef struct _bulletclip {
    match *bullet;
    struct _bulletclip *next;
} bulletclip;

/*
 * Structs for Thread Function Arg Lists
 */
typedef struct {
    adna_ret ret;
    int id;
    char fn[MAX_STR_LEN];
    horde *h;
} fl_args;

/*
 * Function/Thread Declarations
 */
adna_ret die(const char *procname, adna_ret errorcode);
void *FiberLoom(void *vargp);
void *StringTinder(void *vargp);
adna_ret SortingHat(bulletclip *clip);

#endif

/*
 *  SKK-like Kana-Kanji translation library
 *
 * by A.ITO November, 1991
 */

#ifndef SKKLIB_H
#define SKKLIB_H

#ifndef NO_MALLOC_H
#include <malloc.h>
#endif

/*
 * Structure for Dictionary
 */
typedef struct DicList {
	struct CandList *cand;
	struct DicList *nextitem;
	char kanaword[1];
} *DicList;

/*
 * Structure for Candidate
 */
typedef struct CandList {
	struct CandList *okuri;
	struct CandList *nextcand;
	struct CandList *prevcand;
	struct DicList *dicitem;
	char candword[1];
} *CandList;

typedef struct Hash {
	DicList h_index;
	short length;
	struct Hash *next;
} *Hash;

#define HASHSIZE 256

typedef struct Dictionary {
	DicList dlist;
	DicList okuriAriFirst;
	DicList okuriNasiFirst;
	Hash *dhash;
	time_t mtime;
} *Dictionary;

#define _NEW(type) ((type)malloc(sizeof(struct type)))
#define _NEW2(type,add)	((type)malloc(sizeof(struct type)+(add)))

Dictionary openSKK();
CandList getCand(),getCandList();
CandList deleteCand(),firstCand();
CandList searchOkuri();
DicList addNewItem();

/* flags for printCand() */
#define NOFREE_CAND 0
#define FREE_CAND 1

#endif /* SKKLIB_H */

/*
 *  SKK-like Kana-Kanji translation library
 *
 * by A.ITO November, 1991
 */
#pragma once
#include <malloc.h>
#include <string>
#include <time.h>

/*
 * Structure for Dictionary
 */
struct DicList
{
  struct CandList* cand;
  struct DicList* nextitem;
  char kanaword[1];
};

/*
 * Structure for Candidate
 */
struct CandList
{
  CandList* okuri;
  CandList* nextcand;
  CandList* prevcand;
  DicList* dicitem;
  char candword[1];
};

struct Hash
{
  DicList* h_index;
  short length;
  Hash* next;
};

#define HASHSIZE 256

struct Dictionary
{
  std::string m_path;

  DicList* dlist;

  DicList* okuriAriFirst;
  DicList* okuriNasiFirst;
  Hash** dhash;
  time_t mtime;

  Dictionary(std::string_view path);
  ~Dictionary();
};

int
isConjugate(char word[], int l);

DicList*
addNewItem(Dictionary dic, char* word, CandList clist);

CandList*
getCandList(FILE* f, DicList* ditem, int okuri);

void
mergeDictionary(Dictionary* dic, const char* dicname);

/* flags for printCand() */
#define NOFREE_CAND 0
#define FREE_CAND 1
void
printCand(CandList* cl, FILE* f, int fre);

int
hashVal(const char* s);

void
addHash(Hash** hash, DicList* ditem);

CandList*
getCand(Dictionary* dic, char* s);

void
selectCand(CandList** first, CandList* cand);

void
freeCand(CandList* cl);

CandList*
deleteCand(CandList* frlist, CandList* itlist);

CandList*
firstCand(CandList* l);

CandList*
searchOkuri(CandList* cl, char* okuri, CandList*** newfirst);

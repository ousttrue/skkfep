/*
 *  SKK-like Kana-Kanji translation library
 *
 * by A.ITO November, 1991
 */
#pragma once
#include <malloc.h>
#include <string>
#include <time.h>
#include <tuple>

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

std::tuple<CandList**, CandList*>
searchOkuri(CandList* cl, const char* okuri);

CandList*
getCandList(FILE* f, DicList* ditem, int okuri);

CandList*
deleteCand(CandList* frlist, CandList* itlist);

/* flags for printCand() */
enum PrintCandTypes
{
  NOFREE_CAND,
  FREE_CAND,
};

void
printCand(CandList* cl, FILE* f, PrintCandTypes fre);


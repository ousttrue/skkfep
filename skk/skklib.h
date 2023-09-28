/*
 *  SKK-like Kana-Kanji translation library
 *
 * by A.ITO November, 1991
 */
#pragma once
#include <string>
#include <time.h>
#include <tuple>

struct DicList
{
  struct CandList* cand = nullptr;
  struct DicList* nextitem = nullptr;
  std::string kanaword;

  DicList(const char* word)
    : kanaword(word)
  {
  }
};

enum class PrintCandTypes
{
  NOFREE_CAND,
  FREE_CAND,
};

struct CandList
{
  CandList* okuri = nullptr;
  CandList* nextcand = nullptr;
  CandList* prevcand = nullptr;
  DicList* dicitem = nullptr;
  std::string candword;

private:
  CandList(const char* word)
    : candword(word)
  {
  }

public:
  static CandList* getCandList(FILE* f, DicList* ditem, int okuri);

  void print(FILE* f, PrintCandTypes fre);
};

std::tuple<CandList**, CandList*>
searchOkuri(CandList* cl, const char* okuri);

CandList*
deleteCand(CandList* frlist, CandList* itlist);

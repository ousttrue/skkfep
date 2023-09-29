/*
 *  SKK-like Kana-Kanji translation library
 *
 * by A.ITO November, 1991
 */
#pragma once
#include <string>
#include <time.h>
#include <tuple>

// 見出し
struct DicList
{
  struct CandList* cand = nullptr;
  struct DicList* nextitem = nullptr;
  std::string kanaword;

  DicList(std::string_view word)
    : kanaword(word)
  {
  }
};

enum class PrintCandTypes
{
  NOFREE_CAND,
  FREE_CAND,
};

// 変換候補
struct CandList
{
  CandList* nextcand = nullptr;
  CandList* prevcand = nullptr;
  DicList* dicitem = nullptr;
  std::string candword;

private:
  CandList(std::string_view word)
    : candword(word)
  {
  }

public:
  static CandList* getCandList(std::string_view buf,
                               DicList* ditem,
                               bool okuri);

  void print(FILE* f, PrintCandTypes fre);
};

CandList*
deleteCand(CandList* frlist, CandList* itlist);

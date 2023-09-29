/*
 *  SKK-like Kana-Kanji translation library
 *
 * by A.ITO November, 1991
 */

#include "skklib.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

CandList*
CandList::getCandList(std::string_view line, DicList* ditem, bool okuri)
{
  if (line.empty()) {
    assert(false);
    return {};
  }
  if (line[0] != '/') {
    assert(false);
    return {};
  }
  if (line.back() == '\n') {
    line = line.substr(0, line.size() - 1);
  }
  line = line.substr(1);

  CandList* citem0 = nullptr;
  CandList* citem2 = nullptr;
  while (line.size() > 0) {
    auto pos = line.find('/');
    std::string_view buf;
    if (pos == std::string_view::npos) {
      // no tail '/'
      buf = line;
      line = {};
    } else {
      buf = line.substr(0, pos);
      line = line.substr(pos + 1);
    }

    auto citem = new CandList(buf);

    if (okuri && buf[0] == '[') {
      assert(false);
      //   for (p = buf; (*p = fgetc(f)) != '/'; p++)
      //     ;
      //   *p = '\0';
      //   citem = new CandList(buf);
      //   citem->prevcand = citem2;
      //   citem->dicitem = ditem;
      //   ccitem2 = citem;
      //   for (;;) {
      //     if ((c = fgetc(f)) == ']')
      //       break;
      //     for (buf[0] = c, p = buf + 1; (*p = fgetc(f)) != '/'; p++)
      //       ;
      //     *p = '\0';
      //     ccitem = new CandList(buf);
      //     if (ccitem2 == citem) {
      //       ccitem2->okuri = ccitem;
      //       ccitem->prevcand = NULL;
      //     } else {
      //       ccitem2->nextcand = ccitem;
      //       ccitem->prevcand = ccitem2;
      //     }
      //     ccitem2 = ccitem;
      //   }
    } else {
      citem = new CandList(buf);
      citem->prevcand = citem2;
      citem->dicitem = ditem;
    }

    if (citem2) {
      citem2->nextcand = citem;
    } else {
      citem0 = citem;
    }
    citem2 = citem;
  }
  return citem0;
}

void
CandList::print(FILE* f, PrintCandTypes fre)
{
  CandList* clist2;
  fputc('/', f);
  for (auto clist = this; clist != NULL; clist2 = clist,
            clist = clist->nextcand,
            (fre == PrintCandTypes::FREE_CAND ? (delete (clist2), 0) : 0)) {
    fprintf(f, "%s/", clist->candword.c_str());
  }
  fputc('\n', f);
}

static void
freeCand(CandList* cl)
{
  CandList* clist2;
  for (auto clist = cl; clist != NULL;
       clist2 = clist, clist = clist->nextcand, delete clist2) {
  }
}

CandList*
deleteCand(CandList* frlist, CandList* itlist)
{
  CandList* l;
  while (itlist != NULL) {
    for (l = frlist; l != NULL; l = l->nextcand) {
      if (itlist->candword == l->candword) {
        if (l->prevcand == NULL) {
          frlist = l->nextcand;
          if (l->nextcand)
            l->nextcand->prevcand = NULL;
        } else {
          l->prevcand->nextcand = l->nextcand;
          if (l->nextcand)
            l->nextcand->prevcand = l->prevcand;
        }
        l->nextcand = NULL;
        freeCand(l);
        break;
      }
    }
    itlist = itlist->nextcand;
  }
  return frlist;
}

CandList*
firstCand(CandList* l)
{
  while (l->prevcand)
    l = l->prevcand;
  return l;
}

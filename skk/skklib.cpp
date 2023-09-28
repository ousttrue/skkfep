/*
 *  SKK-like Kana-Kanji translation library
 *
 * by A.ITO November, 1991
 */

#include "skklib.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

CandList*
CandList::getCandList(FILE* f, DicList* ditem, int okuri)
{
  char buf[256];
  CandList* citem = nullptr;
  CandList* citem2 = nullptr;
  CandList* citem0 = NULL;
  CandList* ccitem;
  CandList* ccitem2;
  char c, *p;

  citem2 = NULL;
  while ((c = fgetc(f)) != '\n' && !feof(f)) {
    if (c == '/')
      continue;
    if (okuri && c == '[') {
      for (p = buf; (*p = fgetc(f)) != '/'; p++)
        ;
      *p = '\0';
      citem = new CandList(buf);
      citem->prevcand = citem2;
      citem->dicitem = ditem;
      ccitem2 = citem;
      for (;;) {
        if ((c = fgetc(f)) == ']')
          break;
        for (buf[0] = c, p = buf + 1; (*p = fgetc(f)) != '/'; p++)
          ;
        *p = '\0';
        ccitem = new CandList(buf);
        if (ccitem2 == citem) {
          ccitem2->okuri = ccitem;
          ccitem->prevcand = NULL;
        } else {
          ccitem2->nextcand = ccitem;
          ccitem->prevcand = ccitem2;
        }
        ccitem2 = ccitem;
      }
    } else {
      for (buf[0] = c, p = buf + 1; (*p = fgetc(f)) != '/'; p++)
        ;
      *p = '\0';
      citem = new CandList(buf);
      citem->prevcand = citem2;
      citem->dicitem = ditem;
    }
    if (citem2)
      citem2->nextcand = citem;
    else
      citem0 = citem;
    citem2 = citem;
  }
  return citem0;
}

void
CandList::print(FILE* f, PrintCandTypes fre)
{
  CandList* clist;
  CandList* clist2;
  CandList* cclist;
  CandList* cclist2;

  fputc('/', f);
  for (clist = this; clist != NULL; clist2 = clist,
      clist = clist->nextcand,
      (fre == PrintCandTypes::FREE_CAND ? (delete (clist2), 0) : 0)) {
    if (clist->okuri) {
      fprintf(f, "[%s/", clist->candword.c_str());
      for (cclist = clist->okuri; cclist != NULL; cclist2 = cclist,
          cclist = cclist->nextcand,
          (fre == PrintCandTypes::FREE_CAND ? (delete (cclist2), 0) : 0)) {
        fprintf(f, "%s/", cclist->candword.c_str());
      }
      fputs("]/", f);
    } else
      fprintf(f, "%s/", clist->candword.c_str());
  }
  fputc('\n', f);
}

static void
freeCand(CandList* cl)
{
  CandList* clist2;
  CandList* cclist2;

  for (auto clist = cl; clist != NULL;
       clist2 = clist, clist = clist->nextcand, delete clist2) {
    if (clist->okuri) {
      for (auto cclist = clist->okuri; cclist != NULL;
           cclist2 = cclist, cclist = cclist->nextcand, delete cclist2)
        ;
    }
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

std::tuple<CandList**, CandList*>
searchOkuri(CandList* cl, const char* okuri)
{
  CandList** newfirst = nullptr;
  for (auto ll = cl; ll != NULL; ll = ll->nextcand) {
    if (ll->okuri && ll->candword == okuri) {
      newfirst = &ll->okuri;
      return { newfirst, ll->okuri };
    }
  }
  if (newfirst && cl->dicitem) {
    if (cl->dicitem->cand->okuri) {
      return {};
    }
    newfirst = &(cl->dicitem->cand);
  }
  return { newfirst, cl };
}

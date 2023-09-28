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

#define _NEW(type) ((type*)malloc(sizeof(struct type)))
#define _NEW2(type, add) ((type*)malloc(sizeof(struct type) + (add)))

CandList*
getCandList(FILE* f, DicList* ditem, int okuri)
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
      citem = _NEW2(CandList, strlen(buf));
      citem->okuri = NULL;
      citem->nextcand = NULL;
      citem->prevcand = citem2;
      citem->dicitem = ditem;
      strcpy(citem->candword, buf);
      ccitem2 = citem;
      for (;;) {
        if ((c = fgetc(f)) == ']')
          break;
        for (buf[0] = c, p = buf + 1; (*p = fgetc(f)) != '/'; p++)
          ;
        *p = '\0';
        ccitem = _NEW2(CandList, strlen(buf));
        ccitem->nextcand = NULL;
        strcpy(ccitem->candword, buf);
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
      citem = _NEW2(CandList, strlen(buf));
      citem->okuri = NULL;
      citem->nextcand = NULL;
      citem->prevcand = citem2;
      citem->dicitem = ditem;
      strcpy(citem->candword, buf);
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
printCand(CandList* cl, FILE* f, PrintCandTypes fre)
{
  CandList* clist;
  CandList* clist2;
  CandList* cclist;
  CandList* cclist2;

  fputc('/', f);
  for (clist = cl; clist != NULL;
       clist2 = clist, clist = clist->nextcand, (fre ? (free(clist2), 0) : 0)) {
    if (clist->okuri) {
      fprintf(f, "[%s/", clist->candword);
      for (cclist = clist->okuri; cclist != NULL; cclist2 = cclist,
          cclist = cclist->nextcand,
          (fre ? (free(cclist2), 0) : 0)) {
        fprintf(f, "%s/", cclist->candword);
      }
      fputs("]/", f);
    } else
      fprintf(f, "%s/", clist->candword);
  }
  fputc('\n', f);
}

static void
freeCand(CandList* cl)
{
  CandList* clist;
  CandList* clist2;
  CandList* cclist;
  CandList* cclist2;

  for (clist = cl; clist != NULL;
       clist2 = clist, clist = clist->nextcand, free(clist2)) {
    if (clist->okuri) {
      for (cclist = clist->okuri; cclist != NULL;
           cclist2 = cclist, cclist = cclist->nextcand, free(cclist2))
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
      if (!strcmp(itlist->candword, l->candword)) {
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

static char*
allocStr(const char* s)
{
  int l = strlen(s);
  char* p = (char*)malloc(l + 1);

  strcpy(p, s);
  return p;
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
    if (ll->okuri && !strcmp(ll->candword, okuri)) {
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

/*
 *  SKK-like Kana-Kanji translation library
 *
 * by A.ITO November, 1991
 */

#include "skklib.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define _NEW(type) ((type*)malloc(sizeof(struct type)))
#define _NEW2(type, add) ((type*)malloc(sizeof(struct type) + (add)))

/*
 * Open SKK
 */
Dictionary*
openSKK(const char* dicname)
{
  FILE* f;
  DicList* ditem;
  DicList* ditem2;
  DicList* globaldic;
  char buf[512];
  char *p, c;
  Hash** dhash;
  Dictionary* dic;
  int i, nitem = 0;
  int okuriAri = 1;

  dhash = (Hash**)calloc(HASHSIZE, sizeof(Hash));
  dic = _NEW(Dictionary);
  dic->dhash = dhash;
  dic->okuriAriFirst = NULL;
  dic->okuriNasiFirst = NULL;
  dic->dlist = NULL;
  ditem2 = NULL;
  globaldic = NULL;
  if ((f = fopen(dicname, "r")) == NULL) {
    return dic;
  }

  struct stat st;
  fstat(fileno(f), &st);
  while (!feof(f)) {
    while ((c = fgetc(f)) == ' ' || c == '\t' || c == '\n')
      ;
    if (feof(f))
      break;
    if (c == ';') { /* comment */
      i = 0;
      while (c != '\n' && !feof(f)) {
        c = fgetc(f);
        buf[i++] = c;
      }
      buf[i] = '\0';
      if (!strncmp(buf, "; okuri-ari entries.", 20)) {
        okuriAri = 1;
      } else if (!strncmp(buf, "; okuri-nasi entries.", 21)) {
        okuriAri = 0;
      }
      continue;
    }
    nitem++;
    for (buf[0] = c, p = buf + 1; !feof(f) && (*p = fgetc(f)) != ' '; p++) {
    }
    *p = '\0';
    ditem = _NEW2(DicList, strlen(buf));
    ditem->nextitem = NULL;
    if (ditem2)
      ditem2->nextitem = ditem;
    if (globaldic == NULL)
      globaldic = ditem;
    strcpy(ditem->kanaword, buf);
    ditem->cand = getCandList(f, ditem, okuriAri);
    addHash(dhash, ditem);
    ditem2 = ditem;
    if (okuriAri) {
      if (!dic->okuriAriFirst)
        dic->okuriAriFirst = ditem2;
    } else {
      if (!dic->okuriNasiFirst)
        dic->okuriNasiFirst = ditem2;
    }
  }
  fclose(f);
  dic->dlist = globaldic;
  dic->mtime = st.st_mtime;
  return dic;
}

/*
 * Check if word is an OKURI-ARI entry or not
 */
int
isConjugate(char word[], int l)
{
  int r;
  if ((word[0] & 0x80) || word[0] == '#') {
    if (word[l - 1] & 0x80)
      r = 0;
    else
      r = (word[l - 1] != '#');
  } else
    r = 0;
  return r;
}

/*
 * Add new word entry to the dictionary
 */
DicList*
addNewItem(Dictionary* dic, char* word, CandList* clist)
{
  DicList* ditem;
  int l = strlen(word);

  ditem = _NEW2(DicList, l);
  strcpy(ditem->kanaword, word);
  ditem->cand = clist;
  addHash(dic->dhash, ditem);
  if (isConjugate(word, l)) {
    if (dic->okuriAriFirst) {
      ditem->nextitem = dic->okuriAriFirst->nextitem;
      dic->okuriAriFirst->nextitem = ditem;
    } else {
      if (dic->dlist) {
        dic->okuriAriFirst = ditem;
        ditem->nextitem = dic->okuriNasiFirst;
        dic->dlist = ditem;
      } else {
        dic->dlist = ditem;
        dic->okuriAriFirst = ditem;
      }
    }
  } else {
    if (dic->okuriNasiFirst) {
      ditem->nextitem = dic->okuriNasiFirst->nextitem;
      dic->okuriNasiFirst->nextitem = ditem;
    } else {
      if (dic->dlist) {
        ditem->nextitem = dic->dlist->nextitem;
        dic->dlist->nextitem = ditem;
        dic->okuriNasiFirst = ditem;
      } else {
        dic->dlist = ditem;
        dic->okuriNasiFirst = ditem;
      }
    }
  }
  return ditem;
}

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
closeSKK(Dictionary* dic, const char* dicname)
{
  FILE* f;
  DicList* dlist;
  DicList* dlist2;
  DicList* globaldic = dic->dlist;
  int okuri = 1;
  int l;
  char* wd;
  struct stat sbuf;
  int old = 0;

  auto buf = (char*)malloc(256);
  /* backup skk-jisyo if jisyo is not empty. */
  sprintf(buf, "%s.BAK", dicname);
  if ((stat(dicname, &sbuf) == 0) && (sbuf.st_size != 0)) {
    if (dic->mtime < sbuf.st_mtime) {
      printf("The dictionary is changed. merging...\n");
      mergeDictionary(dic, dicname);
    }
    rename(dicname, buf);
    old = 1;
  }
  if ((f = fopen(dicname, "w")) == NULL) {
    free(buf);
    return;
  }

  fprintf(f, ";; okuri-ari entries.\n");
  for (dlist = globaldic; dlist != NULL;
       dlist2 = dlist, dlist = dlist->nextitem, free(dlist2)) {
    wd = dlist->kanaword;
    l = strlen(wd);
    if (okuri && (!isConjugate(wd, l))) {
      fprintf(f, ";; okuri-nasi entries.\n");
      okuri = 0;
    }
    fprintf(f, "%s ", dlist->kanaword);
    printCand(dlist->cand, f, FREE_CAND);
  }
  fclose(f);
  if (old)
    chmod(dicname, sbuf.st_mode);

  for (l = 0; l < HASHSIZE; l++) {
    Hash* h1;
    Hash* h2;
    for (h1 = dic->dhash[l]; h1; h1 = h2) {
      h2 = h1->next;
      free(h1);
    }
  }
  free(dic->dhash);
  free(dic);
  free(buf);
}

/* #define DEBUG_MERGE /* debug dictionary merge */

void
mergeDictionary(Dictionary* dic, const char* dicname)
{
  FILE* f;
  CandList* cand;
  CandList* dcand;
  DicList* ditem;
  char *p, c;
  int i;
#ifdef DEBUG_MERGE
  DicList change[10];
  int n = 0;
#endif

  auto buf = (char*)malloc(512);
  if ((f = fopen(dicname, "r")) == NULL) {
    free(buf);
    return;
  }
  while (!feof(f)) {
    while ((c = fgetc(f)) == ' ' || c == '\t' || c == '\n')
      ;
    if (feof(f))
      break;
    if (c == ';') { /* comment */
      while (c != '\n' && !feof(f)) {
        c = fgetc(f);
      }
      continue;
    }
    for (buf[0] = c, p = buf + 1; !feof(f) && (*p = fgetc(f)) != ' '; p++) {
    }
    *p = '\0';
    i = strlen(buf);
    dcand = getCand(dic, buf);
    if (dcand == NULL) {
      cand = getCandList(f, NULL, isConjugate(buf, i));
      ditem = addNewItem(dic, buf, cand);
      for (; cand; cand = cand->nextcand)
        cand->dicitem = ditem;
#ifdef DEBUG_MERGE
      change[n++] = ditem;
#endif
    } else {
      cand = getCandList(f, dcand->dicitem, isConjugate(buf, i));
      cand = deleteCand(cand, dcand);
      if (cand) {
        dcand->dicitem->cand = cand;
        while (cand->nextcand != NULL)
          cand = cand->nextcand;
        cand->nextcand = dcand;
        dcand->prevcand = cand;
#ifdef DEBUG_MERGE
        change[n++] = dcand->dicitem;
#endif
      }
    }
  }
  fclose(f);
#ifdef DEBUG_MERGE
  for (i = 0; i < n; i++) {
    printf("i=%d; ", i);
    fflush(stdout);
    printf("register(%d): %s (%x)", i, change[i]->kanaword, change[i]->cand);
    fflush(stdout);
    printCand(change[i]->cand, stdout, NOFREE_CAND);
    putchar('\n');
  }
#endif
  free(buf);
}

void
printCand(CandList* cl, FILE* f, int fre)
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

static char*
allocStr(const char* s)
{
  int l = strlen(s);
  char* p = (char*)malloc(l + 1);

  strcpy(p, s);
  return p;
}

int
hashVal(const char* s)
{
  int n = 0;

  while (*s) {
    n += (*s) * (*s);
    s++;
  }
  return n % HASHSIZE;
}

void
addHash(Hash** hash, DicList* ditem)
{
  Hash* h;
  Hash* hh;
  int v;
  char buf[256];

  v = hashVal(ditem->kanaword);
  h = _NEW(Hash);
  h->h_index = ditem;
  h->length = strlen(ditem->kanaword);
  h->next = hash[v];
  hash[v] = h;
}

CandList*
getCand(Dictionary* dic, char* s)
{
  int l, v;
  Hash* h;

  l = strlen(s);
  v = hashVal(s);
  for (h = dic->dhash[v]; h != NULL; h = h->next) {
    if (h->length != l || strcmp(h->h_index->kanaword, s))
      continue;
    return h->h_index->cand;
  }
  return NULL;
}

void
selectCand(CandList** first, CandList* cand)
{
  if (cand->prevcand) {
    cand->prevcand->nextcand = cand->nextcand;
    if (cand->nextcand)
      cand->nextcand->prevcand = cand->prevcand;
    cand->prevcand = NULL;
  }
  if (*first != cand) {
    (*first)->prevcand = cand;
    cand->nextcand = *first;
    *first = cand;
  }
}

void
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

CandList*
firstCand(CandList* l)
{
  while (l->prevcand)
    l = l->prevcand;
  return l;
}

CandList*
searchOkuri(CandList* cl, char* okuri, CandList*** newfirst)
{
  CandList* ll;
  CandList** nc;

  for (ll = cl; ll != NULL; ll = ll->nextcand) {
    if (ll->okuri && !strcmp(ll->candword, okuri)) {
      if (newfirst)
        *newfirst = &(ll->okuri);
      return ll->okuri;
    }
  }
  if (newfirst && cl->dicitem) {
    if (cl->dicitem->cand->okuri) {
      return NULL;
    }
    *newfirst = &(cl->dicitem->cand);
  }
  return cl;
}

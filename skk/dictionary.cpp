#include "dictinary.h"
#include "skklib.h"
#include <string.h>
#include <sys/stat.h>

#define HASHSIZE 256
struct Hash
{
  DicList* h_index;
  short length;
  std::shared_ptr<Hash> next;
};

static int
hashVal(const char* s)
{
  int n = 0;

  while (*s) {
    n += (*s) * (*s);
    s++;
  }
  return n % HASHSIZE;
}

// Check if word is an OKURI-ARI entry or not
static bool
isConjugate(std::string_view word)
{
  auto l = word.size();
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

Dictionary::Dictionary(std::string_view path)
  : m_path(path)
  , dhash(HASHSIZE)
{
  if (auto f = fopen(m_path.c_str(), "r")) {

    struct stat st;
    fstat(fileno(f), &st);

    char buf[512];
    int okuriAri = 1;
    DicList* ditem2 = NULL;
    DicList* globaldic = NULL;
    while (!feof(f)) {
      char c;
      while ((c = fgetc(f)) == ' ' || c == '\t' || c == '\n')
        ;
      if (feof(f))
        break;
      if (c == ';') { /* comment */
        int i = 0;
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
      {
        char* p;
        for (buf[0] = c, p = buf + 1; !feof(f) && (*p = fgetc(f)) != ' '; p++) {
        }
        *p = '\0';
      }
      auto ditem = new DicList(buf);
      if (ditem2)
        ditem2->nextitem = ditem;
      if (globaldic == NULL)
        globaldic = ditem;
      ditem->cand = CandList::getCandList(f, ditem, okuriAri);
      addHash(ditem);
      ditem2 = ditem;
      if (okuriAri) {
        if (!this->okuriAriFirst)
          this->okuriAriFirst = ditem2;
      } else {
        if (!this->okuriNasiFirst)
          this->okuriNasiFirst = ditem2;
      }
    }
    this->dlist = globaldic;
    this->mtime = st.st_mtime;
    fclose(f);
  }
}

Dictionary::~Dictionary()
{
  int old = 0;
  std::string buf;
  /* backup skk-jisyo if jisyo is not empty. */
  buf += m_path;
  buf += ".BAK";
  struct stat sbuf;
  if ((stat(m_path.c_str(), &sbuf) == 0) && (sbuf.st_size != 0)) {
    if (this->mtime < sbuf.st_mtime) {
      printf("The dictionary is changed. merging...\n");
      mergeDictionary(m_path.c_str());
    }
    rename(m_path.c_str(), buf.c_str());
    old = 1;
  }
  if (auto f = fopen(m_path.c_str(), "w")) {
    fprintf(f, ";; okuri-ari entries.\n");
    DicList* globaldic = this->dlist;
    DicList* dlist2;
    int okuri = 1;
    for (dlist = globaldic; dlist != NULL;
         dlist2 = dlist, dlist = dlist->nextitem, delete dlist2) {
      auto wd = dlist->kanaword;
      if (okuri && (!isConjugate(wd))) {
        fprintf(f, ";; okuri-nasi entries.\n");
        okuri = 0;
      }
      fprintf(f, "%s ", dlist->kanaword.c_str());
      dlist->cand->print(f, PrintCandTypes::FREE_CAND);
    }
    fclose(f);
    if (old)
      chmod(m_path.c_str(), sbuf.st_mode);
  }
}

/*
 * Add new word entry to the dictionary
 */
DicList*
Dictionary::addNewItem(const char* word, CandList* clist)
{
  auto ditem = new DicList(word);
  ditem->cand = clist;
  addHash(ditem);
  if (isConjugate(word)) {
    if (this->okuriAriFirst) {
      ditem->nextitem = this->okuriAriFirst->nextitem;
      this->okuriAriFirst->nextitem = ditem;
    } else {
      if (this->dlist) {
        this->okuriAriFirst = ditem;
        ditem->nextitem = this->okuriNasiFirst;
        this->dlist = ditem;
      } else {
        this->dlist = ditem;
        this->okuriAriFirst = ditem;
      }
    }
  } else {
    if (this->okuriNasiFirst) {
      ditem->nextitem = this->okuriNasiFirst->nextitem;
      this->okuriNasiFirst->nextitem = ditem;
    } else {
      if (this->dlist) {
        ditem->nextitem = this->dlist->nextitem;
        this->dlist->nextitem = ditem;
        this->okuriNasiFirst = ditem;
      } else {
        this->dlist = ditem;
        this->okuriNasiFirst = ditem;
      }
    }
  }
  return ditem;
}

CandList*
Dictionary::getCand(const char* s) const
{
  int l = strlen(s);
  auto v = hashVal(s);
  for (auto h = this->dhash[v]; h != NULL; h = h->next) {
    if (h->length != l || h->h_index->kanaword != s)
      continue;
    return h->h_index->cand;
  }
  return NULL;
}

void
Dictionary::addHash(DicList* ditem)
{
  auto v = hashVal(ditem->kanaword.c_str());
  auto h = std::make_shared<Hash>();
  h->h_index = ditem;
  h->length = ditem->kanaword.size();
  h->next = dhash[v];
  dhash[v] = h;
}

void
Dictionary::mergeDictionary(const char* dicname)
{
  // FILE* f;
  // CandList* cand;
  // CandList* dcand;
  // DicList* ditem;
  // char *p, c;
  // int i;

  std::string buf;
  if (auto f = fopen(dicname, "r")) {
    while (!feof(f)) {
      char c;
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
      buf.push_back(c);
      while (!feof(f)) {
        buf.push_back(fgetc(f));
        if (buf.back() == ' ') {
          break;
        }
      }
      auto dcand = this->getCand(buf.c_str());
      if (dcand == NULL) {
        auto cand = CandList::getCandList(f, NULL, isConjugate(buf));
        auto ditem = this->addNewItem(buf.c_str(), cand);
        for (; cand; cand = cand->nextcand)
          cand->dicitem = ditem;
      } else {
        auto cand = CandList::getCandList(f, dcand->dicitem, isConjugate(buf));
        cand = deleteCand(cand, dcand);
        if (cand) {
          dcand->dicitem->cand = cand;
          while (cand->nextcand != NULL)
            cand = cand->nextcand;
          cand->nextcand = dcand;
          dcand->prevcand = cand;
        }
      }
    }
    fclose(f);
  }
}

#include "dictinary.h"
#include "skklib.h"
#include <assert.h>
#include <fstream>
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
hashVal(std::string_view s)
{
  int n = 0;
  for (auto c : s) {
    n += (c) * (c);
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
    this->mtime = st.st_mtime;

    DicList* lastItem = NULL;
    bool okuriAri = true;
    char buf[4096];
    while (auto l = fgets(buf, std::size(buf), f)) {
      // skip white space
      std::string_view line(l);
      assert(line.back() == '\n');
      while (line.size() &&
             (line[0] == ' ' || line[0] == '\t' || line[0] == '\n')) {
        line = line.substr(1);
      }
      if (line.empty()) {
        continue;
      }

      // comment
      if (line[0] == ';') {
        if (line.starts_with("; okuri-ari entries.")) {
          okuriAri = true;
        } else if (line.starts_with("; okuri-nasi entries.")) {
          okuriAri = 0;
        }
        continue;
      }

      // 見出し語
      auto space = line.find(' ');
      if (space == std::string::npos) {
        // ?
        continue;
      }
      auto word = line.substr(0, space);
      line = line.substr(space + 1);

      // new item
      auto ditem = new DicList(word);
      if (!dlist) {
        dlist = ditem;
      }
      if (lastItem) {
        lastItem->nextitem = ditem;
      }

      // candidate
      ditem->cand = CandList::getCandList(line, ditem, okuriAri);
      addHash(ditem);
      lastItem = ditem;
      if (okuriAri) {
        if (!this->okuriAriFirst)
          this->okuriAriFirst = lastItem;
      } else {
        if (!this->okuriNasiFirst)
          this->okuriNasiFirst = lastItem;
      }
    }
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
Dictionary::addNewItem(std::string_view word, CandList* clist)
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
Dictionary::getCand(std::string_view s) const
{
  auto v = hashVal(s);
  for (auto h = this->dhash[v]; h; h = h->next) {
    if (h->length != s.size() || h->h_index->kanaword != s)
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
    char buf[512];
    while (auto l = fgets(buf, std::size(buf), f)) {
      // skip white space
      std::string_view line(l);
      while (line.size() &&
             (line[0] == ' ' || line[0] == '\t' || line[0] == '\n')) {
        line = line.substr(1);
      }
      if (line.empty()) {
        continue;
      }

      // comment
      if (line[0] == ';') {
        continue;
      }

      // 見出し語
      auto space = line.find(' ');
      if (space == std::string::npos) {
        // ?
        continue;
      }
      auto word = line.substr(0, space);
      line = line.substr(space + 1);

      auto dcand = this->getCand(word);
      if (!dcand) {
        auto cand = CandList::getCandList(line, NULL, isConjugate(buf));
        auto ditem = this->addNewItem(word, cand);
        for (; cand; cand = cand->nextcand)
          cand->dicitem = ditem;
      } else {
        auto cand =
          CandList::getCandList(line, dcand->dicitem, isConjugate(buf));
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

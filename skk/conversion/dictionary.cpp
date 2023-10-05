#include "dictionary.h"
#include <assert.h>
#include <sys/stat.h>

namespace skk {

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

static CandList
makeCand(std::string_view line, bool okuri)
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

  CandList list;
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

    // auto citem = new CandList(buf);

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
      // citem = new CandList(buf);
      // citem->prevcand = citem2;
      // citem->dicitem = ditem;
      list.push_back({ buf.begin(), buf.end() });
    }
  }

  return list;
}

bool
Dictionary::load(std::string_view path)
{
  m_path = path;
  auto f = fopen(m_path.c_str(), "r");
  if (!f) {
    return false;
  }

  struct stat st;
  fstat(fileno(f), &st);
  this->mtime = st.st_mtime;

  // DicList* lastItem = NULL;
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
      if (line.find("; okuri-ari entries.") != std::string_view::npos) {
        okuriAri = true;
      } else if (line.find("; okuri-nasi entries.") != std::string_view::npos) {
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
    auto _word = line.substr(0, space);
    std::string word{ _word.begin(), _word.end() };
    line = line.substr(space + 1);

    // new entry
    auto list = makeCand(line, okuriAri);
    assert(list.size());
    if (okuriAri) {
      auto ret = m_okuriAri.insert({ word, list });
      assert(ret.second);
    } else {
      auto ret = m_okuriNasi.insert({ word, list });
      assert(ret.second);
    }
  }
  fclose(f);
  return true;
}

// Dictionary::~Dictionary()
// {
//   int old = 0;
//   std::string buf;
//   /* backup skk-jisyo if jisyo is not empty. */
//   buf += m_path;
//   buf += ".BAK";
//   struct stat sbuf;
//   if ((stat(m_path.c_str(), &sbuf) == 0) && (sbuf.st_size != 0)) {
//     if (this->mtime < sbuf.st_mtime) {
//       printf("The dictionary is changed. merging...\n");
//       mergeDictionary(m_path.c_str());
//     }
//     rename(m_path.c_str(), buf.c_str());
//     old = 1;
//   }
//   // if (auto f = fopen(m_path.c_str(), "w")) {
//   //   fprintf(f, ";; okuri-ari entries.\n");
//   //   int okuri = 1;
//   //   for (dlist = globaldic; dlist != NULL;
//   //        dlist2 = dlist, dlist = dlist->nextitem, delete dlist2) {
//   //     auto wd = dlist->kanaword;
//   //     if (okuri && (!isConjugate(wd))) {
//   //       fprintf(f, ";; okuri-nasi entries.\n");
//   //       okuri = 0;
//   //     }
//   //     fprintf(f, "%s ", dlist->kanaword.c_str());
//   //     dlist->cand->print(f, PrintCandTypes::FREE_CAND);
//   //   }
//   //   fclose(f);
//   //   if (old)
//   //     chmod(m_path.c_str(), sbuf.st_mode);
//   // }
// }

const CandList*
Dictionary::getCand(std::string_view s) const
{
  {
    auto found = m_okuriAri.find({ s.begin(), s.end() });
    if (found != m_okuriAri.end()) {
      return &found->second;
    }
  }
  {
    auto found = m_okuriNasi.find({ s.begin(), s.end() });
    if (found != m_okuriNasi.end()) {
      return &found->second;
    }
  }
  return {};
}

void
Dictionary::mergeDictionary(const std::string& dicname)
{
  std::string buf;
  if (auto f = fopen(dicname.c_str(), "r")) {
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

      ;
      auto okuriAri = isConjugate(word);
      if (auto list = this->getCand(word)) {
        // merge
        assert(false);
        // auto cand =
        //   CandList::getCandList(line, dcand->dicitem, okuriAri);
        // cand = deleteCand(cand, dcand);
      } else {
        // add
        auto cands = makeCand(line, okuriAri);
        assert(cands.size());
        if (okuriAri) {
          m_okuriAri.insert({ { word.begin(), word.end() }, cands });
        } else {
          m_okuriNasi.insert({ { word.begin(), word.end() }, cands });
        }
      }
    }
    fclose(f);
  }
}

} // namespace

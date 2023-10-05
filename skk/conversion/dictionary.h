#pragma once
#include <list>
#include <memory>
#include <string>
#include <time.h>
#include <unordered_map>
#include <vector>

namespace skk {

using CandList = std::list<std::string>;

struct Dictionary
{
private:
  std::string m_path;
  std::unordered_map<std::string, CandList> m_okuriAri;
  std::unordered_map<std::string, CandList> m_okuriNasi;
  time_t mtime;

public:
  Dictionary() {}
  ~Dictionary() {}
  Dictionary(const Dictionary&) = delete;
  Dictionary& operator=(const Dictionary&) = delete;

  bool load(std::string_view path);
  const CandList* getCand(std::string_view s) const;

private:
  void mergeDictionary(const std::string& dicname);
};

struct CandidateSelector
{
  const CandList* List = nullptr;
  std::list<std::string>::const_iterator Cand;

  CandidateSelector() {}

  CandidateSelector(const CandList* list)
    : List(list)
  {
    if (List) {
      Cand = List->begin();
    }
  }

  bool IsEnabled()
  {
    if (List && Cand != List->end()) {
      return true;
    }
    return false;
  }

  bool Increment()
  {
    if (IsEnabled()) {
      ++Cand;
      return true;
    }
    return false;
  }

  bool Decrement()
  {
    if (IsEnabled() && Cand != List->begin()) {
      --Cand;
      return true;
    }
    return false;
  }
};

} // namespace

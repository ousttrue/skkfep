#pragma once
#include <list>
#include <memory>
#include <string>
#include <time.h>
#include <unordered_map>
#include <vector>

using CandList = std::list<std::string>;

struct Dictionary
{
private:
  std::string m_path;
  std::unordered_map<std::string, CandList> m_okuriAri;
  std::unordered_map<std::string, CandList> m_okuriNasi;
  time_t mtime;

public:
  Dictionary(){}
  ~Dictionary(){}
  Dictionary(const Dictionary&) = delete;
  Dictionary& operator=(const Dictionary&) = delete;

  bool load(std::string_view path);
  const CandList* getCand(std::string_view s) const;

private:
  void mergeDictionary(const std::string& dicname);
};

#pragma once
#include <memory>
#include <string>
#include <vector>
struct Hash;
struct DicList;
struct CandList;

struct Dictionary
{
private:
  std::string m_path;
  DicList* dlist = nullptr;
  DicList* okuriAriFirst = nullptr;
  DicList* okuriNasiFirst = nullptr;
  std::vector<std::shared_ptr<Hash>> dhash;
  time_t mtime;

public:
  Dictionary(std::string_view path);
  ~Dictionary();
  Dictionary(const Dictionary&) = delete;
  Dictionary& operator=(const Dictionary&) = delete;
  CandList* getCand(std::string_view s) const;

private:
  DicList* addNewItem(std::string_view word, CandList* clist);
  void addHash(DicList* ditem);
  void mergeDictionary(const char* dicname);
};

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
  CandList* getCand(const char* s) const;

private:
  DicList* addNewItem(const char* word, CandList* clist);
  void addHash(DicList* ditem);
  void mergeDictionary(const char* dicname);
};

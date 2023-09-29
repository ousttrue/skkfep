#pragma once
#include <string>

struct Capability
{
  std::string seq;
  mutable std::string m_buf;

  bool getstr(const char* name);
  operator bool() const;
  std::string_view xy(int x, int y) const;
};

struct Terminal
{
  Capability ce;
  Capability kr;
  Capability kl;
  Capability cr;
  Capability bt;
  Capability ta;
  Capability sc;
  Capability rc;
  Capability so;
  Capability se;
  Capability us;
  Capability ue;
  Capability cl;
  Capability cs;
  Capability cm;
  Capability ts;
  Capability fs;
  Capability ss;
  Capability hs;
  Capability es;

  bool load(const char* term);
};

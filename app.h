#pragma once
#include "skk.h"
#include <string>
#include <string_view>

class App
{
  skk::Skk m_skk;
  App();

public:
  ~App();
  App(const App&) = delete;
  App& operator=(const App&) = delete;
  static App& Instance()
  {
    static App s_instance;
    return s_instance;
  }

  bool Initialize(const std::string& UserDicName,
                  const std::string& cmd,
                  char** args,
                  const char* version);
  int Run();

  void Abort();
  void Exit(int code);
  void Reset(const char* msg);

private:
  void PtyFree();
  void SaveJisyo();
};

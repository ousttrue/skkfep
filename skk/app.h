#pragma once
#include <string>

class App
{
  std::string UserDicName;

  App();

public:
  struct Dictionary* UserDic = nullptr;
  ~App();
  App(const App&) = delete;
  App& operator=(const App&) = delete;
  static App& Instance()
  {
    static App s_instance;
    return s_instance;
  }

  bool Initialize(const char* UserDicName, const char* cmd, char** args);
  int Run();

  void Abort();
  void Exit(int code);
  void Reset(const char* msg);

  void PtyFree();

  void OpenDictionary(const char* path);
  void SaveJisyo();
};

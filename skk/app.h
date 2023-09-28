#pragma once
#include <string>
#include <string_view>

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

  bool Initialize(std::string_view UserDicName, const char* cmd, char** args);
  int Run();

  void Abort();
  void Exit(int code);
  void Reset(const char* msg);

  void PtyFree();

  void OpenDictionary(std::string_view path);
  void SaveJisyo();
};
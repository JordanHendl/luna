#pragma once
#ifdef _WIN32
  #ifdef  LunaLibraryExport
    #define LunaAPI __declspec(dllexport)
  #else
    #define LunaAPI __declspec(dllimport)  
  #endif
#elif defined __linux__
    #define LunaAPI
#endif

namespace luna {
  LunaAPI auto init_engine(const char* db_path) -> void;
  LunaAPI auto run() -> int;
}
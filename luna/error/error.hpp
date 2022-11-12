#pragma once
namespace luna {
//#ifdef Luna_Debug
}
#include <cassert>
#include <iostream>
namespace luna {
template<typename ...Args>
inline auto _luna_assert_impl(Args ... args) {
  {(std::cout << "----- ERROR ---- Assertion failed: " << ... << args); std::cout << "---- ERROR ----" << std::endl;}
}

#define LunaAssert(cond, ...)       \
  if (!(cond)) {                    \
    luna::_luna_assert_impl(__VA_ARGS__); \
    assert(cond);                   \
  }
//#else
//#define LunaAssert(cond, description)
//#endif
}
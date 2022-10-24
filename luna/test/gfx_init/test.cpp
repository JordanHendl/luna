#include "luna/core/luna.hpp"
#include <gtest/gtest.h>
#include <utility>

int main(int argc, char* argv[]) {
  if(argc <= 1){ std::cout << "Usage: " << argv[0] << " <path to sample database>" << std::endl; return 0;}
  luna::init_engine(argv[1]);
  //testing::InitGoogleTest(&argc, argv);
  //auto success = RUN_ALL_TESTS();
  return luna::run();
}

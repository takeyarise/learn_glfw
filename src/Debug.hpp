#pragma once

#if defined(DEBUG)
  #define PRINT_DEBUG(fmt) do{std::cout << "[DEBUG: " << __FILE__ << ":" << __LINE__ << "] " << fmt << std::endl;}while(0)
#else
  #define PRINT_DEBUG(fmt) ((void)0)
#endif

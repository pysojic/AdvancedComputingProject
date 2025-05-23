// Log.hpp
#pragma once
#include <iostream>
#include <mutex>
#include <string>

namespace Log {
  static std::mutex mu;
  enum Level { INFO = 0, DEBUG = 1, WARN = 2 };
  inline Level globalLevel = INFO;
  inline void setLevel(Level L) { globalLevel = L; }
  inline void line(Level L, const std::string& msg) {
    if (L < globalLevel) return;
    std::lock_guard<std::mutex> lk(mu);
    const char* prefix = (L==INFO ? "[INFO] " :
                          L==DEBUG? "[DEBUG]" :
                                     "[WARN] ");
    std::cout << prefix << " " << msg << "\n";
  }
}

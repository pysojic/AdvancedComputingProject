#pragma once
#include <iostream>
#include <mutex>
#include <string>

namespace Log {
  static std::mutex mu;
  enum Level { INFO, DEBUG, WARN };
  inline const char* levelColor(Level L) {
    switch(L) {
      case INFO:  return "\x1b[32m"; // green
      case DEBUG: return "\x1b[36m"; // cyan
      case WARN:  return "\x1b[33m"; // yellow
    }
    return "\x1b[0m";
  }
  inline void line(Level L, const std::string& msg) {
    std::lock_guard lk(mu);
    std::cout
      << levelColor(L)
      << (L==INFO ? "ℹ️  " : L==DEBUG ? "🐞 " : "⚠️  ")
      << msg
      << "\x1b[0m\n";
  }
}

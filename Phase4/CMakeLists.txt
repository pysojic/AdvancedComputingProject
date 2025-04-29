cmake_minimum_required(VERSION 3.10)
project(HFT_Project)

set(CMAKE_CXX_STANDARD 17)

# Make headers in include/ and generated headers in build/ visible
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

# ——— CMake cache options ———
set(DEFAULT_TICKS    10000       CACHE STRING "How many ticks to simulate")
set(DEFAULT_PTRMODE  "shared"    CACHE STRING "Pointer mode: shared,pool,raw")
set(ENABLE_ALIGN64   ON          CACHE BOOL   "Apply alignas(64) to MarketData")
set(ENABLE_POOL      ON          CACHE BOOL   "Use MemoryPool by default")
set(DEFAULT_LAYOUT   "multimap"  CACHE STRING "Container layout: multimap,flat")

# Convert BOOLs (ON/OFF) into integer 1/0 for config.h
if(ENABLE_ALIGN64)
    set(ENABLE_ALIGN64_VALUE 1)
else()
    set(ENABLE_ALIGN64_VALUE 0)
endif()

if(ENABLE_POOL)
    set(ENABLE_POOL_VALUE 1)
else()
    set(ENABLE_POOL_VALUE 0)
endif()

# Generate config.h from config.h.in
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
    @ONLY
)

# ——— Main executable ———
add_executable(hft_app
    src/main.cpp
    src/MarketData.cpp
    src/OrderBook.cpp
    src/MatchingEngine.cpp
    src/OrderManager.cpp
    src/TradeLogger.cpp
    src/LatencyAnalyzer.cpp
    src/MemoryPool.cpp
)

# ——— Latency‐unit test ———
add_executable(test_latency
    test/test_latency.cpp
    src/LatencyAnalyzer.cpp
)
add_test(NAME LatencyTest COMMAND test_latency)

cmake_minimum_required(VERSION 3.10)
project(HFT_Project)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

# main app
add_executable(hft_app
    src/main.cpp
    src/MarketData.cpp
    src/OrderBook.cpp
    src/MatchingEngine.cpp
    src/OrderManager.cpp
    src/TradeLogger.cpp
    src/LatencyAnalyzer.cpp
)

# latency test
add_executable(test_latency
    test/test_latency.cpp
    src/LatencyAnalyzer.cpp  # reuse analyzer impl
)
add_test(NAME LatencyTest COMMAND test_latency)

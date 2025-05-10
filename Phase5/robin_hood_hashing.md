## Benchmark

Here is the console output of the benchmark of the robinhood hash table vs std::unordered_map:

- When the size of the table is set to 1000 times the number of market data inserted

    Robin Hood Insert: 237.928milliseconds
    std::unordered_map Insert: 164.294milliseconds
    Robin Hood Lookup: 47.9074milliseconds
    std::unordered_map Lookup: 28.0773milliseconds

- When the size of the table is set to 2 times the number of market data inserted

    Robin Hood Insert: 24.5124milliseconds
    std::unordered_map Insert: 35.7107milliseconds
    Robin Hood Lookup: 17.4531milliseconds
    std::unordered_map Lookup: 19.3579milliseconds

- When the size of the table is set to be exactly the number of market data inserted

    Robin Hood Insert: 1555.45milliseconds
    std::unordered_map Insert: 33.6775milliseconds
    Robin Hood Lookup: 811.668milliseconds
    std::unordered_map Lookup: 19.2522milliseconds


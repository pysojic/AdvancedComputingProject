#include <iostream>
#include <unordered_map>
#include <map>
#include <list>
#include <vector>
#include <chrono>
#include <random>

struct Order {
    int    id;
    double price;
    int    qty;
    char   side; // 'B' or 'S'
};

class NaiveOrderBook {
public:
    std::unordered_map<int, Order> orders_by_id;
    std::map<double, std::vector<int>> levels[2]; // 0 = bids, 1 = asks

    void add(int id, double p, int q, char s) {
        orders_by_id[id] = {id,p,q,s};
        auto& lvl = levels[(s=='B')?0:1][p];
        lvl.push_back(id);
    }

    bool modify(int id, int newQty) {
        auto it = orders_by_id.find(id);
        if (it==orders_by_id.end()) return false;
        it->second.qty = newQty;
        return true;
    }

    bool remove(int id) {
        auto it = orders_by_id.find(id);
        if (it==orders_by_id.end()) return false;
        double p = it->second.price;
        char side = it->second.side;
        auto& vec = levels[(side=='B')?0:1][p];
        // O(n) find & erase
        for (auto vit = vec.begin(); vit!=vec.end(); ++vit) {
            if (*vit == id) { vec.erase(vit); break; }
        }
        if (vec.empty()) levels[(side=='B')?0:1].erase(p);
        orders_by_id.erase(it);
        return true;
    }

    std::vector<Order> query_price(double p) {
        std::vector<Order> out;
        if (auto itb = levels[0].find(p); itb!=levels[0].end())
            for (int id: itb->second) out.push_back(orders_by_id[id]);
        if (auto ita = levels[1].find(p); ita!=levels[1].end())
            for (int id: ita->second) out.push_back(orders_by_id[id]);
        return out;
    }

    std::pair<double, std::vector<Order>> bestBid() {
        if (levels[0].empty()) return {0,{}};
        auto it = levels[0].rbegin();
        std::vector<Order> v;
        for (int id: it->second) v.push_back(orders_by_id[id]);
        return {it->first, v};
    }
    std::pair<double, std::vector<Order>> bestAsk() {
        if (levels[1].empty()) return {0,{}};
        auto it = levels[1].begin();
        std::vector<Order> v;
        for (int id: it->second) v.push_back(orders_by_id[id]);
        return {it->first, v};
    }
};

class EfficientOrderBook {
    struct Loc {
        char side;
        double price;
        std::list<Order>::iterator iter;
    };

    std::map<double, std::list<Order>> levels[2];
    std::unordered_map<int, Loc> index;

public:
    void add(int id, double p, int q, char s) {
        int idx = (s=='B')?0:1;
        auto& lst = levels[idx][p];
        lst.push_back({id,p,q,s});
        index[id] = {s,p, std::prev(lst.end())};
    }

    bool modify(int id, int newQty) {
        auto it = index.find(id);
        if (it==index.end()) return false;
        it->second.iter->qty = newQty;
        return true;
    }

    bool remove(int id) {
        auto it = index.find(id);
        if (it==index.end()) return false;
        int idx = (it->second.side=='B')?0:1;
        auto& lvl = levels[idx][it->second.price];
        lvl.erase(it->second.iter);                   // O(1)
        if (lvl.empty()) levels[idx].erase(it->second.price);
        index.erase(it);
        return true;
    }

    std::vector<Order> query_price(double p) {
        std::vector<Order> out;
        for (int side=0;side<2;++side) {
            if (auto it=levels[side].find(p); it!=levels[side].end())
                out.insert(out.end(), it->second.begin(), it->second.end());
        }
        return out;
    }

    std::pair<double,std::vector<Order>> bestBid() {
        if (levels[0].empty()) return {0,{}};
        auto it = levels[0].rbegin();
        return {it->first, {it->second.begin(), it->second.end()}};
    }
    std::pair<double,std::vector<Order>> bestAsk() {
        if (levels[1].empty()) return {0,{}};
        auto it = levels[1].begin();
        return {it->first, {it->second.begin(), it->second.end()}};
    }
};

void benchmark(size_t nOps=100000) {
    std::mt19937_64 rng(123);
    std::uniform_real_distribution<double> priceDist(90.0,110.0);
    std::uniform_int_distribution<int> qtyDist(1,1000);
    std::uniform_int_distribution<char> sideDist(0,1);

    NaiveOrderBook naive;
    EfficientOrderBook fast;

    std::vector<std::tuple<int,double,int,char>> ops;
    ops.reserve(nOps);
    for (int i=1;i<=int(nOps);++i) {
        ops.emplace_back(i, priceDist(rng), qtyDist(rng), sideDist(rng)?'B':'S');
    }

    auto run = [&](auto& book, const char* name){
        using clk = std::chrono::high_resolution_clock;
        auto t0 = clk::now();
        for (auto& [id,p,q,s] : ops) book.add(id,p,q,s);
        for (auto& [id,p,q,s] : ops) book.modify(id, q+1);
        for (auto& [id,p,q,s] : ops) book.query_price(std::get<1>(ops[id%ops.size()]));
        for (auto& [id,p,q,s] : ops) {
            if (id % 3 == 0) book.remove(id);
        }
        auto t1 = clk::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count();
        std::cout << name << ": " << ms << " ms\n";
    };

    run(naive, "Naïve");
    run(fast,  "Efficient");
}

int main()
{
    benchmark(200000);
    return 0;
}

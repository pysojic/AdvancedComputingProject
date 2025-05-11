#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <unordered_map>
#include <map>
#include <vector>
#include <optional>

struct Order {
    int id;
    double price;
    int quantity;
    char side;
};

class OrderBookHybrid {
public:
    void addOrder(const Order& o){
        orders_by_id[o.id] = o;
        if(o.side=='B') bid_levels[o.price].push_back(o.id);
        else               ask_levels[o.price].push_back(o.id);
    }
    bool modifyOrder(int id,int newQty){
        auto it = orders_by_id.find(id);
        if(it==orders_by_id.end()) return false;
        it->second.quantity = newQty;
        return true;
    }
    bool deleteOrder(int id){
        auto it = orders_by_id.find(id);
        if(it==orders_by_id.end()) return false;
        Order o = it->second;
        auto& vec = (o.side=='B' ? bid_levels[o.price] : ask_levels[o.price]);
        for(size_t i=0;i<vec.size();++i)
            if(vec[i]==id){
                vec[i] = vec.back();
                vec.pop_back();
                break;
            }
        orders_by_id.erase(it);
        return true;
    }
    std::vector<Order> getOrdersAtPrice(double price,char side) const{
        const auto& levels = (side=='B' ? bid_levels : ask_levels);
        auto it = levels.find(price);
        if(it==levels.end()) return {};
        std::vector<Order> res;
        for(int id: it->second)
            res.push_back(orders_by_id.at(id));
        return res;
    }
    std::optional<double> bestBidPrice() const{
        if(bid_levels.empty()) return std::nullopt;
        return bid_levels.rbegin()->first;
    }
    std::optional<double> bestAskPrice() const{
        if(ask_levels.empty()) return std::nullopt;
        return ask_levels.begin()->first;
    }
private:
    std::unordered_map<int,Order>       orders_by_id;
    std::map<double,std::vector<int>>   bid_levels;
    std::map<double,std::vector<int>>   ask_levels;
};

class OrderBookMapOnly {
public:
    void addOrder(const Order& o){
        orders_by_id[o.id] = o;
        if(o.side=='B') bid_levels[o.price].push_back(o.id);
        else               ask_levels[o.price].push_back(o.id);
    }
    bool modifyOrder(int id,int newQty){
        auto it = orders_by_id.find(id);
        if(it==orders_by_id.end()) return false;
        it->second.quantity = newQty;
        return true;
    }
    bool deleteOrder(int id){
        auto it = orders_by_id.find(id);
        if(it==orders_by_id.end()) return false;
        Order o = it->second;
        auto& vec = (o.side=='B' ? bid_levels[o.price] : ask_levels[o.price]);
        for(size_t i=0;i<vec.size();++i)
            if(vec[i]==id){
                vec[i] = vec.back();
                vec.pop_back();
                break;
            }
        orders_by_id.erase(it);
        return true;
    }
    std::vector<Order> getOrdersAtPrice(double price,char side) const{
        const auto& levels = (side=='B' ? bid_levels : ask_levels);
        auto it = levels.find(price);
        if(it==levels.end()) return {};
        std::vector<Order> res;
        for(int id: it->second)
            res.push_back(orders_by_id.at(id));
        return res;
    }
    std::optional<double> bestBidPrice() const{
        if(bid_levels.empty()) return std::nullopt;
        return bid_levels.rbegin()->first;
    }
    std::optional<double> bestAskPrice() const{
        if(ask_levels.empty()) return std::nullopt;
        return ask_levels.begin()->first;
    }
private:
    std::map<int,Order>                 orders_by_id;
    std::map<double,std::vector<int>>   bid_levels;
    std::map<double,std::vector<int>>   ask_levels;
};

#endif


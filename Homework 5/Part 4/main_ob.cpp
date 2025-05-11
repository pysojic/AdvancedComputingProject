#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include "order_book.h"

int main(){
    const size_t n = 100000;
    std::vector<Order> orders;
    orders.reserve(n);
    std::mt19937_64 rng(42);
    std::uniform_real_distribution<double> priceDist(10.0,100.0);
    for(size_t i=0;i<n;++i){
        double p = std::round(priceDist(rng)*100.0)/100.0;
        char side = (i%2==0?'B':'S');
        orders.push_back(Order{static_cast<int>(i),p,static_cast<int>(rng()%1000),side});
    }
    std::vector<double> queryPrices;
    queryPrices.reserve(n);
    for(auto& o:orders) queryPrices.push_back(o.price);

    OrderBookHybrid hybrid;
    auto t0 = std::chrono::high_resolution_clock::now();
    for(auto& o:orders) hybrid.addOrder(o);
    auto t1 = std::chrono::high_resolution_clock::now();
    for(size_t i=0;i<n;++i) hybrid.bestBidPrice(), hybrid.bestAskPrice();
    auto t2 = std::chrono::high_resolution_clock::now();
    for(size_t i=0;i<n;++i) hybrid.modifyOrder(i, orders[i].quantity+1);
    auto t3 = std::chrono::high_resolution_clock::now();
    for(size_t i=0;i<n;++i) hybrid.deleteOrder(i);
    auto t4 = std::chrono::high_resolution_clock::now();
    for(size_t i=0;i<n;++i) hybrid.getOrdersAtPrice(queryPrices[i], orders[i].side);
    auto t5 = std::chrono::high_resolution_clock::now();

    std::cout<<"Hybrid Add ms: "<<std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count()<<"\n";
    std::cout<<"Hybrid Best ms: "<<std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()<<"\n";
    std::cout<<"Hybrid Modify ms: "<<std::chrono::duration_cast<std::chrono::milliseconds>(t3-t2).count()<<"\n";
    std::cout<<"Hybrid Delete ms: "<<std::chrono::duration_cast<std::chrono::milliseconds>(t4-t3).count()<<"\n";
    std::cout<<"Hybrid Query ms: "<<std::chrono::duration_cast<std::chrono::milliseconds>(t5-t4).count()<<"\n";

    OrderBookMapOnly mo;
    t0 = std::chrono::high_resolution_clock::now();
    for(auto& o:orders) mo.addOrder(o);
    t1 = std::chrono::high_resolution_clock::now();
    for(size_t i=0;i<n;++i) mo.bestBidPrice(), mo.bestAskPrice();
    t2 = std::chrono::high_resolution_clock::now();
    for(size_t i=0;i<n;++i) mo.modifyOrder(i, orders[i].quantity+1);
    t3 = std::chrono::high_resolution_clock::now();
    for(size_t i=0;i<n;++i) mo.deleteOrder(i);
    t4 = std::chrono::high_resolution_clock::now();
    for(size_t i=0;i<n;++i) mo.getOrdersAtPrice(queryPrices[i], orders[i].side);
    t5 = std::chrono::high_resolution_clock::now();

    std::cout<<"MapOnly Add ms: "<<std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count()<<"\n";
    std::cout<<"MapOnly Best ms: "<<std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()<<"\n";
    std::cout<<"MapOnly Modify ms: "<<std::chrono::duration_cast<std::chrono::milliseconds>(t3-t2).count()<<"\n";
    std::cout<<"MapOnly Delete ms: "<<std::chrono::duration_cast<std::chrono::milliseconds>(t4-t3).count()<<"\n";
    std::cout<<"MapOnly Query ms: "<<std::chrono::duration_cast<std::chrono::milliseconds>(t5-t4).count()<<"\n";

    return 0;
}

#include "OrderProcessing.hpp"
#include "Analytics.hpp"
#include <iostream>
#include <thread>

int main()
{
    constexpr int P = 5, C = 4, ORDERS = 10;           // 5×10 = 50 orders
    std::vector<std::thread> prod, cons;

    for(int i=0;i<P;++i)  prod.emplace_back(producer,i+1,ORDERS);
    for(int i=0;i<C;++i)  cons.emplace_back(consumer);

    std::cout<<"Launching…\n";
    startLatch.count_down();                           // let everyone run

    for(auto& t:prod) t.join();
    for(auto& t:cons) t.join();

    std::cout<<"Processed: "<<processed.load()<<"\n";
    runAnalytics();

    std::shared_lock sl{obMtx};
    std::cout<<"--- Order Book ---\n";
    for(auto& [sym,v]:orderBook)
        std::cout<<sym<<"  buy="<<v.first<<"  sell="<<v.second<<"\n";
}

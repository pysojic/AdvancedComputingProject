#include <map>
#include <memory>
#include <iostream>

class OrderManager {
    public:
        enum class Side { Buy, Sell };
    
        struct MyOrder {
            int id;
            Side side;
            double price;
            int quantity;
            int filled;  
            MyOrder(int id_, Side side_, double price_, int quantity_)
                : id(id_), side(side_), price(price_), quantity(quantity_), filled(0) {}
        };
    
    private:
        std::map<int, std::unique_ptr<MyOrder>> orders;
        int nextOrderId = 1;
    
    public:
        int placeOrder(Side side, double price, int quantity);
    
        bool cancelOrder(int orderId);
    
        void handleExecution(int orderId, int fillQuantity);
    
        bool hasOrder(int orderId) const;
    
        bool empty() const;
    
        void cancelAll();
    };
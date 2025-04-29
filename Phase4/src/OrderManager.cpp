#include "../include/OrderManager.hpp"

template<typename PriceType, typename OrderIdType>
OrderManager<PriceType, OrderIdType>::OrderManager(OrderBook<PriceType, OrderIdType>& book)
    : order_book(book) {}

template<typename PriceType, typename OrderIdType>
void OrderManager<PriceType, OrderIdType>::submit_order(std::shared_ptr<Order<PriceType, OrderIdType>> order) {
    static_assert(std::is_integral<OrderIdType>::value, "Order ID must be integral");
    order_book.add_order(order);
}

template<typename PriceType, typename OrderIdType>
void OrderManager<PriceType, OrderIdType>::cancel_order(OrderIdType id) {
    order_book.remove_order(id);
}

// Explicitly instantiate
template class OrderManager<double, int>;

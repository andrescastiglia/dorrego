#pragma once

#define ORDER_SIZE sizeof(struct Order)
#define TRADE_SIZE sizeof(struct Trade)
#define ORDERBOOK_SIZE (ORDER_SIZE * 1024)

namespace dorrego
{
    using namespace boost::interprocess;
    using namespace boost::multi_index;

    enum Side
    {
        Buy,
        Sell,
    };

    struct Order : Request
    {
        Order() = default;
        Order(ID _id, ID _account, CURRENCY _currency, AMOUNT _price, AMOUNT _amount, Side _side) : id(_id), account(_account), currency(_currency), price(_price), amount(_amount), side(_side)
        {
        }
        ID id;
        ID account;
        CURRENCY currency;
        AMOUNT price;
        AMOUNT amount;
        Side side;
    };

    struct Trade : Response
    {
        Trade() = default;
        ID order_id_buyer;
        ID order_id_seller;
    };

    typedef boost::multi_index_container<
        Order,
        indexed_by<
            ordered_unique<tag<struct by_id>, member<Order, ID, &Order::id>>,
            ordered_non_unique<tag<struct by_price>, member<Order, AMOUNT, &Order::price>>>,
        allocator<Order, managed_shared_memory::segment_manager>>
        OrderbookContainer;

    class Orderbook : public Book
    {
    private:
        managed_shared_memory segment;
        OrderbookContainer *orderbook[MAX_CURRENCY][SIDES];

    public:
        Orderbook();
        virtual ~Orderbook() = default;

        static bool destroy();

        bool add(Order &_order);
        bool remove(ID _id, CURRENCY _currency, Side _side);
        bool match(Order &_order, ID &_id);
    };
}
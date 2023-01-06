#pragma once

using boost::interprocess::interprocess_mutex;

class Command
{
private:
    interprocess_mutex mutex;

    Orderbook &orderbook;
    Order &order;
    Trade &trade;

public:
    Command(Orderbook &_orderbook, Order &_order, Trade &_trade);
    virtual ~Command() = default;

    void execute();
};
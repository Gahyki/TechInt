#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct Order
{
    bool is_buy = false;
    int qty = 0;
    double price = 0.0;
};

ostream& operator <<(ostream& os, const Order& order)
{
    return os << (order.is_buy ? "buy " : "sell ")
        << order.qty << "@$" << setprecision(1)
        << fixed << order.price;
}

bool operator <(const Order& a, const Order& b)
{
    return a.price < b.price;
};

class OrderBook {
public:
    OrderBook() = default;

    // formats and prints the orders as the test cases expect.
    ~OrderBook() {
        auto itr = splitIntoBuyAndSellOrders();
        std::sort(begin(orders), itr);
        std::sort(itr, end(orders));
        print();
    }

    // checks the opposing side's available orders.
    // for a buy order, look at existing sell orders, and vice versa.
    // if a trade is possible, update the order book accordingly.
    // otherwise, insert the order into the book.
    void add(Order order);

    // print the order book
    // IMPORTANT: the test cases depend on this method.
    // please make sure this is the ONLY ouput that gets printed to stdout.
    void print();

private:
    // returns an iterator to the best "match" for a given order.
    // for buy orders, this would be the lowest sell price.
    // for sell orders, the highest buy price.
    // if no orders meet the criteria, return orders.end()
    vector<Order>::iterator findTrade(const Order& order);

    // split orders into buy and sell orders.
    // return an iterator to the first sell order.
    // all orders before the iterator are buy orders.
    // all orders after are sell orders.
    vector<Order>::iterator splitIntoBuyAndSellOrders();

    vector<Order> orders;
};

vector<Order>::iterator OrderBook::splitIntoBuyAndSellOrders() {
    return stable_partition(begin(orders),
        end(orders),
        [](const Order& order) { return order.is_buy; });
}

void OrderBook::print() {
    for (const auto& order : orders)
        cout << order << endl;
}


void OrderBook::add(Order order)
{
    auto other = findTrade(order);
    if (other == end(orders)) {
        for (int i = 0; i < orders.size(); i++) {
            if (order.is_buy == orders[i].is_buy && order.price == orders[i].price) {
                orders[i].qty += order.qty;
                return;
            }
        }
        if (order.is_buy) {
            orders.insert(orders.begin(), order);
        }
        else {
            orders.push_back(order);
        }
    }
    else {
        if (order.qty > other->qty) {
            order.qty -= other->qty;
            orders.erase(other);
            add(order);
        }
        else if (order.qty == other->qty) {
            orders.erase(other);
        }
        else {
            other->qty -= order.qty;
        }
    }
}

vector<Order>::iterator OrderBook::findTrade(const Order& order)
{
    auto ret = end(orders);

    size_t i = 0;
    double best = order.price;
    while (i < orders.size()) {
        if (order.is_buy != orders[i].is_buy) {

            if (best >= orders[i].price && order.is_buy) { //buying
                ret = begin(orders) + i;
                best = orders[i].price;
            }
            else if (best <= orders[i].price && !order.is_buy) { //selling
                ret = begin(orders) + i;
                best = orders[i].price;
            }
        }
        i++;
    }
    return ret;
}

void parse(OrderBook& book) {
    string word;
    while (true) {
        cin >> word;
        if (word == "end")
            break;

        Order* order = new Order;
        order->is_buy = word == "buy";
        cin >> order->qty;
        cin >> order->price;
        book.add(*order);
    }
}
int main() {
    OrderBook book;
    parse(book);
    Order order = { "buy", 10, 11.0 };
    book.add(order);
}
class Order:
    def __init__(self, is_buy, qty, price):
        self.is_buy = is_buy
        self.qty = qty
        self.price = price

    def __repr__(self):
        return '{} {}@${:.1f}'.format(
            'buy' if self.is_buy else 'sell',
            self.qty,
            self.price)
    
    def __gt__(self, other):
        return self.price > other.price

class OrderBook:
    def __init__(self):
        self._orders = []

    def __enter__(self):
        return self

    def __exit__(self, *args):
        '''
        formats and prints the order book as the test cases expect
        '''
        buys, sells = self._split_into_buy_and_sell_orders()
        buys = sorted(buys)
        sells = sorted(sells)
        for o in [*buys, *sells]:
            print(o)

    def _split_into_buy_and_sell_orders(self):
        '''
        splits orders into buy and sell orders.
        returns a pair of iterables:
        first iterable points to the first buy order.
        second points to the first sell order.
        '''
        from itertools import tee, filterfalse
        is_buy = lambda o: o.is_buy
        buys, sells = tee(self._orders)
        return filter(is_buy, buys), filterfalse(is_buy, sells)


    def add(self, order):
        '''
        checks the opposing side's available orders.
        for a buy order, look at existing sell orders, and vice versa.
        if a trade is possible, update the order book accordingly.
        otherwise, insert the order into the book.
        '''
        other = self._find_trade(order)
        if other:
            obuy = other.is_buy
            oqty = other.qty
            self._orders.remove(other)
            if order.qty > oqty: #order more than trade
                self.add(Order(order.is_buy, int(order.qty - oqty), float(order.price)))
            elif order.qty < oqty:
                self.add(Order(obuy, int(oqty - order.qty), float(order.price)))


        else:
            i = 0
            while i < len(self._orders):
                if order.price == self._orders[i].price:
                    order = Order(order.is_buy, int(order.qty + self._orders[i].qty), float(order.price))
                    self._orders.remove(self._orders[i])
                i += 1
            self._orders.append(order)

    def _find_trade(self, order):
        '''
        returns an order for the best "match" for a give order.
        for buy orders, this would be the lowest sell price.
        for sell orders,the highest buy price.
        if no orders meet the criteria, return None.
        '''
        ret = None
        i = 0
                
        while i < len(self._orders):
            if order.is_buy != self._orders[i].is_buy: #check if its buy and sell
                if order.price >= self._orders[i].price and order.is_buy == True: #buying
                    #if order.qty > self._orders[i].qty:
                    ret = self._orders[i]
                elif order.price <= self._orders[i].price and order.is_buy == False: #selling
                    ret = self._orders[i]
                else:
                    pass
            i += 1
        return ret
            
def parse(order_book = OrderBook()):
    while True:
        line = input().strip().split()
        
        if line[0] == 'end':
            break
        
        is_buy = line[0] == 'buy'
        qty, price = line[1:]
        order_book.add(Order(is_buy, int(qty), float(price)))

with OrderBook() as order_book:
    parse(order_book)
    order_book.add(Order(True, 10, 11.0))


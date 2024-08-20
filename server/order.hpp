#include <string>
#include <cstdint>

typedef double Price;
typedef uint64_t Quantity;
typedef std::string ID;
typedef std::string Ticker;

enum Side {
    BID,
    ASK
};

enum OrderType {
    ORDER,
    LIMIT,
    STOP,
    STOP_LIMIT
};

enum OrderCondition {
    NONE = 0,
    AON = 1,
    IOC = AON << 1,
    FOK = AON | IOC
};

struct Order {
    Ticker ticker;
    ID id;
    Price price;
    Price stop = -1;
    Quantity quantity;
    Side side;
    OrderType orderType = ORDER;
    OrderCondition condtion = NONE;
};

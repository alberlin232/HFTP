#include <iostream>
#include "book.hpp"

int main() {
    Book book;
    book.insertAsk({"APPL", "ABE", 100.0, 10});
    book.insertBid({"APPLE", "ABE", 100.0, 9});
    book.printAsk();
}


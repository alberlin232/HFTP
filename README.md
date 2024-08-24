## High Frequency Trading Platform (HFTP)

Currently, HFTP is a FIFO order matching engine that can do: market, limit, stop, and stop limit orders (single-threaded)

### Build Instructions
1. Create build directory
```
mkdir build && cd build
cmake ..
```
2. Build and run (from project dir)
```
cmake -build ./build
./build/server
```

### Features
Soon to come features:
- Order conditions: AON, IOC, FOK
- Cancel orders
- Return order state


Further plans:
- Make multi-threaded
- Build out FIX protocal
- Build client

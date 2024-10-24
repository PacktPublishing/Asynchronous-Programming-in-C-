#include <iostream>
#include <boost/cobalt.hpp>
#include <boost/asio.hpp>

boost::cobalt::promise<void> producer(boost::cobalt::channel<int>& ch) {
    for (int i = 1; i <= 10; ++i) {
        std::cout << "Producer waiting for request\n";
        co_await ch.write(i);
        std::cout << "Producing value " << i << std::endl;
    }
    std::cout << "Producer end\n";
    ch.close();
    co_return;
}

boost::cobalt::main co_main(int, char*[]) {
    boost::cobalt::channel<int> ch;

    auto p = producer(ch);
    while (ch.is_open()) {
        std::cout << "Consumer waiting for next number \n";
        std::this_thread::sleep_for(std::chrono::seconds(5));
        auto n = co_await ch.read();
        std::cout << "Consuming value " << n << std::endl;
        std::cout << n * n << std::endl;
    }

    co_await p;
    co_return 0;
}

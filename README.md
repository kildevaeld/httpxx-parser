# Usage


```c++

#include <httpxx_parser/parser.hpp>

using namespace httpxx_parser;

Parser parser(HTTPResponse);

parser.once<HeaderEvent>([](const auto &e, auto &) {
    std::cout << "status: " << e.status << "\nHeaders:\n";
    for (const auto &h: e.header) {
        std::cout "Field: " << h.first << ", Value: " << h.second << "\n"; 
    }
    std::cout << std::endl;
});

parser.once<EndEvent>([](const auto &e, auto &) {
    std::cout << "parsed done" << std::endl;
});

parser.on<DataEvent>([](const auto &e, auto &) {
    std::cout << e.data << std::endl;
});

auto i = parser.execute("HTTP/1.1 200 OK\r\n"
                        "Content-Length: 13\r\n"
                        "Content-Type: plain/text\r\n\r\n"
                        "Hello, World!");


```
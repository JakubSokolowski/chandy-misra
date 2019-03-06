#include "ThreadSafePrinter.h"
#include <iostream>

std::mutex ThreadSafePrinter::cout_lock_m;

void ThreadSafePrinter::Print(std::string const &text) {
    std::lock_guard<std::mutex> lock_guard(cout_lock_m);
    std::cout << text << std::endl;
}

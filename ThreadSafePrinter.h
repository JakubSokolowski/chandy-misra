#ifndef DININGPHILOSOPHERS_THREADSAFEPRINTER_H
#define DININGPHILOSOPHERS_THREADSAFEPRINTER_H

#include <mutex>

class ThreadSafePrinter {
public:
    static void Print(std::string const &text);
private:
    static std::mutex cout_lock_m;
};


#endif //DININGPHILOSOPHERS_THREADSAFEPRINTER_H

#ifndef DININGPHILOSOPHERS_COORDINATOR_H
#define DININGPHILOSOPHERS_COORDINATOR_H

#include <mutex>
#include <condition_variable>
#include <atomic>

// Wrapper around mutex + condition_variable
class Coordinator {

public:
    void Wait();
    void NotifyAll();

private:
    std::mutex mutex;
    // Blocks one or more threads, until another notifies it
    std::condition_variable cv;
};

// Allows communication between Philosopher threads, table, and visualisation
struct TableChannel {
    Coordinator coordinator;
    std::atomic<bool> dinner_finished{false};
    std::atomic<uint> finished_count{0};
    int max_cycle{0};
};

#endif //DININGPHILOSOPHERS_COORDINATOR_H

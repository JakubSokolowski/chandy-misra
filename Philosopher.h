#ifndef DININGPHILOSOPHERS_PHILOSOPHER_H
#define DININGPHILOSOPHERS_PHILOSOPHER_H

#include <thread>
#include <random>
#include <string>
#include "Fork.h"
#include "Coordinator.h"
#include "ThreadSafePrinter.h"
#include "Visualisation.h"

enum Status{
    THINKING,
    EATING,
    REQUESTING,
    HAS_RIGHT_FORK,
    HAS_LEFT_FORK,
    FINISHED
};

class Philosopher {
public:
    Philosopher(int id, TableChannel &table, Fork &left, Fork &right, Visualisation &sandbox);
    Philosopher(Philosopher&& other) noexcept ;
    ~Philosopher();

    void Dine();

    std::string GetId();
    std::string GetStatus();

private:
    int id_m = -1;
    TableChannel &table_m;
    Fork &left_fork_m;
    Fork &right_fork_m;
    std::thread life_thread_m;

    Visualisation &sandbox;
    int cycle_count_m = 0;
    std::mt19937 rng_m{ std::random_device{}()};
    Status status = Status::THINKING;

    void Think();
    void Eat();
    void RequestForks();
    void FreeForks();
    void WaitRandom(int low, int up);
    void SetStatus(Status st);

    bool IsFinished();
    void Cleanup();
};

#endif //DININGPHILOSOPHERS_PHILOSOPHER_H

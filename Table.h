
#ifndef DININGPHILOSOPHERS_TABLE_H
#define DININGPHILOSOPHERS_TABLE_H

#include <deque>
#include "Philosopher.h"
#include "Visualisation.h"

enum  StopOption {
    TIME_ELAPSED,
    MAX_CYCLE,
    KEY_PRESSED,
};
using StopCondition = std::pair<StopOption , int>;

class Table {
public:
    Table(uint p_num, StopCondition condition);

    void StartDinner();
    std::vector<std::string> GetPhilStatus();
    std::vector<std::string> GetPhilIds();

    int philosophers_num;
    TableChannel channel;
    Visualisation vis;

private:
    StopCondition condition_m;
    uint finished_count_m;

    std::deque<Philosopher> philosophers_m;
    std::deque<Fork> forks_m;

    void Start();
    void Stop();
    void CreateForks(uint fork_num);
    void CreatePhilosophers(uint phil_num);
};

#endif //DININGPHILOSOPHERS_TABLE_H

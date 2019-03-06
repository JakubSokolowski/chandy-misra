#include "Table.h"


Table::Table(uint p_num, StopCondition condition)
        : philosophers_num(p_num)
        , vis(channel)
        , condition_m(std::move(condition))
        , finished_count_m(0)
{
    CreateForks(p_num);
    CreatePhilosophers(p_num);
    vis.UpdateTableInfo(GetPhilIds(), GetPhilStatus());
}

// Starts dinner and sets comm channel in according to stop condition
void Table::StartDinner() {
    switch (condition_m.first) {
        case StopOption::TIME_ELAPSED:
            Start();
            std::this_thread::sleep_for(std::chrono::seconds(condition_m.second));
            break;
        case StopOption::MAX_CYCLE:
            channel.max_cycle = condition_m.second;
            Start();
            break;
        case StopOption::KEY_PRESSED:
            channel.max_cycle = -1;
            Start();
            break;
    }
}

// Notifies other threads that the dinner is started. Starts the visualisation
void Table::Start() {
    channel.coordinator.NotifyAll();
    vis.Start();
    Stop();
}

void Table::Stop() {
    channel.dinner_finished = true;
}


void Table::CreateForks(uint fork_num) {
    for(uint i = 0; i < fork_num; i++) {
        if(i == fork_num - 1)
            forks_m.emplace_back(i, 0);
        else
            forks_m.emplace_back(i, i);
    }
}

void Table::CreatePhilosophers(uint phil_num) {
    for(uint i = 0; i < phil_num; i++) {
        if (i == phil_num - 1)
            philosophers_m.emplace_back(i, channel, forks_m[i], forks_m[0], vis);
        else
            philosophers_m.emplace_back(i, channel, forks_m[i], forks_m[i+1], vis);
    }
}


std::vector<std::string> Table::GetPhilStatus() {
    auto out = std::vector<std::string>();
    for(auto &phil : philosophers_m)
        out.emplace_back(phil.GetStatus());
    // Items in ncurses menu need to be null terminated
    out.emplace_back("\0");
    return out;
}

std::vector<std::string> Table::GetPhilIds() {
    auto out = std::vector<std::string>();
    for(auto &phil : philosophers_m)
        out.emplace_back(phil.GetId());
    out.emplace_back("\0");
    return out;
}

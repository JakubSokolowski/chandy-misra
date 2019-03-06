#include <iostream>
#include "Philosopher.h"


Philosopher::Philosopher(int id, TableChannel &table, Fork &left, Fork &right, Visualisation &box)
        : id_m(id)
        , table_m(table)
        , left_fork_m(left)
        , right_fork_m(right)
        , life_thread_m(&Philosopher::Dine, this)
        , sandbox(box)
{}

Philosopher::Philosopher(Philosopher && other) noexcept
        : id_m(other.id_m)
        , table_m(other.table_m)
        , left_fork_m(other.left_fork_m)
        , right_fork_m(other.right_fork_m)
        , life_thread_m(std::move(other.life_thread_m))
        , sandbox(other.sandbox)
{}

Philosopher::~Philosopher() {
    if(life_thread_m.joinable())
        life_thread_m.detach();
}


void Philosopher::Dine() {
    table_m.coordinator.Wait();
    for(;;) {
        Think();
        Eat();
        if(IsFinished()) {
            Cleanup();
            break;
        }
    }

}

void Philosopher::Think() {
    for(int i = 0; i < 10 ; i++) {
        WaitRandom(200,300);
        SetStatus(Status::THINKING);
    }
}

void Philosopher::Eat() {

    // Request Forks, and wait for them to become free
    RequestForks();

    // Lock them. using std::lock_guard unlocks mutex on destruction
    std::lock(left_fork_m.GetMutex(), right_fork_m.GetMutex());
    std::lock_guard<std::mutex> left_lock(left_fork_m.GetMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> right_lock(right_fork_m.GetMutex(), std::adopt_lock);

    left_fork_m.SetStatus(ForkStatus::DIRTY);
    right_fork_m.SetStatus(ForkStatus::DIRTY);
    // Forks retrieved, start eating

    for(int i = 0; i < 10 ; i++) {
        WaitRandom(200,300);
        SetStatus(Status::EATING);
    }

    // Communicate to others that the fork is free
    FreeForks();
    cycle_count_m++;
}

void Philosopher::RequestForks() {
    SetStatus(Status::REQUESTING);
    static thread_local std::uniform_int_distribution<> dist(1, 2);
    if(dist(rng_m) == 1) {
        left_fork_m.Request(id_m);
        SetStatus(Status::HAS_LEFT_FORK);
        right_fork_m.Request(id_m);
    }
    else {
        right_fork_m.Request(id_m);
        SetStatus(Status::HAS_RIGHT_FORK);
        left_fork_m.Request(id_m);
    }
//    left_fork_m.Request(id_m);
//    SetStatus(Status::HAS_LEFT_FORK);
//    right_fork_m.Request(id_m);
}

void Philosopher::FreeForks() {
    left_fork_m.Free();
    right_fork_m.Free();
    SetStatus(Status::THINKING);
    left_fork_m.SetStatus(ForkStatus::CLEAN);
    right_fork_m.SetStatus(ForkStatus::CLEAN);
}

void Philosopher::WaitRandom(int low, int up) {
    static thread_local std::uniform_int_distribution<> dist(low, up);
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng_m)));
}


std::string Philosopher::GetId() {
    return "P" + std::to_string(id_m) +  " " + std::to_string(cycle_count_m) + ": ";
}

std::string Philosopher::GetStatus() {
    auto fork_status = left_fork_m.GetId() + " " + left_fork_m.GetStatus();
    std::string msg;
    switch(status){
        case Status::THINKING:
            msg = "Thinking             ";
            break;
        case Status::REQUESTING:
            msg = "Requesting fork      ";
            break;
        case Status::HAS_RIGHT_FORK:
            msg = "Has RF, requesting LF";
            break;
        case Status::HAS_LEFT_FORK:
            msg = "Has LF, requesting RF";
            break;
        case Status::EATING:
            msg = "Eating               ";
            break;
        case Status::FINISHED:
            msg = "Finished             ";
            break;
    }
    return msg + " "  + fork_status;
}

void Philosopher::SetStatus(Status st) {
    status = st;
    sandbox.UpdatePhilInfo(id_m, GetId(), GetStatus());
}


bool Philosopher::IsFinished() {
    return cycle_count_m == table_m.max_cycle || table_m.dinner_finished;
}

void Philosopher::Cleanup() {
    table_m.finished_count++;
    FreeForks();
    SetStatus(Status::FINISHED);
    if(life_thread_m.joinable()) {
        life_thread_m.detach();
    }
}










#include "Fork.h"

Fork::Fork(int fork_id, int owner_id)
        : id_m(fork_id)
        , owner_id_m(owner_id)
        , dirty_m(true)
{}

Fork::Fork(const Fork &other)
        : id_m(other.id_m)
        , owner_id_m(other.owner_id_m)
        , dirty_m(other.dirty_m)
{}

// Transfer the ownership of fork to philosopher specified by id.
// If someone is using the fork (fork is clean), the requesting
// philosopher thread will be blocked, until the fork is no longer used (dirty)
void Fork::Request(int phil_id) {
    while (owner_id_m != phil_id) {
        if (dirty_m) {
            std::lock_guard<std::mutex> lock(mutex_m);
            dirty_m = false;
            status = ForkStatus::CLEAN;
            owner_id_m = phil_id;
        } else {
            channel_m.Wait();
        }
    }
}

// Make the fork available for others. Notify the waiting threads
void Fork::Free() {
    dirty_m = true;
    channel_m.NotifyAll();
}

std::string Fork::GetStatus() {
    std::string msg;
    switch(status) {
        case ForkStatus::DIRTY:
            msg = "Dirty";
            break;
        case ForkStatus::CLEAN:
            msg = "Clean";
            break;
    }
    return "Owner P" + std::to_string(owner_id_m) + ": " + msg;
}

std::string Fork::GetId() {
    return "F" + std::to_string(id_m);
}

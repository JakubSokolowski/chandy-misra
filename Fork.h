#ifndef DININGPHILOSOPHERS_FORK_H
#define DININGPHILOSOPHERS_FORK_H

#include <mutex>
#include "Coordinator.h"

enum ForkStatus {
    DIRTY,
    CLEAN,
};

class Fork {
public:
    Fork(int fork_id, int owner_id);
    Fork(const Fork& other);

    void Request(int phil_id);
    void Free();
    std::mutex& GetMutex() { return mutex_m; }
    std::string GetStatus();
    std::string GetId();
    void SetStatus(ForkStatus stat) { status = stat;}

private:
    int id_m;
    int owner_id_m;
    bool dirty_m;

    std::mutex mutex_m;
    Coordinator channel_m;

    ForkStatus status = ForkStatus::DIRTY;

};

#endif //DININGPHILOSOPHERS_FORK_H

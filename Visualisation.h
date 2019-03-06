#ifndef CHANDY_MISRA_VISUALISATION_H
#define CHANDY_MISRA_VISUALISATION_H

#include "menu.h"
#include <ncurses.h>
#include <vector>
#include <string>
#include <memory>
#include <cstdio>
#include <string>
#include <cstring>
#include <cstdlib>
#include <mutex>
#include <atomic>
#include "Coordinator.h"

using std::string;
using std::vector;

class Visualisation {

public:
    explicit Visualisation(TableChannel &channel);
    ~Visualisation();

    void Start();
    void UpdateTableInfo(vector<string> id_vec, vector<string> state_vec);
    void UpdatePhilInfo(int id, string name, string desc);

private:

    std::mutex window_lock_m;
    std::vector<std::string> phil_ids_m;
    std::vector<std::string> phil_states_m;

    TableChannel &channel_m;
    ITEM **phil_items = nullptr;
    MENU * phil_menu = nullptr;
    WINDOW* phil_menu_win = nullptr;

    int lines, cols;

    void Setup();
    void Teardown();
    void CreatePhilMenu();

    void SendKillSignal() { channel_m.dinner_finished = true;}
};


#endif // CHANDY_MISRA_VISUALISATION_H

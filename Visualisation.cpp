#include "Visualisation.h"

using std::string;

Visualisation::~Visualisation() {
    wrefresh(phil_menu_win);
    Teardown();
}

void Visualisation::UpdateTableInfo(std::vector<std::string> id_vec, std::vector<std::string> state_vec) {
    phil_ids_m = id_vec;
    phil_states_m = std::move(state_vec);
    phil_items = new ITEM*[id_vec.size()];
    auto num = phil_ids_m.size();
    for(uint i = 0; i < num; ++i)
        phil_items[i] = new_item(phil_ids_m[i].c_str(), phil_states_m[i].c_str());

    Setup();
    CreatePhilMenu();
}

void Visualisation::Start() {
    int option;
    bool finished = false;

    while((option = wgetch(phil_menu_win)) && !finished )
    {
        std::lock_guard<std::mutex> lock_guard(window_lock_m);
        switch(option)
        {
            case KEY_DOWN:
                menu_driver(phil_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(phil_menu, REQ_UP_ITEM);
                break;
            case KEY_NPAGE:
                menu_driver(phil_menu, REQ_SCR_DPAGE);
                break;
            case KEY_PPAGE:
                menu_driver(phil_menu, REQ_SCR_UPAGE);
                break;
            case 113:
                SendKillSignal();
                finished = true;
                break;
            default:
                break;
        }
        wrefresh(phil_menu_win);
    }
}

void Visualisation::Setup() {
    /* Initialize curses */
    initscr();
    getmaxyx(stdscr, lines, cols);
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, true);
    use_default_colors();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
}

void Visualisation::CreatePhilMenu() {
    /* Crate menu */
    phil_menu = new_menu(phil_items);
    /* Create the window to be associated with the menu */
    // lines, cols, begin y, begin x
    phil_menu_win = newwin(lines - 5, cols - 10, 1, 4);
    keypad(phil_menu_win, true);

    /* Set main window and sub window */
    set_menu_win(phil_menu, phil_menu_win);
    set_menu_sub(phil_menu, derwin(phil_menu_win, lines - 10, cols - 15, 3, 1));
    // rows,cols
    set_menu_format(phil_menu, lines - 10, 1);

    /* Set menu mark to the string " * " */
    set_menu_mark(phil_menu, "");

    /* Print a border around the main window and print a title */
    box(phil_menu_win, 0, 0);
    mvwprintw(phil_menu_win, 1, 4, "%s", "Philosophers");
    mvwprintw(phil_menu_win, 1, (cols/2) - 4, "%s", "Forks");
    refresh();
    mvwaddch(phil_menu_win, 2, 0, ACS_LTEE);
    mvwhline(phil_menu_win, 2, 1, ACS_HLINE, cols - 10);
    mvwaddch(phil_menu_win, 2, cols - 11, ACS_RTEE);

    /* Post the menu */
    post_menu(phil_menu);
    wrefresh(phil_menu_win);

    mvprintw(LINES - 3, 4, "Use PageUp and PageDown, and Arrow Keys to scroll");
    mvprintw(LINES - 2, 4, "Q to stop Philosopher threads");
    refresh();
}

void Visualisation::Teardown() {
    // Unpost and free all the memory taken up
    unpost_menu(phil_menu);
    free_menu(phil_menu);
    for(uint i = 0; i < phil_ids_m.size(); ++i)
        free_item(phil_items[i]);
    endwin();
}

void Visualisation::UpdatePhilInfo(int id, string name, string desc) {
    std::lock_guard<std::mutex> lock_guard(window_lock_m);
    phil_ids_m[id] = std::move(name);
    phil_states_m[id] = std::move(desc);
    phil_items[id] = new_item(phil_ids_m[id].c_str(), phil_states_m[id].c_str());

    // Prevent cursor from moving to the top of the menu
    auto current = phil_menu->curitem;
    unpost_menu(phil_menu);
    set_menu_items(phil_menu,phil_items);
    set_current_item(phil_menu, current);
    post_menu(phil_menu);
    wrefresh(phil_menu_win);
}

Visualisation::Visualisation(TableChannel &channel)
        : channel_m(channel)
{}








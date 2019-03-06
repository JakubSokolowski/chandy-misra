#include <iostream>
#include "Table.h"

int main(int argc, char* argv[]) {
    {
        int num;
        try{
           num = (uint)std::stoi(argv[1]);
            if(num <= 1)
                num = 5;
        }
        catch(...) {
            num = 5;
        }

        Table table((uint)num, std::make_pair(StopOption::KEY_PRESSED, 1000));
        table.StartDinner();
        std::cout<<"Main Finished!\n";
    }
}

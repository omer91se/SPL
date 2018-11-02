#include "../include/Restaurant.h"
#include <ifstream>

using namespace std;

//Empty constructor.
Restaurant(){
    open = false;
    tables = new vector<Table*>;
    menu = new vector<Dish>;
    actionsLog = new vector<BaceAction*>;
}

Restaurant(const std::string &configFilePath){
    
}


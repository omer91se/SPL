/**
*@Authors: Anat Bar-Sinai, Omer Segal.
*@version: Last Update: 8.11.2018
*/

#include "../include/Restaurant.h"
#include <fstream>

using namespace std;


//Empty constructor.
Restaurant::Restaurant(): open(false), tables(), menu(), actionsLog(){}


/*Constructor.
*Input: Path to the config file.
*/
Restaurant::Restaurant(const string &configFilePath): open(false){

    ifstream config_file(configFilePath);
    string line;

    if (config_file.is_open()) {
        int counter = 0;

        while (getline(config_file, line)) {
            if (line.size() > 0 && line[0] != '#') {

                //Assign number of tables
                if (counter == 1) {
                    int num_of_t = stoi(line, nullptr);
                    tables = vector<Table *>(num_of_t);
                }

                //Capacity of tables
                if (counter == 2) {
                    size_t pos = 0;
                    int table_cap = 0;

                    //Initialize tables by size
                    while ((pos = line.find(',')) != std::string::npos) {
                        table_cap = stoi(line.substr(0, pos), nullptr);
                        cout << table_cap << endl; //For test
                        Table *table = new Table(table_cap);

                        tables.push_back(table);
                        line.erase(0, pos + 1);
                    }
                }

                //Initialize menu
                if (counter >= 3) {
                    int dish_id = 0;
                    int price = 0;
                    int pos = 0;
                    string dish_name = NULL;
                    DishType dish_type;

                    pos = line.find(',');
                    dish_name = line.substr(0, pos);
                    line.erase(0, pos + 1);

                    pos = line.find(',');
                    dish_type = str_to_DishTp(line.substr(0, pos));
                    line.erase(0, pos + 1);

                    pos = line.find(',');
                    price = stoi(line, nullptr);
                    Dish dish = Dish(dish_id, dish_name, price, dish_type);
                    dish_id++;
                    this->menu.push_back(dish);

                }
                counter++;
            }
        }
    }
    else cout << "Unable to open file";

    config_file.close();
}

/**
 * Destructor
 */
Restaurant::~Restaurant(){
   clean();
}

/**
 *
 * @param other
 * @return this
 */
Restaurant& Restaurant::operator=(const Restaurant& other){
    if(this != &other){
        clean();
        copy(other);
    }
    return *this;
}

//TODO: check if open == true or same as other.open.
Restaurant::Restaurant(const Restaurant& other): open(false){
    this->tables = vector<Table*>(other.tables.size());
    this->actionsLog = vector<BaseAction*>(other.actionsLog.size());
    this->menu = vector<Dish>(other.menu.size());
    copy(other);
}

void Restaurant::start() {
    cout << "Restaurant is now open!" << endl;
    this->open = true;
}

int Restaurant::getNumOfTables() const {
    return this->tables.size();
}

Table *Restaurant::getTable(int ind) {
    if (ind < getNumOfTables() && tables[ind] != nullptr)
        return tables[ind];

    else {
        cout << "A table with that index does not exist." << endl;
        return nullptr;
    }

}

const std::vector<BaseAction *>& Restaurant::getActionsLog() const {
    return this->actionsLog;
}

std::vector<Dish>& Restaurant::getMenu() {
    return this->menu;
}

/*
 * Cast from string to DishType
 * Input: dish type in string.
 * Output: DishType.
 */
DishType Restaurant::str_to_DishTp(string str) {
    DishType dish_type;
    if(str == "VEG")
        dish_type = VEG;
    else if(str == "SPC")
        dish_type = SPC;
    else if(str == "BVG")
        dish_type = BVG;
    else
        dish_type = ALC;
    return dish_type;

}

/**
 * Deletes all the memory that has been allocated.
 */
void Restaurant::clean(){
    for (vector<Table *>::iterator table_it = tables.begin(); table_it != tables.end(); ++table_it){
        Table* table = *table_it;
        tables.erase(table_it);
        delete(table);
    }
    for (vector<BaseAction*>::iterator action_it = actionsLog.begin(); action_it != actionsLog.end(); ++action_it){
        BaseAction* BA = *action_it;
        actionsLog.erase(action_it);
        delete(BA);
    }
}
/**
 * Deep copy.
 * @param other.
 */
void Restaurant::copy(const Restaurant& other) {
    for (auto table_it = other.tables.begin(); table_it != other.tables.end(); ++table_it) {
        this->tables.push_back(*table_it);
    }
    for (auto actions_it = other.actionsLog.begin(); actions_it != other.actionsLog.end(); ++actions_it) {
        this->actionsLog.push_back(*actions_it);
    }
}
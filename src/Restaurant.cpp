/*
*Authors: Anat Bar-Sinai, Omer Segal.
*Last Update: 7.11.2018
*/

#include "../include/Restaurant.h"
#include <fstream>

using namespace std;

DishType str_to_DishTp(string str);

//Empty constructor.
Restaurant::Restaurant(): open(false), tables(), menu(), actionsLog(){}

//TODO Init actionlogs.OMER
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
                    tables = new vector<Table *>(num_of_t);
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
DishType str_to_DishTp(string str) {
    DishType dish_type;
    if(str == "VEG")
        dish_type = VEG;
    else if(str == "SPC")
        dish_type = BVG;
    else if(str == "BVG")
        dish_type = BVG;
    else
        dish_type = ALC;
    return dish_type;

}








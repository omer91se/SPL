/**
*@Authors: Anat Bar-Sinai, Omer Segal.
*@version: Last Update: 18.11.2018
*/

#include "../include/Restaurant.h"
#include "../include/Customer.h"
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

Restaurant &Restaurant::operator=(Restaurant &&other) {
    if(this != &other){
        clean();
        steal(other);
    }
    return *this;
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

Restaurant::Restaurant(const Restaurant& other):tables(vector<Table*>(other.tables.size())),
                                                actionsLog(vector<BaseAction*>(other.actionsLog.size())),
                                                menu(vector<Dish>())
                                                //other.menu.size()
{
    copy(other);
}

Restaurant::Restaurant(Restaurant &&other): menu(other.menu) {
    steal(other);
}

/**
 * Destructor
 */
Restaurant::~Restaurant(){
   clean();
    cout<< "Restaurant Dest"<<endl;
}

//TODO Add actions to actionLog.
void Restaurant::start() {
    cout << "Restaurant is now open!" << endl;
    this->open = true;
    int cust_count = 0;
    string input = "";
    string action = "";
    string args = "";

    while (action != "closeall"){
        cin >> input;
        action = input.substr(0,input.find(' '));
        args = input.substr(input.find(' ')+1,input.size());

            if (action == "open"){
                int id = stoi(input.substr(0,input.find(' ')),nullptr);
                input = input.substr(input.find(' ')+1,input.size());
                vector<Customer*> customers;
                string name;
                string type;
                while(input != ""){
                    name = args.substr(0,input.find(','));
                    type = args.substr(input.find(',')+1,input.find(' '));

                        string str_status;
                        if (type == "veg"){
                            Customer *cust = new VegetarianCustomer(name, cust_count);
                            cust_count++;
                            customers.push_back(cust);
                        }

                        if (type == "chp"){
                            Customer *cust = new CheapCustomer(name, cust_count);
                            cust_count++;
                            customers.push_back(cust);
                        }
                        if (type == "spc"){
                            Customer *cust = new SpicyCustomer(name, cust_count);
                            cust_count++;
                            customers.push_back(cust);}

                        if (type == "alc"){
                            Customer *cust = new AlchoholicCustomer(name, cust_count);
                            cust_count++;
                            customers.push_back(cust);
                        }
                        args = args.substr(input.find(' ')+1,input.size());
                }
                BaseAction *open_table = new OpenTable(id,customers);
                open_table->act(*this);
                this->actionsLog.push_back(open_table);
            }

            if (action == "order"){
                int id = stoi(args);
                BaseAction *order = new Order(id);
                order->act(*this);
                this->actionsLog.push_back(order);
            }

            if (action == "move"){
                int src = stoi(args.substr(0,args.find(' ')));
                args = args.substr(0,args.find(' ')+1);
                int dst = stoi(args.substr(0,args.find(' ')));
                args = args.substr(0,args.find(' ')+1);
                int id = stoi(args.substr(0,args.find(' ')));

                BaseAction *move = new MoveCustomer(src,dst,id);
                move->act(*this);
                this->actionsLog.push_back(move);
            }
             if (action == "close"){
                 int id = stoi(args);
                 BaseAction *close = new Close(id);
                 close->act(*this);
                 this->actionsLog.push_back(close);
             }

            if (action == "closeall"){
                CloseAll close;
                close.act(*this);
            }
            if (action == "menu"){
                BaseAction *print = new PrintMenu;
                print->act(*this);
                this->actionsLog.push_back(print);
            }
            if (action == "status"){
                int id = stoi(args);
                BaseAction *print = new PrintTableStatus(id);
                print->act(*this);
                this->actionsLog.push_back(print);
            }
            if (action == "log"){
                BaseAction *log = new PrintActionsLog;
                log->act(*this);
                this->actionsLog.push_back(log);
            }

            if (action == "backup"){
                BaseAction *backup = new BackupRestaurant;
                backup->act(*this);
                this->actionsLog.push_back(backup);

            }
            if (action == "restore"){
                BaseAction *restore = new RestoreResturant;
                restore->act(*this);
                this->actionsLog.push_back(restore);
            }


    }
}

int Restaurant::getNumOfTables() const {
    return this->tables.size();
}

Table *Restaurant::getTable(int ind){
    if (ind < getNumOfTables() && tables[ind] != nullptr)
        return tables[ind];

    else {
        return nullptr;
    }

}

const std::vector<BaseAction *>& Restaurant::getActionsLog() const {
    return this->actionsLog;
}

std::vector<Dish>& Restaurant::getMenu() const {
    return this->getMenu();
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
void Restaurant::copy(const Restaurant& other)  {
    for (vector<Table*>::const_iterator table_it = other.tables.begin(); table_it != other.tables.end(); ++table_it) {
        Table *table = new Table(*(*table_it));
        this->tables.push_back(table);
    }
    for (vector<BaseAction*>::const_iterator actions_it = other.getActionsLog().begin(); actions_it != other.actionsLog.end(); ++actions_it) {
        this->actionsLog.push_back((*actions_it)->clone());
    }

    for (vector<Dish>::const_iterator dish_it = other.getMenu().begin(); dish_it != other.getMenu().end(); ++dish_it) {
        this->menu.push_back(*dish_it);
    }

    this->open = other.open;
}

void Restaurant::steal(Restaurant &other) {
    this->open = other.open;

    for (vector<Dish>::const_iterator dish_it = other.getMenu().begin(); dish_it != other.getMenu().end(); ++dish_it)
        this->menu.push_back(*dish_it);

    this->actionsLog = other.actionsLog;
    this->tables = other.tables;

}

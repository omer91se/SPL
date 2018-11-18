/**
*@Authors: Anat Bar-Sinai, Omer Segal.
*@version: Last Update: 15.11.2018
*/

#include "../include/Action.h"
#include "../include/Restaurant.h"
#include <vector>

using namespace std;
extern Restaurant* backup;



BaseAction::BaseAction(): errorMsg(""),status(PENDING) {}

ActionStatus BaseAction::getStatus() const {
    return this->status;
}

void BaseAction::complete() {
    this->status = COMPLETED;
}

void BaseAction::error(std::string errorMsg) {
    this->status = ERROR;
    this->errorMsg = errorMsg;
}

std::string BaseAction::getErrorMsg() const {
    return this->errorMsg;
}


//OpenTable
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):BaseAction(), tableId(id), customers(customersList){}

/**
 * Opens the table and adds the customers.
 *
 * @param restaurant
 * Fails if there is no table with that ID or it is not open(sending an error otherwise)
 */
void OpenTable::act(Restaurant &restaurant) {
    Table *table = restaurant.getTable(this->tableId);
    if (table || table->isOpen() || customers.size() > (table->getCapacity())) {
        this->error("Table does not exist or is already open");
        return;
    }
    else {
        table->openTable();
        for (vector<Customer *>::iterator cust = customers.begin(); cust != customers.end(); ++cust)
            table->addCustomer(*cust);
    }
    complete();
}

OpenTable *OpenTable::clone() const {
    return new OpenTable(*this);
}

std::string OpenTable::toString() const {
    string output = "";
    output = "open " + tableId;
    for (vector<Customer *>::const_iterator cust = customers.begin(); cust != customers.end(); ++cust)
        output += " " + (*cust)->getName() + "," + (*cust)->toString();

    output += " " + status_to_string(getStatus());
    return output;
}

//Order
Order::Order(int id):BaseAction(), tableId(id)  {}

Order *Order::clone() const {
    return new Order(*this);
}

/**
 * Orders from each customer and prints the orders.
 *
 * @param restaurant
 * Fails if the table does not exist or is not open
 */
void Order::act(Restaurant &restaurant) {
    Table *table = restaurant.getTable(this->tableId);
    if(table || !table->isOpen()) {
        this->error("Table does not exist or is not open");
        return;
    }
    else{
        table->order(restaurant.getMenu());
        for (vector<OrderPair>::iterator pair = table->getOrders().begin(); pair != table->getOrders().end(); ++pair){
            Customer *cust = table->getCustomer(pair->first);
            Dish dish = pair->second;
            cout << cust->getName() << " ordered " << dish.getName() << endl;
        }
    }
    complete();
}

std::string Order::toString() const {

    string id = to_string(tableId);

    string output = "order " + id + " " + status_to_string(getStatus());
    return output;
}


MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction(), srcTable(src),dstTable(dst),id(customerId) {}

MoveCustomer *MoveCustomer::clone() const {
    return new MoveCustomer(*this);
}

void MoveCustomer::act(Restaurant &restaurant) {
    Table *src_table = restaurant.getTable(srcTable);
    Table *dst_table = restaurant.getTable(dstTable);
    bool is_customer = false;

    //Makes sure both tables exists.
    if(!src_table | !dst_table) {
        error("Cannot move customer");
        return;
    }

    //Makes sure both tables are open.
    if(!(src_table->isOpen()) | !(dst_table->isOpen())){
        error("Cannot move customer");
        return;
    }

    //Goes through the src table's customers list and checks if the customer (by id) is in the src table.
    for (vector<Customer*>::iterator cust = src_table->getCustomers().begin(); cust != src_table->getCustomers().end(); ++cust)
        if ((*cust)->getId() == id)
            is_customer = true;

    if(!is_customer){
        error("Cannot move customer");
        return;
    }

    //Makes sure the customer have a place in dst table.
    if(dst_table->getCustomers().size() == dst_table->getCapacity()){
        error("Cannot move customer");
        return;
    }

    //Moving the customer's orders.
    for (vector<OrderPair>::iterator pair = src_table->getOrders().begin(); pair != src_table->getOrders().end(); ++pair){
        if((*pair).first == id){
            dst_table->getOrders().push_back(*pair);
            src_table->getOrders().erase(pair);
        }
    }

    //Moving the customer itself.
    dst_table->addCustomer(src_table->getCustomer(id));
    src_table->removeCustomer(id);

    //close src table if necessary.
    if(src_table->getCustomers().size() == 0){
        src_table->closeTable();
    }
    complete();

}

std::string MoveCustomer::toString() const {
    string output = "move ";
    string static_dst = to_string(dstTable);
    string static_id = to_string(id);
    string static_src = to_string(srcTable);
    output = static_src + " " + static_dst + " " + static_id + status_to_string(getStatus());

    return output;
}

Close::Close(int id):BaseAction(), tableId(id) {}

Close *Close::clone() const {
    return new Close(*this);
}


void Close::act(Restaurant &restaurant) {
    Table *table = restaurant.getTable(tableId);
    int bill = 0;
    if(table || !(table->isOpen())){
        error("Table does not exist or is not open");
        return;
    }
    for (vector<OrderPair>::iterator pair = table->getOrders().begin(); pair != table->getOrders().end(); ++pair){
        bill += (*pair).second.getPrice();
    }
    table->closeTable();
    cout << "Table " << tableId << "was closed. Bill " << bill << "NIS" << endl;

    complete();
}

std::string Close::toString() const {
    string output = "close ";
    string static_id = to_string(tableId);
    string stat = status_to_string(getStatus());
    output += static_id + status_to_string(getStatus());

    return output;
}

CloseAll::CloseAll():BaseAction() {}

CloseAll *CloseAll::clone() const {
    return new CloseAll(*this);
}

void CloseAll::act(Restaurant &restaurant) {
    for(int i = 0; i < restaurant.getNumOfTables(); i++){
        Close close(i);
        close.act(restaurant);
    }
    complete();
}

std::string CloseAll::toString() const {
    return "closeall " + status_to_string(getStatus());
}



PrintMenu::PrintMenu() {}

PrintMenu *PrintMenu::clone() const {
    return new PrintMenu(*this);
}

void PrintMenu::act(Restaurant &restaurant) {
    vector<Dish> menu = restaurant.getMenu();
    for (vector<Dish>::iterator dish = menu.begin(); dish != menu.end(); ++dish)
        cout << (*dish).getName() << " " << (*dish).getType() << " " << (*dish).getPrice() << "NIS" << endl;

    complete();
}

std::string PrintMenu::toString() const {
    return "menu " + status_to_string(getStatus());
}



PrintTableStatus::PrintTableStatus(int id) : tableId(id){}

PrintTableStatus *PrintTableStatus::clone() const {
    return new PrintTableStatus(*this);
}

void PrintTableStatus::act(Restaurant &restaurant) {
    Table *table = restaurant.getTable(tableId);
    string t_status = "open";
    int bill = 0;

    if (!table->isOpen()){
        t_status = "close";
        cout << "Table " << tableId << " status: " << t_status << "\n" << "Customers:" << endl;
        }

    else {
        cout << "Table " << tableId << " status: " << t_status << "\n" << "Customers:" << endl;

        for (vector<Customer *>::iterator cust = table->getCustomers().begin(); cust != table->getCustomers().end(); ++cust)
            cout << (*cust)->getId() << " " << (*cust)->getName() << endl;

        cout << "Orders:" << endl;

        for (vector<OrderPair>::iterator pair = table->getOrders().begin(); pair != table->getOrders().end(); ++pair)
            cout << (*pair).second.getName() <<  " " << (*pair).second.getPrice() << " " << (*pair).first << endl;

        for (vector<OrderPair>::iterator pair = table->getOrders().begin(); pair != table->getOrders().end(); ++pair)
            bill += (*pair).second.getPrice();

        cout << "Current bill: " << bill << endl;
    }
    complete();
}

std::string PrintTableStatus::toString() const {
    string static_id = to_string(tableId);
    return "status " + static_id + " " + status_to_string(getStatus());
}



PrintActionsLog::PrintActionsLog() {}

PrintActionsLog *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

void PrintActionsLog::act(Restaurant &restaurant) {
    for (vector<BaseAction*>::const_iterator action = restaurant.getActionsLog().begin(); action != restaurant.getActionsLog().end(); ++action)
        cout << (*action) << endl;

    complete();
}

std::string PrintActionsLog::toString() const {
    return "log " + status_to_string(getStatus());
}

BackupRestaurant::BackupRestaurant() {}

BackupRestaurant *BackupRestaurant::clone() const {
    return new BackupRestaurant(*this);
}


void BackupRestaurant::act(Restaurant &restaurant) {
    *backup = restaurant;
    complete();
}

std::string BackupRestaurant::toString() const {
    return "backup " + status_to_string(getStatus());
}

RestoreResturant::RestoreResturant() {}

RestoreResturant *RestoreResturant::clone() const {
    return new RestoreResturant(*this);
}

void RestoreResturant::act(Restaurant &restaurant) {
    restaurant = *backup;
    complete();
}

std::string RestoreResturant::toString() const {
    return "restore " + status_to_string(getStatus());
}

string status_to_string(ActionStatus stat) {
    string str_status;
    switch (stat) {
        case PENDING:
            str_status = "PENDING";
        case ERROR:
            str_status = "ERROR";
        case COMPLETED:
            str_status = "COMPLETED";
    }
    return str_status;
}

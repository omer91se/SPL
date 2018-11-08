/**
*@Authors: Anat Bar-Sinai, Omer Segal.
*@version: Last Update: 8.11.2018
*/
#include <iostream>
#include "../include/Table.h"

using namespace std;

template <typename T>
typename vector<T*>::iterator find_it_in_vector (int id, vector<T*> list);


Table::Table(int t_capacity):capacity(t_capacity), open(false)  {}
int Table::getCapacity() const{
    return this->capacity;
}

//TODO check if input checks are needed.
//TODO for all the functions.

/**
 * Adds a customer to customers vector.
 *
 * @param a pointer to a Customer
 */
void Table::addCustomer(Customer *customer) {
    if(customer !=nullptr)
        this->customersList.push_back(customer);
}

/**
 * Removes a Customer.
 *
 * @param id of the Customer to remove.
 */
void Table::removeCustomer(int id) {
    vector<Customer*>::iterator customer_it = find_it_in_vector(id, this->customersList);
    this->customersList.erase(customer_it);
    //TODO check if customer's destructor is called.
}

/**
 * Finds a Customer.
 *
 * @param: id of the Customer.
 * @return: A pointer to the Customer.
 */
Customer* Table::getCustomer(int id) {
    vector<Customer*>::iterator customer_it = find_it_in_vector(id, this->customersList);
            return *(customer_it);
    }

/**
 * customersList Getter.
 *
 * @return: vector of Customer*.
 */
std::vector<Customer *> &Table::getCustomers() {
    return this->customersList;
}

/**
 * orderList Getter.
 *
 * @return: A vector of OrderPair.
 */
std::vector<OrderPair> &Table::getOrders() {
    return orderList;
}

/**
 * Iterates over the customersList and takes orders.
 *
 * @param menu to order from (sends it to the Customer's action order()).
 */
void Table::order(const std::vector<Dish> &menu) {

    //Iterates over each customer and takes orders.
    for (vector<Customer *>::iterator customer_it = this->customersList.begin(); customer_it != this->customersList.end(); ++customer_it){
       vector<int> order = (*customer_it)->order(menu);

        //Iterates over each customer's order and insert it to the order list.
        for (vector<int>::iterator order_it = order.begin(); order_it != order.end(); ++order_it)
            for (int i = 0 ; i < menu.size() ; i++)
                if (menu[i].getId() == *order_it) {
                    Dish dish = menu[i];
                    OrderPair pair((*customer_it)->getId(), dish);
                    this->orderList.push_back(pair);
                    break;
                }
    }
}

/**
 * Sets open to true.
 */
void Table::openTable() {
    this->open = true;
}

/**
 * Sets open to false.
 */
void Table::closeTable() {
    this->open = false;
}

/**
 *  Sums up the bills of each customer to one bill.
 *
 * @return the total price (bill).
 */
int Table::getBill() {
    int bill = 0;

    for (vector<OrderPair>::iterator pair_it = this->orderList.begin(); pair_it != this->orderList.end(); ++pair_it)
        bill = bill + (*pair_it).second.getPrice();

    return bill;
}

/**
 *
 * @return true if the table is open. false otherwise.
 */
bool Table::isOpen() {
    return open;
}

/**
 *
 * @param id
 * @param customers_list
 * @return
 */
vector<Customer*>::iterator find_customer (int id, vector<Customer*> customers_list) {
    for (vector<Customer *>::iterator it = customers_list.begin(); it != customers_list.end(); ++it) {
        if ((*it)->getId() == id)
            return it;
    }
}
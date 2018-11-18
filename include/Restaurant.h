/**
*@Authors: Anat Bar-Sinai, Omer Segal.
*@version: Last Update: 2.11.2018
*/
#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"


class Restaurant{		
public:
	Restaurant();
    Restaurant(const std::string &configFilePath);

    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu() const;

    //Rule of five
    Restaurant(const Restaurant& other);
    Restaurant& operator=(const Restaurant& other);
    Restaurant& operator=(Restaurant &&other);
    Restaurant(Restaurant &&other);
    ~Restaurant();


private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;

    void clean();
    void steal(Restaurant &other);
	void copy(const Restaurant& other) ;
	DishType str_to_DishTp(std::string str);

};

#endif

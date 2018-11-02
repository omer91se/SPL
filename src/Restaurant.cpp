/*
*Authors: Anat Bar-Sinai, Omer Segal.
*Last Update: 2.11.2018
*/

#include "../include/Restaurant.h"
#include <ifstream>

using namespace std;

//Empty constructor.
Restaurant(): open(false), tables(new vector<Table*>), menu(new vector<Dish>), actionsLog(new vector<BaseAction*>){}

/*Constructor
*Input: Path to the config file.
*/
Restaurant(const std::string &configFilePath){
    ifstream config_file (*configFilePath);
    string line;

    if (config_file.is_open()){
        int counter = 0;

        while (getline (config_file,line)){
                if(line.size > 0 && line[0] != "#"){

                    //Assign number of tables
                    if(counter == 1){
                        int num_of_t = stoi(line,nullptr);
                        tables = new vector<Table*>(num_of_t);                     
                    }

                    //Capacity of tables
                    if(counter == 2){
                        
                        
                    }

                    counter++;
                }  
        }
        config_file.close();
    }

    else cout << "Unable to open file"; 

}



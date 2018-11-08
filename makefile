# All Targets
all: ass1

# Tool invocations
# Executable "ass1" depends on the files ass.o and run.o.
ass1: bin/Restaurant.o bin/Action.o bin/Customer.o bin/Dish.o bin/Table.o src/Main.cpp
	@echo 'Building target: ass1'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/Restaurant.o bin/Action.o bin/Customer.o bin/Dish.o bin/Table.o src/Main.cpp
	@echo 'Finished building target: hello'
	@echo ' '

# Depends on the source and header files
bin/Restaurant.o: src/Restaurant.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Restaurant.o src/Restaurant.cpp

# Depends on the source and header files
bin/Action.o: src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

# Depends on the source and header files
bin/Customer.o: src/Customer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Customer.o src/Customer.cpp

# Depends on the source and header files
bin/Dish.o: src/Dish.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Dish.o src/Dish.cpp

# Depends on the source and header files
bin/Table.o: src/Table.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Table.o src/Table.cpp

# Depends on the source and header files
#bin/Main.o: src/Main.cpp
#    g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Main.o src/Main.cpp




#Clean the build directory
clean:
	#rm -f bin/*

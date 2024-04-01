CC = g++
FLAGS = --std=c++11
file_name = sat_solver
INPUT_FILE = 01.cnf

all: build run

build: $(file_name)

$(file_name): $(file_name).cpp
	$(CC) $(FLAGS) $< -o $@

run:
	./$(file_name) < $(INPUT_FILE)

clean:
	rm -f $(file_name)

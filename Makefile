CC = g++
FLAGS = --std=c++11
file_name = sat_solver
INPUT_FILE = ./benchmarks/aim-50-1_6-no-1.cnf

all: build run

build: $(file_name)

$(file_name): $(file_name).cpp
	$(CC) $(FLAGS) $< -o $@

run:
	./$(file_name) < $(INPUT_FILE)

run_multiple:
	./run_benchmarks.sh

clean:
	rm -f $(file_name)
	rm -f *.txt

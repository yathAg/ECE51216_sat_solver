# ECE51216_sat_solver

This repository contains the files for the ECE51216 course project.

Team Members

- Gauthami Arun
- Yatharth Agarwal

## Folder structure and file functions

| Directory/File     | Description                                                                                                  |
|--------------------|--------------------------------------------------------------------------------------------------------------|
| _benchmarks        | Contains all the `.cnf` files used for testing the SAT solver.                                               |
| _benchmarks/ZIP    | Contains the original downloaded zip files of benchmarks from [here](https://www.cs.ubc.ca/~hoos/SATLIB/benchm.html). |
| _reports           | Contains the project guidelines and final report submission.                                                  |
| _results         | Contains the all the evaluation results and script to compile results into a .csv|
| format_checker.cpp | A script to check if the output file is in the correct format for submission.                                |
| run.sh             | Evaluates all the benchmarks and stores the result in their respective directory. Can automatically time out if the file takes too long to execute. |
| SatSolver.py       | DPLL SAT solver with CDCL and VSIDS implementation.                                                          |

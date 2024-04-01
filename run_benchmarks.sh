#!/bin/bash

# Define the path to the SAT solver
SOLVER="./solver"

# Define the path to the benchmarks folder
BENCHMARKS_FOLDER="./benchmarks"

# Define the path to the output file
OUTPUT_FILE="./results.txt"

# Remove the existing results file if it exists
rm -f "$OUTPUT_FILE"

# Find all .cnf files in the benchmarks folder and its subfolders
find "$BENCHMARKS_FOLDER" -type f -name "*.cnf" | while read -r file; do
    # Run the solver with the current .cnf file and append the result to the output file
    echo "Running solver for $file..."
    $SOLVER "$file" >> "$OUTPUT_FILE"
done

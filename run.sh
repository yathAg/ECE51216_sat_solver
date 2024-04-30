#!/bin/bash

# Loop through all .cnf files in the _benchmarks directory
for file in _benchmarks/*.cnf; do
    # Get the filename without the directory path and extension
    filename=$(basename -- "$file")
    filename_no_ext="${filename%.*}"

    # Run the python3 code with the current .cnf file and pipe the output to format_checker
    python3 code.py "$file" > "_results/${filename_no_ext}_output.txt"
done

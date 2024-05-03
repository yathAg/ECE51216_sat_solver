#!/bin/bash

# Loop through all .cnf files in the _benchmarks directory
for file in _benchmarks/*.cnf; do
    # Get the filename without the directory path and extension
    filename=$(basename -- "$file")
    filename_no_ext="${filename%.*}"

    # Run the python3 code with the current .cnf file and pipe the output to format_checker
    if timeout 120m python3 SatSolver.py "$file" > "_decay5/${filename_no_ext}_output.txt"; then
        # Print message indicating the completion of evaluation for the current file
        echo "Done evaluating $filename"
    else
        # Print message indicating the skipping of the file due to timeout
        echo "Skipping $filename due to timeout"
        # Add specific result message to the output file
        echo "RESULT: Skipped file" > "_decay5/${filename_no_ext}_output.txt"
        continue  # Skip to the next file
    fi

    # Delay for 5 seconds
    sleep 5

done

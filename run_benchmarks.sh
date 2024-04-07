#!/bin/bash

# Define the output file
output_file="all_outputs.txt"

# Clear the output file if it already exists
> "$output_file"

# Iterate over each file in the benchmarks directory
for file in benchmarks/*; do
    # Extract the filename without the path
    filename=$(basename "$file")
    
    # Run the command and append the output, including filename, to the text file
    echo "Output for file: $filename" >> "$output_file"
    ./sat_solver < "$file" >> "$output_file"
    echo -e "\n\n" >> "$output_file"  # Add some spacing between each output
done

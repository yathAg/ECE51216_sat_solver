import os
import csv
import re

def extract_data(file_path):
    with open(file_path, 'r') as file:
        data = file.read()

    # Extracting required information using regular expressions
    match = re.search(r'Vars\s*:\s*(\d+)\s*Clauses\s*:\s*(\d+).*Read time\s*:\s*(\d+\.\d+).*# Restarts\s*:\s*(\d+).*# Learned Clauses\s*:\s*(\d+).*# Decisions\s*:\s*(\d+).*# Implications\s*:\s*(\d+).*Solve time\s*:\s*(\d+\.\d+).*RESULT\s*:\s*(\w+)', data, re.DOTALL)
    
    if match:
        vars_count = match.group(1)
        clauses_count = match.group(2)
        restarts = match.group(4)
        learned_clauses = match.group(5)
        decisions = match.group(6)
        implications = match.group(7)
        solve_time = match.group(8)
        result = match.group(9)
        return vars_count, clauses_count, solve_time, restarts, learned_clauses, decisions, implications, result
    else:
        skipped_match = re.search(r'RESULT\s*:\s*(Skipped file)', data)
        if skipped_match:
            return None, None, None, None, None, None, None, skipped_match.group(1)
        else:
            return None

def process_files(folder_path):
    data_list = []
    for filename in os.listdir(folder_path):
        if filename.endswith('.txt'):
            file_path = os.path.join(folder_path, filename)
            file_data = extract_data(file_path)
            if file_data:
                data_list.append((file_data + (filename,)))
    return data_list

def write_to_csv(data_list, folder_path):
    folder_name = os.path.basename(folder_path)
    csv_file = os.path.join(folder_path, f'{folder_name}.csv')
    with open(csv_file, 'w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['Vars', 'Clauses', 'Solve time', 'Restarts', 'Learned Clauses', 'Decisions', 'Implications', 'Result', 'File'])
        writer.writerows(data_list)

def main():
    folder_path = './_decay5'

    data_list = process_files(folder_path)
    if data_list:
        write_to_csv(data_list, folder_path)
        print("CSV file created successfully!")
    else:
        print("No data found in the folder.")

if __name__ == "__main__":
    main()

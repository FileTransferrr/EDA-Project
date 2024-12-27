import re
import sys

def extract_identifiers(file_path):
    identifiers = set()
    max_order = 0
    with open(file_path, 'r', encoding='utf-8') as file:
        lines = file.readlines()
        for line in lines:
            order_matches = re.findall(r'(\d+)\)', line)
            if order_matches:
                max_order = max(max_order, int(order_matches[0]))
                
            matches = re.findall(r'w_\d+_\d+', line)
            identifiers.update(matches)
    return identifiers, max_order

def compare_identifiers(reference_identifiers, input_identifiers):
    missing_identifiers = input_identifiers - reference_identifiers
    return missing_identifiers

if (len(sys.argv) != 3):
    print("Usage: python checker_R4.py <reference_file_path> <input_file_path>")
    sys.exit(1)
    
reference_file_path = sys.argv[1]
input_file_path = sys.argv[2]

reference_identifiers, reference_max_order = extract_identifiers(reference_file_path)
input_identifiers, input_max_order = extract_identifiers(input_file_path)

if reference_max_order != input_max_order:
    print("Register Quantity Error!")
    sys.exit(1)
missing_identifiers = compare_identifiers(reference_identifiers, input_identifiers)

if not missing_identifiers:
    print("Matched!")
else:
    print("The following identifiers are missing in the reference file:")
    for identifier in missing_identifiers:
        print(identifier)
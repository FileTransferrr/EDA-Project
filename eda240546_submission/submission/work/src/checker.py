import re

def parse_loops(file_path):
    loops = []
    with open(file_path, 'r') as file:
        content = file.read()
        if (file_path == "../../result_1.txt" or file_path == "../soln/result_1_soln.txt" or file_path == "../../result_2.txt" or file_path == "../soln/result_2_soln.txt"):
            loop_pattern = re.compile(r"\d+\)\s*Loop Signals: (.+?)\n\s*Loop Gates: (.+?)\n", re.DOTALL)
        if (file_path == "../../result_3.txt" or file_path == "../../gate_5000_5000_1000/result_3.txt"):
            loop_pattern = re.compile(r"\d+\)\s*Loop Signals: (.+?)\n\s*Loop Gates: (.+?)\n\s*Loop Condition: (.+?)\n", re.DOTALL)
        if (file_path == "../../result_4.txt" or file_path == "../soln/result_4_soln.txt"):
            loop_pattern = re.complie(r"\d+\)\s*Loop Breaker: (.+?)\n", re.DOTALL)
            
        matches = loop_pattern.findall(content)

        for match in matches:
            if len(match) == 1:
                signals = ""
                gates = ""
                conditions = ""
                breakers = match
            if len(match) == 2:
                signals, gates = match
                breakers = ""
                conditions = ""
            elif len(match) == 3:
                signals, gates, conditions = match
                breakers = ""

            signal_list = tuple(sorted(s.strip() for s in signals.split(",") if s.strip()))
            gate_list = tuple(sorted(g.strip() for g in gates.split(",") if g.strip()))
            condition_list = tuple(sorted(c.strip() for c in conditions.split(",") if c.strip())) if conditions else tuple()
            breaker_list = tuple(sorted(b.strip() for b in breakers.split(",") if b.strip()))

            loops.append((signal_list, gate_list, condition_list, breaker_list))

    return loops

def compare_files(file1, file2):
    loops1 = parse_loops(file1)
    loops2 = parse_loops(file2)
    missing_loops = []
    for i, loop1 in enumerate(loops1):
        if loop1 not in loops2:
            missing_loops.append((i + 1, loop1))

    if missing_loops:
        print("The loop in file1 that not founded in file2:")
        for i, loop in missing_loops:
            print(f"The {i}-th loop in file1:")
            print(f"  Signals: {loop[0]}")
            print(f"  Gates: {loop[1]}")
            print(f"  Conditions: {loop[2]}")
            print(f"  Breakers: {loop[3]}")
    else:
        print("Matched!")

file1 = "../../result_3.txt"
file2 = "../../gate_5000_5000_1000/result_3.txt"

compare_files(file1, file2)

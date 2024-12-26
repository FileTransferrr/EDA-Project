import sys
import re


if len(sys.argv) == 1:
	print("""
Usage:
	python collect_mem.py verisim.log
""")
	exit(0)

log_file = sys.argv[1]

print(log_file)

print_header = False

with open("memory.csv", "w+") as csv_file:
    with open(log_file) as f:
        for line in f.readlines():
            line = line[:-1]
            ret = re.match(r'=N:\[(.+)\] Memory Usage - RSS: (.+) MBytes, Peak: (.+) MBytes, Virtual: (.+) MBytes', line)
            if ret:

                if not print_header:
                    csv_file.write("item,RSS,Peak,Virtual\n")
                    print_header = True

                csv_file.write("{},{},{},{}\n".format(ret.group(1).replace(",", " "), ret.group(2), ret.group(3), ret.group(4)))

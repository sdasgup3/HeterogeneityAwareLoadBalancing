import os,sys,csv
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt 
import numpy as np
path = os.getcwd()

avg_idle_time_list = {}
avg_time_per_iteration_list = {}
max_idle_time_list = {}
avg_idle_time_list2 = {}
avg_time_per_iteration_list2 = {}
max_idle_time_list2 = {}

split_char = ':'

if len(sys.argv) > 3:
	x = int(sys.argv[1])
	y = int(sys.argv[2])
	file_name = sys.argv[3]
else:
	print("USAGE:  python parseAndPlot.py x_position y_position file_name file_2")
	exit(-1)

if len(sys.argv) > 4:
	file_2 = sys.argv[4]

file_without_ext = os.path.splitext(file_name)[0]
#result_path = os.path.join(path, file_without_ext)
#if not os.path.exists(result_path): os.makedirs(result_path)

input_file  = os.path.join(path, file_name)
csv_reader  = csv.reader(input_file)
i=0
with open(input_file) as csv_reader:
    for row in csv_reader:
        tokens = [t.strip() for t in row.split(split_char)]
        token = int(tokens[x])
	if "Total Program Time" in row:
          avg_time_per_iteration_list[token] = float(tokens[y])
        elif "Max idle time" in row:
          max_idle_time_list[token] = float(tokens[y])
        elif "Average idle time" in row:
          avg_idle_time_list[token] = float(tokens[y])

csv_reader = csv.reader(file_2)

with open(file_2) as csv_reader:
    for row in csv_reader:
        tokens = [t.strip() for t in row.split(split_char)]
        token = int(tokens[x])
	if "Total Program Time:" in row:
          avg_time_per_iteration_list2[token] = float(tokens[y])
        elif "Max idle time" in row:
          max_idle_time_list2[token] = float(tokens[y])
        elif "Average idle time" in row:
          avg_idle_time_list2[token] = float(tokens[y])

fig1 = plt.figure()
plt.xlabel('Power')
plt.ylabel('Total Program Time')
plt.title('Total Program Time vs Power (with RefineLB)')
plt.plot(avg_time_per_iteration_list.keys(),avg_time_per_iteration_list.values(),'ro', label = 'With PowerAwareGreedyLB')
fig1.savefig(file_without_ext + "_total_program_time.eps")

plt.plot(avg_time_per_iteration_list2.keys(),avg_time_per_iteration_list2.values(),'g^', label = 'With GreedyLB')
plt.legend()
fig1.savefig(file_without_ext + "_total_program_time.eps")

fig2 = plt.figure()
plt.xlabel('Power')
plt.ylabel('Max Idle time ')
plt.title('Max Idle time vs Power ')

plt.plot(max_idle_time_list.keys(),max_idle_time_list.values(),'ro', label = 'With PowerAwareGreedyLB')
fig2.savefig(file_without_ext + "__Max_idle_time.eps")

plt.plot(max_idle_time_list2.keys(),max_idle_time_list2.values(),'g^', label = 'With GreedyLB')
plt.legend()
fig2.savefig(file_without_ext + "__Max_idle_time.eps")

fig3 = plt.figure()
plt.xlabel('Power')
plt.ylabel('Average Idle time')
plt.title('Average Idle time vs Power ')

plt.plot(avg_idle_time_list.keys(),avg_idle_time_list.values(),'ro', label = 'With PowerAwareGreedyLB')
fig3.savefig(file_without_ext + "__Average_idle_time.eps")

plt.plot(avg_idle_time_list2.keys(),avg_idle_time_list2.values(),'g^', label = 'With GreedyLB')
plt.legend()
fig3.savefig(file_without_ext + "__Average_idle_time.eps")

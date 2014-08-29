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
	if "Time per iteration:" in row:
          avg_time_per_iteration_list[token] = float(tokens[y])
        elif "Percentage max idle time" in row:
          max_idle_time_list[token] = float(tokens[y])
        elif "Percentage average idle time" in row:
          avg_idle_time_list[token] = float(tokens[y])

csv_reader = csv.reader(file_2)

with open(file_2) as csv_reader:
    for row in csv_reader:
        tokens = [t.strip() for t in row.split(split_char)]
        token = int(tokens[x])
	if "Time per iteration:" in row:
          avg_time_per_iteration_list2[token] = float(tokens[y])
        elif "Percentage max idle time" in row:
          max_idle_time_list2[token] = float(tokens[y])
        elif "Percentage average idle time" in row:
          avg_idle_time_list2[token] = float(tokens[y])

fig1 = plt.figure()
plt.xlabel('Power')
plt.ylabel('Average time per iteration')
plt.title('Average time per iteration vs Power (with RefineLB)')
plt.plot(avg_time_per_iteration_list.keys(),avg_time_per_iteration_list.values(),'ro', label = 'With RefineLB')
fig1.savefig(file_without_ext + "_Average_time_per_iteration.eps")

plt.plot(avg_time_per_iteration_list2.keys(),avg_time_per_iteration_list2.values(),'g^', label = 'Without RefineLB')
plt.legend()
fig1.savefig(file_without_ext + "_Average_time_per_iteration.eps")

fig2 = plt.figure()
plt.xlabel('Power')
plt.ylabel('Percentage Max Idle time')
plt.title('Percentage Max Idle time vs Power (with RefineLB)')

plt.plot(max_idle_time_list.keys(),max_idle_time_list.values(),'ro', label = 'With RefineLB')
fig2.savefig(file_without_ext + "_Percentage_max_idle_time.eps")

plt.plot(max_idle_time_list2.keys(),max_idle_time_list2.values(),'g^', label = 'Without RefineLB')
plt.legend()
fig2.savefig(file_without_ext + "_Percentage_max_idle_time.eps")

fig3 = plt.figure()
plt.xlabel('Power')
plt.ylabel('Percentage Average idle time')
plt.title('Percentage Average idle time vs Power (with RefineLB)')

plt.plot(avg_idle_time_list.keys(),avg_idle_time_list.values(),'ro', label = 'With RefineLB')
fig3.savefig(file_without_ext + "_Percentage_Average_idle_time.eps")

plt.plot(avg_idle_time_list2.keys(),avg_idle_time_list2.values(),'g^', label = 'Without RefineLB')
plt.legend()
fig3.savefig(file_without_ext + "_Percentage_Average_idle_time.eps")

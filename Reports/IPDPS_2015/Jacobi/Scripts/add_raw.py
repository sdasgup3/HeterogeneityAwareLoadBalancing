import os,sys,csv,math
#import numpy as np
path = os.getcwd()


split_char = ':'

if len(sys.argv) > 3:
	iteration = int(sys.argv[1])
	start     = int(sys.argv[2])
	skip      = int(sys.argv[3])
	file_name = sys.argv[4]
else:
	print("USAGE:  python .py itreation_count start_count raw_file")
	exit(-1)

result_file = "result_add_raw"
i=1
s=0.0
i_skip = 1
f = open(result_file,'w')
input_file  = os.path.join(path, file_name)
csv_reader  = csv.reader(input_file)
with open(input_file) as csv_reader:
    for row in csv_reader:
        tokens = [t.strip() for t in row.split(split_char)]
        token = float(tokens[0])
	s+=token
        i_skip = i_skip + 1 
	if(i % iteration == 0):
		f.write(str(start))
		f.write(' ')
		f.write(str(s))
		f.write('\n')
		s=0.0
		start = start - 1
                i_skip = 1
	i = i+1
	if(start==0):
		break
	
	

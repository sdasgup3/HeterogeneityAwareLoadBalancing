import os,sys,csv
from collections import defaultdict

#################
#usage
#py .py file_path
#################

path = os.getcwd()
result_path = os.path.join(path,"../Document/InitialTest_withoutPcap/")
result_file = os.path.join(result_path,"totalTimePerChar.csv") 
file = os.path.join(result_path, sys.argv[1])

dict_char = dict()
#for i in range(0,20):
 # for j in range(0,20):
  #  temp = "(" + str(i) + " " + str(j)+")"
   # dict_char[temp] = dict()
with open(file) as csv_reader:
    for row in csv_reader:
        #print row
        tokens = [t.strip() for t in row.split(",")]
        try:
          chare = str(tokens[0])
          time = float(tokens[2])
          if chare in dict_char.keys():
            dict_char[chare] = dict_char[chare] + time 
          else:
            dict_char[chare] = time
        except ValueError:
                      continue
csv_writer = csv.writer(open(result_file,'wb'))
for key in dict_char:
    row = []
    row.append(key)
    row.append(dict_char.get(key))
    csv_writer.writerow(row)

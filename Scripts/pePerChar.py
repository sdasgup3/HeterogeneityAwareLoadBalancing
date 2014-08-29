import os,sys,csv
from collections import defaultdict
#################
#usage
#py .py file_path
##############


path = os.getcwd()
result_path = os.path.join(path,"../Document/InitialTest_withoutPcap/")
result_file = os.path.join(result_path,"numberOfPePerChar.csv") 
file = os.path.join(result_path, sys.argv[1])

dict_char = dict()
for i in range(0,20):
  for j in range(0,20):
    temp = "(" + str(i) + " " + str(j)+")"
    dict_char[temp] = dict()
with open(file) as csv_reader:
    for row in csv_reader:
        tokens = [t.strip() for t in row.split(",")]
        try:
          chare = str(tokens[0])
          pe = int(tokens[1])
          if pe in dict_char.get(chare).keys():
            dict_char[chare][pe] = dict_char[chare][pe] + 1 
          else:
            dict_char[chare][pe] = 1
        except ValueError:
                      continue

csv_writer = csv.writer(open(result_file,'wb'))
for key in dict_char:
    row = []
    row.append(key)
    row.append(dict_char.get(key))
    csv_writer.writerow(row)

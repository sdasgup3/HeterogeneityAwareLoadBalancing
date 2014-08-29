import os,sys,csv
import matplotlib 
matplotlib.use('Agg')
import matplotlib.pyplot as plt 

path = os.getcwd()

split_char = ','
result = 'result'

if len(sys.argv) > 3:
	x = int(sys.argv[1])
	y = int(sys.argv[2])
	file_name = sys.argv[3]
else:
	print("USAGE:  python parseAndPlot.py x_position y_position file_name file_2")
	exit(-1)
if len(sys.argv) > 4:
	file_2 = sys.argv[4]
fig2 = plt.figure()
def plot_graph(file_name,j):
	input_file  = os.path.join(path, file_name)
	csv_reader  = csv.reader(input_file)
	i=0
	with open(input_file) as csv_reader:
	    for row in csv_reader:
	        tokens = [t.strip() for t in row.split(split_char)]
		token = int(tokens[x])
		color = 'g'
		if(j==0):
			color = 'y'
		if(i<6):
			line_style = '^'
		elif(i<12):
			line_style = 'p'
		elif(i<18):
			line_style = 'D'
		elif(i<24):
			line_style = 's'
		elif(i<30):
			line_style = 'h'
		elif(i<36):
			line_style = '>'
		elif(i<42):
			line_style = '*'
		elif(i<48):
			line_style = 'v'
		elif(i<54):
			line_style = 'o'
		elif(i<60):
			line_style = '<'
		if(j==0):
			if(i%6==0):
				plt.plot(token,tokens[y], color+line_style, label = "Core of Tarekc"+str(i/6+41)+" (" +str(i)+"-"+str(i+5)+")")
 			else:
				plt.plot(token,tokens[y], color+line_style)
 		else:
			plt.plot(token,tokens[y], color+line_style)
		i = i+1		
	plt.legend(ncol=2, prop={'size':6})
	plt.xlabel('Core Id')
	plt.ylabel('Total Execution Time (sec)')
	plt.title('Jacobi2d- Frame Size: 36000x36000 Chare Size: 600x600 Iterations: 150', fontsize = 10)	
	plt.text(50,480,'Power: 20W',fontsize=8,bbox={'facecolor':'green'})
	plt.text(50,450,'Power: 60W',fontsize=8,bbox={'facecolor':'yellow'})
	fig2.savefig(result + ".eps")
	return

plot_graph(file_2,0)
plot_graph(file_name,1)

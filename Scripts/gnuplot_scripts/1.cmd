set style data linespoints
#set terminal png
#set output '1.png'
#set term postscript eps size 1024, 720 color blacktext "Helvetica" 24
set term postscript eps color blacktext "Helvetica" 24
set output '1.eps'
set datafile separator ","
set xlabel "core ids"
set ylabel "average execution time (secs)"
set title "Core Id vs Average Execution Time"
set xrange [0:12]
set yrange [0:1]
plot "1.txt" using 1:2 with lp pt 5 ps 1 notitle


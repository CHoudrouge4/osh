set terminal png size 1600,1200 rounded
set output 'main.png'

plot for [i=0:9] 'sa_runner'.i.'.csv' using 3:2 with lines

set terminal png size 1600,1200 rounded
set output 'main.png'

plot for [i=0:99] 'sa_runner_22_100_'.i.'.csv' using 2:3 with lines notitle

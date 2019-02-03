#set terminal png
#set output 'result.png'
#
#plot \
#  '0.csv' using 1:3 with lines title '1 100', \
#  '1.csv' using 1:3 with lines title '1 500', \
#  '2.csv' using 1:3 with lines title '1 1000',\
#  '3.csv' using 1:3 with lines title '2 100', \
#  '4.csv' using 1:3 with lines title '2 500', \
#  '5.csv' using 1:3 with lines title '2 1000'
#


#set terminal png size 1600,1200 rounded
#set output 'mulambdas.png'
#
#plot \
#  'mulambda0.csv' using 2:3 with lines title 'ml 1 ', \
#  'mulambda1.csv' using 2:3 with lines title 'ml 2 ', \
#  'mulambda2.csv' using 2:3 with lines title 'ml 3 ', \
#  'mulambda3.csv' using 2:3 with lines title 'ml 4 ',
##  'mulambda4.csv' using 2:3 with lines title 'ml 5 ', \
##  'mulambda5.csv' using 2:3 with lines title 'ml 6 '

#set terminal png size 1600,1200 rounded
#set output 'main.png'
#
#plot \
#  'main0.csv' using 2:3 with lines title 'ml 1 ', \
#  'main1.csv' using 2:3 with lines title 'ml 2 ', \
#  'main2.csv' using 2:3 with lines title 'ml 3 ', \
#  'main3.csv' using 2:3 with lines title 'ml 4 ', \
#  'main4.csv' using 2:3 with lines title 'ml 5 ', \
#  'main5.csv' using 2:3 with lines title 'ml 6 ', \
#  'main6.csv' using 2:3 with lines title 'ml 7 '

set terminal png size 1600,1200 rounded
set output 'main.png'

plot \
  'sa_runner0.csv' using 2:3 with lines title 'ml 1 ', \
  'sa_runner1.csv' using 2:3 with lines title 'ml 2 ', \
  'sa_runner2.csv' using 2:3 with lines title 'ml 3 ', \
  'sa_runner3.csv' using 2:3 with lines title 'ml 4 ', \
  'sa_runner4.csv' using 2:3 with lines title 'ml 5 ', \
  'sa_runner5.csv' using 2:3 with lines title 'ml 6 ', \
  'sa_runner6.csv' using 2:3 with lines title 'ml 7 ', \
  'sa_runner7.csv' using 2:3 with lines title 'ml 8 ', \
  'sa_runner8.csv' using 2:3 with lines title 'ml 9 ', \
  'sa_runner9.csv' using 2:3 with lines title 'ml 10 ', 

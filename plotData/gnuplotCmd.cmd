set terminal png
set output 'result.png'

plot \
  '0.csv' using 1:3 with lines title '1 100', \
  '1.csv' using 1:3 with lines title '1 500', \
  '2.csv' using 1:3 with lines title '1 1000',\
  '3.csv' using 1:3 with lines title '2 100', \
  '4.csv' using 1:3 with lines title '2 500', \
  '5.csv' using 1:3 with lines title '2 1000'

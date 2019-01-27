set terminal png
set output 'something.png'

plot '0.csv' using 2:3 with lines title '100', '1.csv' using 2:3 with lines title '500', '2.csv' using 2:3 with lines title '1000'

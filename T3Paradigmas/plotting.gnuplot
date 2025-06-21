# Configuración básica
set datafile separator ","
set title "Triangulación de Delaunay"
set xlabel "x"
set ylabel "y"
set size ratio -1
set grid

# Dibuja cada arista como un vector desde (x1,y1) hasta (x2,y2)
plot "partition.csv" using 1:2:($3-$1):($4-$2) with vectors nohead lw 1 lc rgb "blue" title "Aristas"


FicheroDatos1= "N750.txt"
FicheroGrafica= "DepenV_N750.pdf"
set xlabel "V(x)"
set ylabel "K"
set xtics font "Times New Roman,10" 
set key bottom left box
set xrange[0:1]
set yrange[0:1]

plot FicheroDatos1 using 1:2 with linespoints lc 1 t "1ºAlgoritmo",  FicheroDatos1 using 1:3 with linespoints lc 2 t "2ºAlgoritmo", FicheroDatos1 using 1:4 with points lc 5 t "Teórico"

set term pdfcairo enhanced color font "Times New Roman"
set output FicheroGrafica
replot







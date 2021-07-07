FicheroDatos1= "longCorrelacion.txt"
FicheroGrafica="longCorrelacion.pdf"
set xlabel "T"
set ylabel "ξ"
set xtics font "Times New Roman,10" 
set key top right box
set xrange[1:4]


plot FicheroDatos1 using 1:2 with points pt 2 ps 1 lc 1 notitle 

set term pdfcairo enhanced color font "Times New Roman"
set output FicheroGrafica
replot







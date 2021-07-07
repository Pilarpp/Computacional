FicheroDatos1= "fEnergia16.txt"
FicheroDatos2= "fEnergia32.txt"
FicheroDatos3= "fEnergia64.txt"
FicheroDatos4= "fEnergia128.txt"
FicheroGrafica= "Energia.pdf"
set xlabel "T"
set ylabel "Energia media"
set xtics font "Times New Roman,10" 
set key bottom right box





plot FicheroDatos1 using 1:2 with linespoints lc 1 t " N=16", FicheroDatos2 using 1:2 with linespoints lc 2 t " N=32", FicheroDatos3 using 1:2 with linespoints lc 3 t " N=64", FicheroDatos4 using 1:2 with linespoints lc 5 t " N=128"

set term pdfcairo enhanced color font "Times New Roman"
set output FicheroGrafica
replot











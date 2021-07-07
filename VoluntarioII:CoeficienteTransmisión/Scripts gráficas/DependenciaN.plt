FicheroDatos1= "0.1.txt"
FicheroDatos2= "0.3.txt"
FicheroDatos3= "0.5.txt"
FicheroDatos4= "0.8.txt"
FicheroGrafica= "DependN.pdf"
set xlabel "N"
set ylabel "K"
set xtics font "Times New Roman,10" 
set key bottom right box
set yrange[0:1]




plot FicheroDatos1 using 1:2 with linespoints lc 1 t " λ=0.1",  FicheroDatos2 using 1:2 with linespoints lc 2 t " λ=0.3", FicheroDatos3 using 1:2 with linespoints lc 4 t " λ=0.5", FicheroDatos4 using 1:2 with linespoints lc 5 t " λ=0.8"

set term pdfcairo enhanced color font "Times New Roman"
set output FicheroGrafica
replot

















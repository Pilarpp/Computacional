FicheroDatos1= "4.nciclooptimo500.txt"
FicheroDatos2= "4.nciclooptimo1000.txt"
FicheroDatos3= "4.nciclooptimo2000.txt"
FicheroGrafica= "nciclosoptimo.pdf"
set xlabel "nciclos"
set ylabel "K"
set xtics font "Times New Roman,10" 
set key bottom right box
set yrange[0:1]




plot FicheroDatos1 using 1:2 with linespoints lc 1 t " N=500",  FicheroDatos2 using 1:2 with linespoints lc 2 t " N=1000", FicheroDatos3 using 1:2 with linespoints lc 4 t " N=2000"

set term pdfcairo enhanced color font "Times New Roman"
set output FicheroGrafica
replot



































































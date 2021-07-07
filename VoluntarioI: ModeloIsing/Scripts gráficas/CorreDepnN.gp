FicheroDatos1= "CorrelT1.5_32.txt"
FicheroDatos2= "CorrelT1.9_32.txt"
FicheroDatos3= "CorrelT2.3_32.txt"
FicheroDatos4= "CorrelT2.7_32.txt"
FicheroDatos5= "CorrelT3.1_32.txt"
FicheroGrafica="Correl32.pdf"
set xlabel "i"
set ylabel "f(i)"
set xtics font "Times New Roman,10" 
set key bottom left box
set xrange[0:8]


plot FicheroDatos1 using 1:2 with linespoints pt 2 ps 1 lc 1 t " T=1.5", FicheroDatos2 using 1:2 with linespoints pt 2 ps 1 lc 2 t " T=1.9", FicheroDatos3 using 1:2 with linespoints pt 2 ps 1 lc 3 t " T=2.3", FicheroDatos4 using 1:2 with linespoints pt 2 ps 1 lc 4 t " T=2.7", FicheroDatos5 using 1:2 with linespoints pt 2 ps 1 lc 5 t " T=3.1"

set term pdfcairo enhanced color font "Times New Roman"
set output FicheroGrafica
replot






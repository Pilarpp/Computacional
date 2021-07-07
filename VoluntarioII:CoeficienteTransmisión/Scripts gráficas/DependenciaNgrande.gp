FicheroDatos1= "1.txt"
FicheroDatos2= "5.txt"
FicheroDatos3= "10.txt"
FicheroGrafica= "DependNgrande.pdf"
set xlabel "N"
set ylabel "K"
set xtics font "Times New Roman,10" 
set key top left box
set yrange[0:0.1]





plot FicheroDatos1 using 1:2 with linespoints lc 1 t " λ=1",  FicheroDatos2 using 1:2 with linespoints lc 2 t " λ=5", FicheroDatos3 using 1:2 with linespoints lc 4 t " λ=10"

set term pdfcairo enhanced color font "Times New Roman"
set output FicheroGrafica
replot









FicheroDatos1= "4.nDoptimo500_0.1_1.txt"
FicheroDatos2= "4.nDoptimo1000_1.txt"
FicheroDatos3= "4.nDoptimo2000_0.5.txt"
FicheroGrafica= "nDoptimo.pdf"
set xlabel "nD"
set ylabel "K"
set xtics font "Times New Roman,10" 
set key center box
set yrange[0:1]




plot FicheroDatos1 using 1:2 with linespoints lc 1 t " N=500,  λ=0.1",  FicheroDatos2 using 1:2 with linespoints lc 2 t " N=1000,  λ=1", FicheroDatos3 using 1:2 with linespoints lc 4 t " N=2000,  λ=0.5"

set term pdfcairo enhanced color font "Times New Roman"
set output FicheroGrafica
replot





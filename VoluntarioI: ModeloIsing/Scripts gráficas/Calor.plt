FicheroDatos1= "fCalor16.txt"
FicheroDatos2= "fCalor32.txt"
FicheroDatos3= "fCalor64.txt"
FicheroDatos4= "fCalor128.txt"
FicheroGrafica= "Calor.pdf"
set xlabel "T"
set ylabel "Calor específico"
set xtics font "Times New Roman,10" 
set key top right box
set xrange[1.5:3.5]
f(x)=-0.4945*log(abs((2.269-x)/2.269))



plot FicheroDatos1 using 1:2 with linespoints lc 1 t " N=16", FicheroDatos2 using 1:2 with linespoints lc 2 t " N=32", FicheroDatos3 using 1:2 with linespoints lc 3 t " N=64", FicheroDatos4 using 1:2 with linespoints lc 5 t " N=128", f(x) lc 7 t " Teórico"

set term pdfcairo enhanced color font "Times New Roman"
set output FicheroGrafica
replot










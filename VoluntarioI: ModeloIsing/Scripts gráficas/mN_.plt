FicheroDatos1= "fmN16.txt"
FicheroDatos2= "fmN32.txt"
FicheroDatos3= "fmN64.txt"
FicheroDatos4= "fmN128.txt"
Ficherodatos5="puntos.txt"
FicheroGrafica= "mN.pdf"
set xlabel "T"
set ylabel "mN"
set xtics font "Times New Roman,10" 
set key top right box
set xrange[1.5:3.5]

f(x)=(1-(sinh(2/x))**(-4))**(0.125)


plot FicheroDatos1 using 1:2 with linespoints lc 1 t " N=16", FicheroDatos2 using 1:2 with linespoints lc 2 t " N=32", FicheroDatos3 using 1:2 with linespoints lc 3 t " N=64", FicheroDatos4 using 1:2 with linespoints lc 5 t " N=128", f(x) lc 7 t " Teórico", Ficherodatos5 using 1:2 with lines lc 7 notitle



set term pdfcairo enhanced color font "Times New Roman"
set output FicheroGrafica
replot































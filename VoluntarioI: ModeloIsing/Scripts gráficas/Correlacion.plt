FicheroDatos1= "CorrelT1.5_16.txt"
FicheroGrafica="CorrelT1.5_16.pdf"
set xlabel "i"
set ylabel "f(i)"
set xtics font "Times New Roman,10" 
set key top right box
set xrange[0:8]

f(x)=b*exp(-x*1.0/(a*1.0))+c
fit f(x) FicheroDatos1 u 1:2 via a,b,c


plot FicheroDatos1 using 1:2 with points pt 2 ps 1 lc 1 t " N=128, T=1.5", f(x) lc 2 t " Ajuste"

set term pdfcairo enhanced color font "Times New Roman"
set output FicheroGrafica
replot


























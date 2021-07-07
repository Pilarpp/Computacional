FicheroDatos1= "correlaciondatosajustar.txt"
FicheroGrafica="AjusteLongCorre.pdf"
set xlabel "T"
set ylabel "ξ"
set xtics font "Times New Roman,10" 
set key top right box
set xrange[1.5:3.5]

f(x)=a*abs((x-2.275)/2.275)**b
fit f(x) FicheroDatos1 u 1:2 via a,b


plot FicheroDatos1 using 1:2 with points pt 2 ps 1 lc 1 t " N=128", f(x) lc 2 t " Ajuste"

set term pdfcairo enhanced color font "Times New Roman"
set output FicheroGrafica
replot






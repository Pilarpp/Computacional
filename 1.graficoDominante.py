from matplotlib import pyplot as plt
import numpy as np


dataIni=np.loadtxt("EstimacionesDominante.txt",max_rows=1)
ypoints=dataIni

data=np.loadtxt("EstimacionesDominante.txt",skiprows=3)
x=data[:,0]
y=data[:,1]



fig=plt.figure()
ax=fig.add_subplot(111)
ax.set_ylabel("Valor propio dominante", fontsize=14)
ax.set_xlabel("Interacciones", fontsize=14)

plt.axhline(ypoints, 0, 20, label='Valor analítico')
ax.plot(x,y,".",color='red',label='Valores estimados Método de las potencias')

plt.legend()

plt.show()

fig.savefig("GraficoEstimacionesDominante.pdf")

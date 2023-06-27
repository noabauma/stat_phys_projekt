import matplotlib.pyplot as plt
import numpy as np
import sys

#$ python3 input.txt output.pdf (.png)
y, x1, x2 = np.loadtxt(sys.argv[1], unpack=True)
plt.plot(y,x1, label='DFTB')
plt.plot(y,x2, label='PM6')

plt.xlabel('Zeitschritte')
plt.ylabel(r"$E_{kin}$")
plt.title('Diels-Alder-Reaktion über Zeit')
plt.legend()
plt.grid()
plt.savefig(sys.argv[2])

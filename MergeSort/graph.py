import os
import subprocess
from termcolor import colored
import matplotlib.pyplot as plt

def print_cyan(str):
    print(colored(str, "cyan"))
os.system("cmake . -B build/")

size = str(10000000)
os.chdir("build")
print_cyan("make")
os.system("make")
arr = []
myrange = range(1, 25)

for i in myrange:
    cmd = "mpirun -np " + str(i) + " ./main " + size
    result = subprocess.check_output(cmd, shell=True, text=True)
    print("num = %s: time = %ss" % (str(i), result))
    arr.append(float(result))
print(arr)

plt.figure(figsize=[12, 5], dpi=100)
plt.plot(list(myrange), [arr[0] / i for i in arr], linewidth=2, label='y1')

plt.xlabel("number of processors")
plt.ylabel("Acceleration")
plt.title("Acceleration on the machine")
plt.minorticks_on()
plt.grid()
plt.show()
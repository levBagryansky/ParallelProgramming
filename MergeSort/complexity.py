import os
import subprocess
from termcolor import colored
import matplotlib.pyplot as plt

def print_cyan(str):
    print(colored(str, "cyan"))
os.system("cmake . -B build/")

size = str(100)
threads= 8
os.chdir("build")
print_cyan("make")
os.system("make")
resultes = []
sizes = []
while int(size) < 5 * (10 ** 7):
    cmd = "mpirun -np " + str(threads) + " ./main " + size
    result = subprocess.check_output(cmd, shell=True, text=True)
    print("size of array = %s: time = %ss" % (size, result))
    sizes.append(int(size))
    resultes.append(float(result))
    if int(size) < 10 ** 6:
        size = str(int(float(size) * 1.5))
    else:
        size = str(int(float(size) * 1.1))

# myrange = range(1, 50)
# for i in myrange:
#     cmd = "mpirun -np " + str(i) + " ./main " + size
#     result = subprocess.check_output(cmd, shell=True, text=True)
#     print("num = %s: time = %ss" % (str(i), result))
#     arr.append(float(result))
# print(arr)

plt.figure(figsize=[12, 5], dpi=100)
plt.plot(list(sizes), resultes, linewidth=2, label='y1')

plt.xlabel("size of array")
plt.ylabel("time(s)")
plt.title("Complexity")
plt.minorticks_on()
plt.grid()
plt.show()

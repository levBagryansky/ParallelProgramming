import os
import subprocess
from termcolor import colored
import matplotlib.pyplot as plt

def print_cyan(str):
    print(colored(str, "cyan"))
os.system("cmake . -B build/")

size = str(10)
threads= 5
os.chdir("build")
print_cyan("make")
os.system("make")
resultes = []
sizes = []
while int(size) < 2500:
    cmd = "mpirun -np " + str(threads) + " ./Parallel " + size + " " + size
    result = subprocess.check_output(cmd, shell=True, text=True)
    print("size of matrix = %s: time = %ss" % (size, result))
    sizes.append(int(size) ** 2)
    resultes.append(float(result))
    size = str(int(size) + 15)

plt.figure(figsize=[12, 5], dpi=100)
plt.plot(list(sizes), resultes, linewidth=2, label='y1')

plt.xlabel("size of matrix (x * x)")
plt.ylabel("time(s)")
plt.title("Complexity")
plt.minorticks_on()
plt.grid()
current_dir = os.getcwd()
os.chdir("../")
if(not os.path.exists("graphs")):
    os.mkdir("graphs")
os.chdir("graphs")
print_cyan(f"Saving graph to {current_dir}/Complexity.png")
plt.savefig('Complexity.png')
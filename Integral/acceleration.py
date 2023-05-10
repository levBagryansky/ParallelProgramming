import os
import subprocess
from termcolor import colored
import matplotlib.pyplot as plt
import numpy as np

def print_cyan(str):
    print(colored(str, "cyan"))
os.system("cmake . -B build/")

threads = 1
os.chdir("build")
print_cyan("make")
os.system("make")
accelerations = []
efficiencies = []
sizes = []
threads = 1

cmd = "./Integral " + str(threads) +  " 0.000000001"
result = subprocess.check_output(cmd, shell=True, text=True)
sizes.append(threads)
accelerations.append(1.0)
efficiencies.append(1.0)
seq = float(result)
threads = 2
while threads < 60:
    cmd = "./Integral " + str(threads) +  " 0.000000001"
    result = subprocess.check_output(cmd, shell=True, text=True)
    print("threads = %s: time = %ss" % (str(threads), result))
    sizes.append(threads)
    accelerations.append(seq / float(result))
    efficiencies.append(seq / (float(result) * threads))
    threads += 1

plt.figure(figsize=[12, 5], dpi=100)
plt.plot(list(sizes), accelerations, '-o', markersize=4, linewidth=2, label='y1', color = np.random.rand(3))

plt.xlabel("num of threads")
plt.ylabel("time / seq_time")
plt.title("Acceleration")
plt.minorticks_on()
plt.grid()
os.chdir("../")
if(not os.path.exists("graphs")):
    os.mkdir("graphs")
os.chdir("graphs")
current_dir = os.getcwd()
print_cyan(f"Saving graph to {current_dir}/Acceleration.png")
plt.savefig('Acceleration.png')

plt.figure(figsize=[12, 5], dpi=100)
plt.plot(list(sizes), efficiencies, '-o', markersize=4, linewidth=2, label='y1', color = np.random.rand(3))
plt.xlabel("num of threads")
plt.ylabel("time / (seq_time * processes)")
plt.title("Efficiency")
plt.minorticks_on()
plt.grid()
current_dir = os.getcwd()
print_cyan(f"Saving graph to {current_dir}/Efficiency.png")
plt.savefig('Efficiency.png')

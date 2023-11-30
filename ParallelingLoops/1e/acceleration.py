import os
import subprocess
import sys
from termcolor import colored
import matplotlib.pyplot as plt
import numpy as np


def print_cyan(str):
    print(colored(str, "cyan"))


def main() -> int:
    os.system("cmake . -B build/")
    os.chdir("build")
    print_cyan("make")
    os.system("make")
    accelerations = []
    efficiencies = []
    sizes = []
    processes = 1

    cmd = "./Sequential"
    print_cyan(cmd)
    result = subprocess.check_output(cmd, shell=True, text=True)
    sizes.append(processes)
    accelerations.append(1.0)
    efficiencies.append(1.0)
    seq = float(result)
    processes = 2

    while processes < 32:
        cmd = "mpirun -np " + str(processes) + " --oversubscribe ./Parallel "
        print_cyan(cmd)
        result = subprocess.check_output(cmd, shell=True, text=True)
        print("   processes = %s: time = %ss" % (str(processes), result))
        sizes.append(processes)
        accelerations.append(seq / float(result))
        efficiencies.append(seq / (float(result) * processes))
        processes += 1

    plt.figure(figsize=[12, 5], dpi=100)
    plt.plot(list(sizes), accelerations, '-o', markersize=4, linewidth=2, label='y1', color=np.random.rand(3))

    plt.xlabel("num of procs")
    plt.ylabel("time / seq_time")
    plt.title("Acceleration")
    plt.minorticks_on()
    plt.grid()
    os.chdir("../")
    if not os.path.exists("graphs"):
        os.mkdir("graphs")
    os.chdir("graphs")
    current_dir = os.getcwd()
    print_cyan(f"Saving graph to {current_dir}/Acceleration.png")
    plt.savefig('Acceleration.png')

    plt.figure(figsize=[12, 5], dpi=100)
    plt.plot(list(sizes), efficiencies, '-o', markersize=4, linewidth=2, label='y1', color=np.random.rand(3))
    plt.xlabel("num of procs")
    plt.ylabel("time / (seq_time * processes)")
    plt.title("Efficiency")
    plt.minorticks_on()
    plt.grid()
    current_dir = os.getcwd()
    print_cyan(f"Saving graph to {current_dir}/Efficiency.png")
    plt.savefig('Efficiency.png')
    return 0


if __name__ == '__main__':
    sys.exit(main())

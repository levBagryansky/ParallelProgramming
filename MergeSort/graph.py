import os
import subprocess
from termcolor import colored

def print_cyan(str):
    print(colored(str, "cyan"))
os.system("cmake . -B build/")

size = str(1000000)
os.chdir("build")
print_cyan("make")
os.system("make")
arr = []
for i in range (1, 20):
    cmd = "mpirun -np " + str(i) + " ./main " + size
    result = subprocess.check_output(cmd, shell=True, text=True)
    print("num = %s: time = %s" % (str(i), result))
    arr.append(float(result))
print(arr)

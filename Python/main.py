
import subprocess
import time
import shlex
import os
import pandas as pd
from datetime import datetime
from copy import deepcopy

from constantes import *

cut_param = [0, 1, 2, 3, 4, 6, 9]

corte_array = [("corte " + str(i)) for i in cut_param]

corte_array[4] = "conjectura"
corte_array[6] = "todos"

commands_per_file = [{
    "file": file,
    "cortes": ["../CPP/main -v aa -c {} -t 1 -i ../CPP/instances/{}".format(c, file) for c in cut_param]
} for file in os.listdir("../CPP/instances")]


timeout = 3600  # Em segundos => 1 hora tem 60 minutos que por sua vez tem 60 segundos

folder_name = str(time.ctime())


def executeCpp(command_line, timeout):

    args = shlex.split(command_line)
    print(args)

    start = time.time()
    try:
        p = subprocess.Popen(args, stdout=subprocess.DEVNULL)
        # p = subprocess.Popen(args)
        p.wait(timeout)
        delta = time.time() - start
        print("")
        return delta
    except subprocess.TimeoutExpired:
        # Terminate the process with SIGTERM
        p.terminate()

        # Terminate the process with SIGKILL
        # p.kill()
        print(RED, command_line, "process was terminated with ",
              RESET, "\n")
        return "ttl"


def mainLoop(timeout):

    # Create the directory with all benchmarks
    os.mkdir(f"./Data/{folder_name}")

    # Create a resume file with resume of the benchmark
    with open(f"./Data/{folder_name}/Resume.txt", "w+") as file:
        file.write("Resume\n\n")
        file.write(f"The benckmark started day {folder_name}.\n")
        file.write(f"Time to live of {timeout} seconds.\n")
        file.write(f"Cut parameters {cut_param}.\n")

    data = deepcopy(commands_per_file)
    for i in range(0, len(commands_per_file)):

        data[i]["cortes"] = [executeCpp(cmd, timeout)
                             for cmd in data[i]["cortes"]]

        data[i]["cortes"] = [task for task in data[i]["cortes"]]

        df = pd.DataFrame(data)
        # print(df.head(6))
        df2 = pd.DataFrame(df)
        df2[corte_array] = pd.DataFrame(
            df2.cortes.values.tolist(), index=df2.index)
        # print(df2.head(6))
        df2.drop(columns="cortes", inplace=True)
        # print(df2.head(6))
        df2.to_csv("./Data/" + folder_name + "/" +
                   str(i+1) + " " + str(time.ctime()) + ".csv")
        '''
        print(f"./Data/{folder_name}/{i+1} {time.ctime()}.csv")
        '''
    return data


    # Driver function
if __name__ == "__main__":

    '''
    print("This code executed in:", executeCpp(
        "../CPP/main -v aa -c 6 -t 1 -i ../CPP/instances/ins_05_20_4.txt", 120), "seconds.")
    '''

    try:
        data = mainLoop(timeout)
        df = pd.DataFrame(data)
        print(df.head(6))
        df2 = pd.DataFrame(df)
        df2[corte_array] = pd.DataFrame(
            df2.cortes.values.tolist(), index=df2.index)
        print(df2.head(6))
        df2.drop(columns="cortes", inplace=True)
        print(df2.head(6))
        df2.to_csv(f"./Data/{folder_name}/FINAL.csv")
    finally:
        print(GREEN, '\nCode Finished\n', RESET)
        with open(f"./Data/{folder_name}/Resume.txt", "a") as file:
            file.write(f"The benckmark ended day {time.ctime()}\n")
            with open(f"./main.py", 'r') as code:
                space = '#'*50
                file.write(f"\n{space}\nCODE\n{space}\n{code.read()}")
    # '''

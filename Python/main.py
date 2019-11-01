
import subprocess
import time
import shlex
import os
import pandas as pd
from datetime import datetime
from copy import deepcopy

from constantes import *

commands_per_file = [{
    "file": file,
    "cortes": ["../CPP/main -v aa -c {} -t 1 -i ../CPP/instances/{}".format(c, file) for c in [0, 2, 6]]
} for file in os.listdir("../CPP/instances")]

timeout = 10  # Segundos


def executeCpp(command_line, timeout):

    args = shlex.split(command_line)
    print(args)

    start = time.time()
    try:
        p = subprocess.Popen(args, stdout=subprocess.PIPE)
        p.wait(timeout)
        delta = time.time() - start
        print("")
        return delta
    except subprocess.TimeoutExpired:
        # Terminate the process with SIGTERM
        p.terminate()

        # Terminate the process with SIGKILL
        # p.kill()
        print(RED, command_line, "process was terminated with delta = ",
              delta,  RESET, "\n")
        return "ttl"


def mainLoop():
    folder_name = str(time.ctime())

    os.mkdir("./Data/" + folder_name)

    data = deepcopy(commands_per_file)
    for i in range(0, len(commands_per_file)):

        data[i]["cortes"] = [executeCpp(cmd, 120) for cmd in data[i]["cortes"]]

        data[i]["cortes"] = [task for task in data[i]["cortes"]]

        df = pd.DataFrame(data)
        # print(df.head(6))
        df2 = pd.DataFrame(df)
        df2[['corte 0 ', 'corte 2', "corte 6"]] = pd.DataFrame(
            df2.cortes.values.tolist(), index=df2.index)
        # print(df2.head(6))
        df2.drop(columns="cortes", inplace=True)
        # print(df2.head(6))
        df2.to_csv("./Data/" + folder_name + "/" + str(time.ctime()) + ".csv")

    return data

    # Driver function
if __name__ == "__main__":
    # print(executeCpp("../CPP/main -v vv -c 0 -t 1 -i ../CPP/instances/ins_05_20_5.txt"))
    mainLoop()
    df = pd.DataFrame(data)
    print(df.head(6))
    df2 = pd.DataFrame(df)
    df2[['corte 0 ', 'corte 2', "corte 6"]] = pd.DataFrame(
        df2.cortes.values.tolist(), index=df2.index)
    print(df2.head(6))
    df2.drop(columns="cortes", inplace=True)
    print(df2.head(6))
    df2.to_csv("./Data/" + str(time.ctime()) + ".csv")

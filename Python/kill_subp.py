#%%
import asyncio
import pandas as pd
import time
from datetime import datetime
import os
from copy import deepcopy

'''
commands_per_file = [{
    "file": file,
    "cortes": ["./mock_main.py -v aa -c {} -t 1 -i {}".format(c, file) for c in [0, 2, 6]]
} for file in os.listdir("./files")]
'''


commands_per_file = [{
    "file": file,
    "cortes": ["../CPP/main -v aa -c {} -t 1 -i ../CPP/instances/{}".format(c, file) for c in [0, 2, 6]]
} for file in os.listdir("../CPP/instances")]

sem = asyncio.Semaphore(5)

async def spawn(cmd):
    async with sem:
        proc = await asyncio.create_subprocess_shell(
        cmd,
        stdout=asyncio.subprocess.PIPE,
        stderr=asyncio.subprocess.PIPE)
        stdout, _ = await proc.communicate()
        return stdout


async def run_command(cmd, ttl):
    """
        Dado um comando retorna tempo de execucao ou "ttl"
    """
    start = time.time()
    try:
        await asyncio.wait_for(spawn(cmd), timeout=ttl)
        delta = time.time() - start
        return delta
    except asyncio.TimeoutError:
        return "ttl"


async def main_original():
    for i in range(0, len(commands_per_file)):
        commands_per_file[i]["cortes"] = [asyncio.create_task(
            run_command(cmd, 0.5)) for cmd in commands_per_file[i]["cortes"]]

        commands_per_file[i]["cortes"] = [await task for task in commands_per_file[i]["cortes"]]

    print(commands_per_file)


async def main():
    folder_name = str(time.ctime())
    
    os.mkdir("./Data/" + folder_name)

    data = deepcopy(commands_per_file)
    for i in range(0, len(commands_per_file)):
        data[i]["cortes"] = [asyncio.create_task(
            run_command(cmd, 120)) for cmd in data[i]["cortes"]]

        data[i]["cortes"] = [await task for task in data[i]["cortes"]]
    
        df = pd.DataFrame(data)
        #print(df.head(6))
        df2 = pd.DataFrame(df)
        df2[['corte 0 ','corte 2', "corte 6"]] = pd.DataFrame(df2.cortes.values.tolist(), index= df2.index)
        #print(df2.head(6))
        df2.drop(columns="cortes", inplace=True)
        #print(df2.head(6))
        df2.to_csv("./Data/" + folder_name + "/" + str(time.ctime()) + ".csv")

    return data

data = asyncio.run(main())
#df = pd.DataFrame(commands_per_file)
df = pd.DataFrame(data)
print(df.head(6))
df2 = pd.DataFrame(df)
df2[['corte 0 ','corte 2', "corte 6"]] = pd.DataFrame(df2.cortes.values.tolist(), index= df2.index)
print(df2.head(6))
df2.drop(columns="cortes", inplace=True)
print(df2.head(6))
df2.to_csv("./Data/" + str(time.ctime()) + ".csv")

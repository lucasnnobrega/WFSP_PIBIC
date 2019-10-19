import signal
import random
import time
import os


class MyTimer():

    def __init__(self):
        self.start = time.time()

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        end = time.time()
        runtime = end - self.start
        msg = 'The function took {time} seconds to complete'
        print(msg.format(time=runtime))


def long_runner():
    for x in range(5):
        sleep_time = random.choice(range(1, 5))
        time.sleep(sleep_time)


def WFSP():
    os.system("../CPP/main -v aa -c 9 -t 1 -i ../CPP/instances/ins_05_15_1.txt")


def signal_handler(signum, frame):
    raise Exception("Timed out!")


if __name__ == '__main__':
    signal.signal(signal.SIGALRM, signal_handler)
    signal.alarm(5)   # Ten seconds
    try:
        with MyTimer():
            WFSP()
    except Exception:
        print("Timed out!")

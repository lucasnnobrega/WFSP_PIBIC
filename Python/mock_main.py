#! /usr/bin/python3

from random import randrange
import argparse
from time import sleep, time
import sys

parser = argparse.ArgumentParser(description="Mock processo lento")
parser.add_argument("--verbose", "-v", type=str)
parser.add_argument("--cortes", "-c", type=int)
parser.add_argument("--t", "-t", type=int)
parser.add_argument("--input", "-i", type=str)

args = parser.parse_args()

sleep(randrange(1, 10)/10)

print("Arquivo: {}, cortes: {}".format(args.input, args.cortes))

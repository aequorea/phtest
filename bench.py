#!/usr/bin/python
#
# bench.py -- benchmark insertion sort
#
# 20190119 -- first version

cc = "gcc"
cpp = "g++"

import os

def prGreen(skk): print("\033[92m {}\033[00m" .format(skk)) 
def prYellow(skk): print("\033[93m {}\033[00m" .format(skk)) 
def prCyan(skk): print("\033[96m {}\033[00m" .format(skk)) 

def do_cmd(cmd):
#    prGreen("$ " + cmd)
    val = os.system(cmd)
    if val == 0:
        return
    prYellow("\nCommand Failed !!")
    exit()

do_cmd("lscpu | grep \"Model name:\" | \
        sed \"s/Model name:          //\"")
do_cmd(cc +" --version | grep \"" + cc +"\"")

do_cmd(cc + " -O3 -c sorter.c hsort.c")
do_cmd(cpp + " -O3 -c cppsort.cpp")
do_cmd(cpp + " -o sorter sorter.o hsort.o cppsort.o")
do_cmd("./sorter")
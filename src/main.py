#!/usr/bin/env python
import mpi4py
import ctypes as ct

def main():
  # Import DSO
  lib = ct.CDLL("../build/libmdlib.so")
  return

if __name__ == "__main__": main()
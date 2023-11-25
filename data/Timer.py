import os
import re
import sys
m = re.compile(r"^(?P<name>\w+):\s(?P<calls>\d+)")
n = re.compile(r"(\d+) ")
M = [[[0 for _ in range(int(sys.argv[3]))]
         for _ in range(int(sys.argv[2]))]
         for _ in range(int(sys.argv[4]))]
x = 0
y = 0
for i in sorted(os.listdir(sys.argv[1]), reverse=False):
  if i.startswith("out"):
    Timing = {}
    with open(os.path.join(sys.argv[1], i), 'r') as fr:
      L = fr.readline()
      while (re.match("TIMING RESULTS:")):
        L = fr.readline()
      L = fr.readlines()
    name = ""
    for l in L:
      a = m.match(l)
      if a:
        Timing.setdefault(a.group("name"), [int(a.group("calls")), 0])
        name = a.group("name")
      a = [int(i) for i in n.findall(l)]
      if a:
        a = max(a)
        if Timing[name][1] < a:
          Timing[name][1] = a
    for l, j in enumerate(Timing.items()):
      M[l][y][x] = j[1][1]
    print(y, x)
    x += 1
    y += int(x / 3)
    x %= 3
for i in range(int(sys.argv[4])):
  for j in range(int(sys.argv[2])):
    for k in reversed(range(int(sys.argv[3]))):
      print(M[i][j][k], end=" ")
    print("")
  print("")
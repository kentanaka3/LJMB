import os
import re
import sys
atom = re.compile( r"^Starting simulation with (?P<atoms>\d+)")
z = 0
m = re.compile(r"^(?P<name>\w+):\s(?P<calls>\d+)")
n = re.compile(r"(\d+) *")
o = re.compile(r"\w+_(?P<node>\d+)_(?P<thread>).out")
x = 0
y = 0
M = {}
for arg in range(1, len(sys.argv)):
  pathname = os.path.join("Timer", arg)
  for i in sorted(os.listdir(pathname), reverse=False):
    if i.endswith("out"):
      node = 0
      thread = 0
      files = o.match(i)
      if files:
        node = files.group("node")
        thread = files.group("thread")
        M.setdefault((node, thread), dict())
      Timing = {}
      z = 0
      with open(os.path.join(pathname, i), 'r') as fr:
        L = fr.readline()
        while (not re.match(L, "- TIMING RESULTS -\n")):
          atoms = atom.match(L)
          if atoms:
            z = int(atoms.group("atoms"))
            M[(node, thread)][z] = dict()
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
      M[(node, thread)][z] = Timing
  x = list(M[(node, thread)].keys())
  y = [0]*len(x)
  titles = [title for title in M[(node, thread)][x[0]]]
  for t in titles:
    with open(os.path.join(pathname, t + ".dat"), 'w') as fr:
      fr.write("# " + t + "\n")
  for t in titles:
    with open(os.path.join(pathname, t + ".dat"), 'a') as fr:
      for i, j in enumerate(x):
        y[i] = M[(node, thread)][j][t][1]/M[(node, thread)][j][t][0]
        fr.write(str(j) + "\t" + str(y[i]) + "\n")

for t in titles:
  os.system("gnuplot -c Timer.plt " + t + " " + " ".join(sys.argv[1:]))
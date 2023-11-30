import os
import re
import sys
atom = re.compile( r"^Starting simulation with (?P<atoms>\d+)")
z = 0
m = re.compile(r"^(?P<name>\w+):\s(?P<calls>\d+)")
n = re.compile(r"(\d+) *")
x = 0
y = 0
M = {}
pathname = os.path.join("Timer", sys.argv[1])
for i in sorted(os.listdir(pathname), reverse=False):
  if i.endwithwith(".out"):
    Timing = {}
    with open(os.path.join(pathname, i), 'r') as fr:
      L = fr.readline()
      while (not re.match(L, "- TIMING RESULTS -\n")):
        atoms = atom.match(L)
        if atoms:
          z = int(atoms.group("atoms"))
          M[z] = dict()
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
    M[z] = Timing
x = list(M.keys())
y = [0]*len(x)
titles = [title for title in M[x[0]]]
for t in titles:
  with open(os.path.join(pathname, t + ".dat"), 'w') as fr:
    fr.write("# " + t + "\n")
for t in titles:
  with open(os.path.join(pathname, t + ".dat"), 'a') as fr:
    for i, j in enumerate(x):
      y[i] = M[j][t][1]/M[j][t][0]
      fr.write(str(j) + "\t" + str(y[i]) + "\n")

for t in titles:
  os.system("gnuplot -c Timer.plt " + t + " " + sys.argv[1])

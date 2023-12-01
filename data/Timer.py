import os
import re
import sys
atom = re.compile( r"^Starting simulation with (?P<atoms>\d+)")
z = 0
m = re.compile(r"^(?P<name>\w+):\s(?P<calls>\d+)")
n = re.compile(r"(\d+) *")
o = re.compile(r"\d+_(?P<node>\d+)_(?P<tasks>\d+)_(?P<thread>\d+).out")
x = 0
y = 0
for arg in range(1, len(sys.argv)):
  M = {}
  pathname = os.path.join("Timer", sys.argv[arg])
  for i in sorted(os.listdir(pathname), reverse=False):
    if i.endswith(".out"):
      key = ""
      files = o.match(i)
      if files:
        key = "_" + files.group("node") + \
              "_" + files.group("tasks") + \
              "_" + files.group("thread")
        M.setdefault(key, dict())
      Timing = {}
      z = 0
      with open(os.path.join(pathname, i), 'r') as fr:
        L = fr.readline()
        while (not re.match(L, "- TIMING RESULTS -\n")):
          atoms = atom.match(L)
          if atoms:
            z = int(atoms.group("atoms"))
            M[key][z] = dict()
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
      M[key][z] = Timing

  """
  if len(M.keys()) > 1:
    x = []
    y = []
    z = []
    for a in M.keys():
      files = o.match(a)
      if files:
        x.append(int(files.group("node")))
        y.append(int(files.group("tasks")))
        z.append(int(files.group("thread")))
    titles = [title for title in M[a][x[0]]]
    for t in titles:
      with open(os.path.join(pathname, t + a + "_tk.dat"), 'w') as fr:
        fr.write("# " + t + "\n")
    for t in titles:
      with open(os.path.join(pathname, t + a + "_tk.dat"), 'a') as fr:
        for i, j in enumerate(x):
          y[i] = M[a][j][t][1]/M[a][j][t][0]
          fr.write(str(j) + "\t" + str(y[i]) + "\n")
  """

  for a in M.keys():
    x = list(M[a].keys())
    y = [0]*len(x)
    titles = [title for title in M[a][x[0]]]
    for t in titles:
      with open(os.path.join(pathname, t + a + "_sz.dat"), 'w') as fr:
        fr.write("# " + t + "\n")
    for t in titles:
      with open(os.path.join(pathname, t + a + "_sz.dat"), 'a') as fr:
        for i, j in enumerate(x):
          y[i] = M[a][j][t][1]/M[a][j][t][0]
          fr.write(str(j) + "\t" + str(y[i]) + "\n")

  for t in titles:
    cmd = f"gnuplot -c Timer.plt {t} {sys.argv[arg]} \"{' '.join(M.keys())}\" Size"
    print(cmd)
    os.system(cmd)
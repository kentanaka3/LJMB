import os
import re
import sys
atom = re.compile( r"^Starting simulation with (?P<atoms>\d+)")
z = 0
m = re.compile(r"^(?P<name>\w+):\s(?P<calls>\d+)")
n = re.compile(r"(\d+) *")
o = re.compile(r"\d+_(?P<node>\d+)_(?P<tasks>\d+)_(?P<thread>\d+)")
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

  if len(M.keys()) > 1:
    x = set()
    y = set()
    z = set()
    for a in M.keys():
      files = a.split("_")
      x.add(files[1])
      y.add(files[2])
      z.add(files[3])
    if len(y) == 1:
      # OpenMP
      z = sorted(list(z))
      for i in z:
        a = "_".join(["", str(list(x)[0]), str(list(y)[0]), i])
        for sz in M[a].keys():
          for t in M[a][sz]:
            with open(os.path.join(pathname, f"{t}_{sz}_tk.dat"), 'w') as fr:
              fr.write("# " + t + "\n")
      for i in z:
        a = "_".join(["", str(list(x)[0]), str(list(y)[0]), i])
        for sz in M[a].keys():
          for t in M[a][sz]:
            with open(os.path.join(pathname, f"{t}_{sz}_tk.dat"), 'a') as fr:
              fr.write(f"{i}\t{M[a][sz][t][1]/M[a][sz][t][0]}\n")
              print(i, M[a][sz][t][1]/M[a][sz][t][0])
    elif len(z) == 1:
      # MPI
      y = sorted(list(y))
      for i in y:
        a = "_".join(["", str(list(x)[0]), i, str(list(z)[0])])
        for sz in M[a].keys():
          for t in M[a][sz]:
            with open(os.path.join(pathname, f"{t}_{sz}_tk.dat"), 'w') as fr:
              fr.write("# " + t + "\n")
      for i in y:
        a = "_".join(["", str(list(x)[0]), i, str(list(z)[0])])
        for sz in M[a].keys():
          for t in M[a][sz]:
            with open(os.path.join(pathname, f"{t}_{sz}_tk.dat"), 'a') as fr:
              fr.write(f"{i}\t{M[a][sz][t][1]/M[a][sz][t][0]}\n")
              print(i, M[a][sz][t][1]/M[a][sz][t][0])
    else:
      # MPI + OpenMP
      y = sorted(list(y))
      z = sorted(list(z))
      for i in y:
        for j in z:
          a = "_".join(["", str(list(x)[0]), i, j])
          if a in M:
            for sz in M[a].keys():
              for t in M[a][sz]:
                with open(os.path.join(pathname, f"{t}_{sz}_tk.dat"), 'w') as fr:
                  fr.write("")
      for i in y:
        for j in z:
          a = "_".join(["", str(list(x)[0]), i, j])
          if a in M:
            for sz in M[a].keys():
              for t in M[a][sz]:
                with open(os.path.join(pathname, f"{t}_{sz}_tk.dat"), 'a') as fr:
                  fr.write(f"{M[a][sz][t][1]/M[a][sz][t][0]}\t")
                  print(i, j, M[a][sz][t][1]/M[a][sz][t][0])
          else:
            a = "_".join(["", str(list(x)[0]), i, z[0]])
            for sz in M[a].keys():
              for t in M[a][sz]:
                with open(os.path.join(pathname, f"{t}_{sz}_tk.dat"), 'a') as fr:
                  fr.write(f"NaN\t")
        a = "_".join(["", str(list(x)[0]), i, z[0]])
        for sz in M[a].keys():
          for t in M[a][sz]:
            with open(os.path.join(pathname, f"{t}_{sz}_tk.dat"), 'a') as fr:
              fr.write(f"\n")

  for a in M.keys():
    x = list(M[a].keys())
    y = [0]*len(x)
    for t in M[a][x[0]]:
      with open(os.path.join(pathname, t + a + "_sz.dat"), 'w') as fr:
        fr.write("# " + t + "\n")
    for t in M[a][x[0]]:
      with open(os.path.join(pathname, t + a + "_sz.dat"), 'a') as fr:
        for i, j in enumerate(x):
          y[i] = M[a][j][t][1]/M[a][j][t][0]
          fr.write(str(j) + "\t" + str(y[i]) + "\n")

os.system("make plot")
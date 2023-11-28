V_LJ(r) = 4ϵ[(σ/r)¹² - (σ/r)⁶]
V_Morse(r) = D[1 - e^(-a(r - re))]²
Fx_Morse(r) = 2 a D e^(-a(r - r₀)) [1 - e^(-a(r - r₀))]² rx

The potential energy and the force functions are modified to fit with the Morse potential.
Rather than the use of Epsilon and Sigma, we define 4 new parameters. As in argon_108.inp, they are given the values
-0.055  # m
0.054531221999255   # De
2.330519837590604186  # a
3.839 # re
Another set of values can be seen in argon_2916.inp
-0.45             # m
0.04395           # De
2.0419            # a
3.8979            # re
They are assumed after a manual curve-fitting for a set of 10000 points ranging between r=1->12. I couldn't find them in literature "Mores parameter values for a system of pure Argon atoms"


In comparison with the reference data, they don't provide close output enrgies. It's a matter of parameter values.

The serial code is relatively slow, do to the use of exponential function and sqrt.
I couldn't manage so far to alternate them, but you can give your ideas there
      r = sqrt(rx*rx + ry*ry + rz*rz);
      MM = exp(- sys->a * (r - sys->re));
      sys->epot += sys->m + sys->De * (1-MM)*(1-MM);




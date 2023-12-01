set term png size 500,300
set logscale
set grid
set title ARG2." ".ARG1
set ylabel "Time (ms)"
set format y "10^{%T}"
if (ARG4 eq "Size") {
  set output "../img/".ARG2."_".ARG1."_sz.png"
  set format x "10^{%T}"
  set xrange [100:100000]
  set xlabel "System Size"
  plot for [i=1:words(ARG3)] "Timer/".ARG2."/".ARG1.word(ARG3, i)."_sz.dat" w lp title word(ARG3, i)
} else {
  set output "../img/".ARG2."_".ARG1."_tk.png"
  set xrange [1:40]
  set xlabel "# Tasks"
  plot for [i=1:words(ARG3)] "Timer/".ARG2."/".ARG1.word(ARG3, i)."_tk.dat" w lp title word(ARG3, i)
}
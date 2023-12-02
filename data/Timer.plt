set term png size 600,400
set grid
set title ARG2." ".ARG1
set ylabel "Time (ms)"
set format y "10^{%T}"
if (ARG5 eq "Size") {
  set logscale
  set output "../img/".ARG2."_".ARG1."_sz.png"
  set format x "10^{%T}"
  set key top left
  set xrange [100:100000]
  set xlabel "System Size"
  plot for [i=1:words(ARG3)] "Timer/".ARG2."/".ARG1.word(ARG3, i)."_sz.dat" w lp title "Nd: ".substr(word(ARG3, i), 2, 3).", Tk: ".substr(word(ARG3, i), 5, 6).", Th:".substr(word(ARG3, i), 8, 9)
} else {
  set logscale y
  set output "../img/".ARG2."_".ARG1."_tk.png"
  set xrange [1:33]
  set xlabel "# Tasks"
  plot for [i=1:words(ARG3)] for [j=1:words(ARG4)] "Timer/".ARG2."/".ARG1."_".word(ARG4, j).word(ARG3, i)."_tk.dat" u 2:3 w lp title "Sz: ".substr(word(ARG3, i), 2, strlen(word(ARG3, i)))
}
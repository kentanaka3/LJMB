set term png size 500,300
set logscale
set grid
set title ARG1
set ylabel "Time (ms)"
set format y "10^{%T}"
set output "../img/SerialComp_".ARG1."_sz.png"
set format x "10^{%T}"
set xrange [100:100000]
set xlabel "System Size"
plot for [i=1:words(ARG2)] "Timer/".word(ARG2, i)."/".ARG1."_01_01_01_sz.dat" w lp title word(ARG2, i)
set term png
set output "../img/".ARG1.".png"
set logscale
plot "Timer/".ARG2."/".ARG1.".dat" w l title ARG2

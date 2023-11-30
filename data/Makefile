all: clean
	../build/MAIN.x < argon_108.inp
timing: clean
	bash Analyze.sh Leonardo argon_108.inp
	bash Analyze.sh Leonardo argon_2916.inp
	bash Analyze.sh Leonardo argon_78732.inp
imaging:
	
clean:
	rm -f *.dat
	rm -f *.xyz

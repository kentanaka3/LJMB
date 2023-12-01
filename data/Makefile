all: clean
	../build/MAIN.x < argon_108.inp
timing: clean
	bash Analyze.sh Leonardo argon_108.inp
	bash Analyze.sh Leonardo argon_2916.inp
	bash Analyze.sh Leonardo argon_78732.inp
organize:
	mv *_01_01_01.out Timer/Optimization
	mv *_01_01_*.out Timer/OpenMP
	mv *_01_*_01.out Timer/MPI
	mv *_01_*_*.out Timer/MPI_OpenMP
	rm *.err
plot:
	gnuplot -c SerialComp.plt Force "Serial Optimization"
	gnuplot -c SerialComp.plt KineticEnergy "Serial Optimization"
	gnuplot -c SerialComp.plt Propagate "Serial Optimization"
	gnuplot -c SerialComp.plt RunTime "Serial Optimization"
	gnuplot -c SerialComp.plt Velverlet "Serial Optimization"
clean:
	rm -f *.dat
	rm -f *.xyz

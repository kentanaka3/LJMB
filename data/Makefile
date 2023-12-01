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
	gnuplot -c Timer.plt Force "MPI" "_01_02_01 _01_04_01 _01_08_01 _01_16_01 _01_32_01" Size
	gnuplot -c Timer.plt KineticEnergy "MPI" "_01_02_01 _01_04_01 _01_08_01 _01_16_01 _01_32_01" Size
	gnuplot -c Timer.plt Propagate "MPI" "_01_02_01 _01_04_01 _01_08_01 _01_16_01 _01_32_01" Size
	gnuplot -c Timer.plt RunTime "MPI" "_01_02_01 _01_04_01 _01_08_01 _01_16_01 _01_32_01" Size
	gnuplot -c Timer.plt Velverlet "MPI" "_01_02_01 _01_04_01 _01_08_01 _01_16_01 _01_32_01" Size
	gnuplot -c Timer.plt Force "OpenMP" "_01_01_02 _01_01_04 _01_01_08 _01_01_16 _01_01_32" Size
	gnuplot -c Timer.plt KineticEnergy "OpenMP" "_01_01_02 _01_01_04 _01_01_08 _01_01_16 _01_01_32" Size
	gnuplot -c Timer.plt Propagate "OpenMP" "_01_01_02 _01_01_04 _01_01_08 _01_01_16 _01_01_32" Size
	gnuplot -c Timer.plt RunTime "OpenMP" "_01_01_02 _01_01_04 _01_01_08 _01_01_16 _01_01_32" Size
	gnuplot -c Timer.plt Velverlet "OpenMP" "_01_01_02 _01_01_04 _01_01_08 _01_01_16 _01_01_32" Size
	gnuplot -c Timer.plt Force "MPI" "_108 _2916 _78732" Task
	gnuplot -c Timer.plt KineticEnergy "MPI" "_108 _2916 _78732" Task
	gnuplot -c Timer.plt Propagate "MPI" "_108 _2916 _78732" Task
	gnuplot -c Timer.plt RunTime "MPI" "_108 _2916 _78732" Task
	gnuplot -c Timer.plt Velverlet "MPI" "_108 _2916 _78732" Task
	gnuplot -c Timer.plt Force "OpenMP" "_108 _2916 _78732" Task
	gnuplot -c Timer.plt KineticEnergy "OpenMP" "_108 _2916 _78732" Task
	gnuplot -c Timer.plt Propagate "OpenMP" "_108 _2916 _78732" Task
	gnuplot -c Timer.plt RunTime "OpenMP" "_108 _2916 _78732" Task
	gnuplot -c Timer.plt Velverlet "OpenMP" "_108 _2916 _78732" Task
clean:
	rm -f *.dat
	rm -f *.xyz

# PARGRIDCOMP
This repository is dedicated to the project of the course parallel and grid computing

Similar Projects: 
[1](https://github.com/szkocot/Laplacian-image-filtering-with-OpenMP-and-MPI-hybrid)
[2](https://github.com/angegonzalez/PC-Image-Filtering)
[3](https://github.com/a-gavriel/OpenMPI-filter/blob/master/src/main.c)
[4](https://github.com/hsouri/OpenMP-Filter)
[MPI Example](https://github.com/BogdanMarghescu/Image-Filtering-using-MPI/blob/main/image_filtering.c)


*Test Cases*:
With Parallelization
./filteropmp -g images/yard.bmp output/out.bmp
Thread ID: 0
Edit function took 0.058368 seconds to execute 

./filteropmp -g images/large.bmp output/out.bmp
Thread ID: 0
Edit function took 0.595392 seconds to execute 

./filteropmp -a images/yard.bmp output/out.bmp
Thread ID: 0
Edit function took 0.876835 seconds to execute 

Without Parallelization

./filter -g images/yard.bmp output/out.bmp
Edit function took 0.011223 seconds to execute 

./filter -g images/large.bmp output/out.bmp
Edit function took 0.339627 seconds to execute 

./filter -a images/yard.bmp output/out.bmp
Edit function took 0.011352 seconds to execute 

./filter -a images/large.bmp output/out.bmp
Edit function took 0.395397 seconds to execute

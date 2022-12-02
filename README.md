# PARGRIDCOMP
This repository is dedicated to the project of the course parallel and grid computing

## Similar Projects: 
[1](https://github.com/szkocot/Laplacian-image-filtering-with-OpenMP-and-MPI-hybrid)
[2](https://github.com/angegonzalez/PC-Image-Filtering)
[3](https://github.com/a-gavriel/OpenMPI-filter/blob/master/src/main.c)
[4](https://github.com/hsouri/OpenMP-Filter)
[MPI Example](https://github.com/BogdanMarghescu/Image-Filtering-using-MPI/blob/main/image_filtering.c)


## Test Cases:


**Without Parallelization**

- greyscale on small image

./filter -g images/yard.bmp output/out.bmp
Edit function took 0.006757 seconds to execute 

- greyscale on large image

./filter -g images/large.bmp output/out.bmp
Edit function took 0.353339 seconds to execute 

- all filters on small image

./filter -a images/yard.bmp output/out.bmp
Edit function took 0.011189 seconds to execute 

- all filters on large image

./filter -a images/large.bmp output/out.bmp
Edit function took 0.416066 seconds to execute




**With Parallelization**

- greyscale on small image

./filteropmp -g images/yard.bmp output/out.bmp
Thread ID: 0
Edit function took 0.079441 seconds to execute 

- greyscale on large image

./filteropmp -g images/large.bmp output/out.bmp
Thread ID: 0
Edit function took 0.590959 seconds to execute 

- all filters on small image

./filteropmp -a images/yard.bmp output/out.bmp
Thread ID: 0
Edit function took 0.887377 seconds to execute 

- all filters on large image

./filteropmp -a images/large.bmp output/out.bmp
Thread ID: 0
Segmentation fault (core dumped)

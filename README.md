# 479-project-2
<b> Parallel Quick Sort </b>

In this project, we implemented parallel quick sort with the use of MPI in C++. Here. It will generate random numbers for input. Our algorithm will divide the array into the sub array of fix size then, each processor will sort their own sub array with the recursive quick sort. At the end, it will merge all the sub array from different processors into a result array as output.

<b> Group members: </b>

Dhyey Desai    dhyeydesai@csu.fullerton.edu    CWID=885451609 <br>
Lency Lakhani  lencylakhani@csu.fullerton.edu  CWID=885196055 <br>
Darshit Desai  darshdesai05@csu.fullerton.edu  CWID=885516997 <br>


## How To Run Project
1. Download quick_sort_parallel_simple.cc file from github
2. In terminal, write mpic++ quick_sort_parallel_simple.cc -o quick_sort_parallel_simple
3. Then to execute code, mpirun -n 10<no of processor> quick_sort_parallel_simple


# Parallel Quick Sort Using MPI

In this project, we implemented parallel quick sort with the use of MPI in C++. Here. It will generate random numbers for input. Our algorithm will divide the array into the sub array of fix size then, each processor will sort their own sub array with the recursive quick sort. At the end, it will merge all the sub array from different processors into a result array as output.

## Problem

QuickSort is a well-known sorting algorithm due to its average-case time complexity of O(n log n). However, as datasets grow in size, sequential QuickSort may face performance challenges. Motivated by the need for efficient sorting in high-performance computing, we explore the parallelization of QuickSort to take advantage of multiple processing units and accelerate the sorting process.

## MPI Commands Used

- `MPI_Init`: Initializes MPI environment.
- `MPI_Comm_rank`: Retrieves the rank of the calling process in the communicator.
- `MPI_Comm_size`: Determines the number of processes in the communicator.
- `MPI_Status`: Provides information about a completed communication.
- `MPI_Send`: Sends a message to a specified process.
- `MPI_Recv`: Receives a message from a specified process.
- `MPI_Wtime`: Returns an elapsed time on the calling processor.
- `MPI_Finalize`: Terminates MPI execution environment

## How To Run Project

1. Download `quick_sort_parallel_simple.cc` file from github
2. In terminal, write command `mpic++ quick_sort_parallel_simple.cc -o quick_sort_parallel_simple`
3. Then to execute code, write command `mpirun -n <no of processor> quick_sort_parallel_simple`

## Dependencies

- MPI library

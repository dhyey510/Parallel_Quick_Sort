#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define SIZE 100


/*
    Partition - select pivot as middle of array
    and divide array in two parts: 
    	- lower than pivot
	- higher than pivot
*/
int partition(int *arr, int start, int end){
    int middle = floor((start+end)/2);
    int pivot = arr[middle]; // middle of an array is pivot
    int temp;

    // move pivot to the end
    temp=arr[middle];  
    arr[middle]=arr[end];
    arr[end]=temp;

    //start dividing array in two parts
    int i = (start - 1);

    for (int j=start; j <= end-1; j++){
        if(arr[j] < pivot){
            i++;
            temp=arr[i];  
            arr[i]=arr[j];
            arr[j]=temp;	
        }
    }

    // move pivot back
    temp=arr[i+1];  
    arr[i+1]=arr[end];
    arr[end]=temp; 

    return (i+1);
}


/*
    Sequential Quicksort
*/
void quicksort(int *number,int first,int last){
    if(first<last){
        int pivot_index = partition(number, first, last);
        quicksort(number,first,pivot_index);
        quicksort(number,pivot_index+1,last);
    }
}


/*
    merge - it will merge two sorted subarray
    and store in resulting array
*/
void merge(int *first, int *second, int *result, int first_array_size, int second_array_size){
	int i=0;
	int j=0;
	int k=0;
	
	while(i<first_array_size && j<second_array_size){

		if (first[i]<second[j]) {
			result[k]=first[i];
			k++;
			i++;
		}else{
			result[k]=second[j];
			k++;
			j++;
		}

		if(i == first_array_size){
			// if the second array is larger than first array or first array is sorted
			while(j<second_array_size){
				result[k]=second[j];
				k++;
				j++;
			}	
		} else if (j == second_array_size){
			// if the first array is larger than second or second array is sorted
			while(i < first_array_size){
				result[k]=first[i];
				i++;
				k++;
			}
		}
		
	}	
}

int main(int argc, char *argv[]) {
	
    int *local_unsorted_array = (int *)malloc(SIZE * sizeof(int));
    int *result = (int *)malloc(SIZE * sizeof(int));
    int size, rank;
    int sub_array_size;

    srand(time(0));
            
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    MPI_Status status;

    if(rank==0){
    	// --- Random Number Generator ---
	std::cout<<"Before sorting Array: " << std::endl;
    	for (int j = 0; j < SIZE; j++) {
        	local_unsorted_array[j] = rand() % 1000;
		std::cout << local_unsorted_array[j] << std::endl;
    	}
       
     }
	
     int processor_count = size; 
	
     // Determine the size of the subarray each Processor receives
     sub_array_size = SIZE / processor_count;

     // Determine extra elements after dividing subarray to all processor
     int extraBit = SIZE % processor_count;

     /* 
        Processor 0 divide array into subarray and send it to all remaining processors. 
        Also, do sorting of its own subarray 
	and at last, merge all subarray received from different processors. 
	
     */
	
     if( rank == 0 ){

	     double start_time;

	     start_time=MPI_Wtime();

	     if(processor_count > 1){

		     // -- Send subarray to all processors --

		     for(int i=0;i<processor_count-1;i++){

			     int j;

			     /* 
			        processor 0 sort subarray from (0 to subarraysize + extraelement) 
			        after that divide remaining subarray to all processor
			     */

			     MPI_Send(&local_unsorted_array[(i+1)*sub_array_size + extraBit],sub_array_size,MPI_INT,i+1,0,MPI_COMM_WORLD); 

		     }
		

		     // -- Sort own subarray --

		     int *sub_array = (int *)malloc((sub_array_size + extraBit) * sizeof(int));

		     for(int i=0; i < (sub_array_size + extraBit); i++){


			     sub_array[i]=local_unsorted_array[i];

		     }

		     // Use quicksort sequential to sort array

		     quicksort(sub_array,0,(sub_array_size + extraBit -1));



		     // -- Receive sorted subarray from processors and merge them into "result" --

		     for (int i=0; i<processor_count; i++){

			     if(i > 0){				

				     int sub_array[sub_array_size];

				     // Receive each subarray

				     MPI_Recv(sub_array, sub_array_size, MPI_INT, i, 2, MPI_COMM_WORLD, &status);

				     // Create another temp_result array for compare
				     int temp_size = i * sub_array_size + extraBit;
				     int temp_result[temp_size];

				     for(int j=0; j < temp_size; j++){

					     temp_result[j] = result[j];

				     }

				     // Merge receive array into the result array

				     merge(sub_array, temp_result, result, sub_array_size, temp_size);

			     }else{

				     // For 0th processor, we have to write sorted sub_array as it is in result	

				     for(int j=0; j < sub_array_size + extraBit; j++){	

					     result[j] = sub_array[j];

				     }		

				     free(sub_array);

			     }

		     }

	     }else{

		     // if there is only one processor then we just do seq quicksort

		     quicksort(local_unsorted_array, 0, SIZE-1);

		     for(int i=0; i<SIZE; i++){

			     result[i] = local_unsorted_array[i];

		     }

	     }

	     double finish_time;

	     finish_time = MPI_Wtime();

	     std::cout<< std::endl << "Result: " << std::endl;

	     std::cout<< "Total processors = " << size << ", execution time = " << finish_time - start_time << " sec " << std::endl;

     }else{

	     // Other processors, receive subarray from 0th processor and sort it and send it back

	     sub_array_size = SIZE / processor_count;

	     int *sub_array = (int *)malloc(sub_array_size * sizeof(int));

	     MPI_Recv(sub_array, sub_array_size, MPI_INT, 0, 0, MPI_COMM_WORLD,&status);	

	     quicksort(sub_array, 0, sub_array_size-1);



	     MPI_Send(sub_array, sub_array_size, MPI_INT, 0, 2, MPI_COMM_WORLD); //sends sorted subarray to rank 0	

	     free(sub_array);

     }



     // Print sorted array by processor 0

     if(rank==0){
	
     	     std::cout << "After sorting " << std::endl;
     
     	     for(int i=0; i<SIZE; i++) {
     
     		     std::cout << result[i] << std::endl;
     
     	     }

     }

     free(local_unsorted_array);

     // End of Parallel Execution

     MPI_Finalize();

}


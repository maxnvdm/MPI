//==============================================================================
// Copyright (C) John-Philip Taylor
// tyljoh010@myuct.ac.za
//
// This file is part of the EEE4084F Course
//
// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>
//
// This is an adaptition of The "Hello World" example avaiable from
// https://en.wikipedia.org/wiki/Message_Passing_Interface#Example_program
//==============================================================================


/** \mainpage Prac4 Main Page
 *
 * \section intro_sec Introduction
 *
 * The purpose of Prac4 is to learn some basics of MPI coding.
 *
 * Look under the Files tab above to see documentation for particular files
 * in this project that have Doxygen comments.
 */



//---------- STUDENT NUMBERS --------------------------------------------------
//
// Please note:  put your student numbers here !!  <<<< NB!  NB!
//
//-----------------------------------------------------------------------------

/* Note that Doxygen comments are used in this file. */
/** \file Prac4
 *  Prac4 - MPI Main Module
 *  The purpose of this prac is to get a basic introduction to using
 *  the MPI libraries for prallel or cluster-based programming.
 */

// Includes needed for the program
#include "Prac4.h"

void swap(int *xp, int *yp) 
{ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 
/* This function takes last element as pivot, places 
   the pivot element at its correct position in sorted 
    array, and places all smaller (smaller than pivot) 
   to left of pivot and all greater elements to right 
   of pivot */
int partition (int arr[], int low, int high) 
{ 
    int pivot = arr[high];    // pivot 
    int i = (low - 1);  // Index of smaller element 
  
    for (int j = low; j <= high- 1; j++) 
    { 
        // If current element is smaller than or 
        // equal to pivot 
        if (arr[j] <= pivot) 
        { 
            i++;    // increment index of smaller element 
            swap(&arr[i], &arr[j]); 
        } 
    } 
    swap(&arr[i + 1], &arr[high]); 
    return (i + 1); 
} 
/* The main function that implements QuickSort 
 arr[] --> Array to be sorted, 
  low  --> Starting index, 
  high  --> Ending index */
void quickSort(int arr[], int low, int high) 
{ 
    if (low < high) 
    { 
        /* pi is partitioning index, arr[p] is now 
           at right place */
        int pi = partition(arr, low, high); 
  
        // Separately sort elements before 
        // partition and after partition 
        quickSort(arr, low, pi - 1); 
        quickSort(arr, pi + 1, high); 
    } 
}


int medianFilter(int startInt, int split){
//out[0].Allocate(Input.Width, Input.Height, Input.Components);
//Output2.Allocate(Input.Width, Input.Height/split, Input.Components);
//JPEG Output2;
//Output2.Allocate(Input.Width, Input.Height, Input.Components);
int x, y;
int j, k;
int start = startInt*(ceil(2560/split));
int end = start + ceil(2560/split);
int index;
for(y = start; y < end; y++){
    for(x = 0; x < Input.Width*Input.Components; x++){
        int pixelComponent[81];
        int k=0;
        
        for (int j = -4; j <= 4; j++) //4 down & up four from y
            {
                for (int i = (-4 * Input.Components); i <= (4 * Input.Components); i += 3, k++) //each 1 = 3
                {
                    if (((y + j) < 0) || ((y + j) >= (Input.Height)))
                    {
                        pixelComponent[k] = 0;
                    }
                    else if (((x + i) < 0) || ((x + i) >= (Input.Components * Input.Width)))
                    {
                        pixelComponent[k] = 0;
                    }
                    else
                    {
                        pixelComponent[k] = Input.Rows[y + j][x + i];
                    }
                }
            }
    //bubbleSort(pixelComponent, sizeof(pixelComponent)/sizeof(pixelComponent[0]));
    quickSort(pixelComponent,0,80);
	//Output.Rows[y][x] = pixelComponent[39];
    return pixelComponent[39];
    }
 }
}

/** This is the master node function, describing the operations
    that the master will be doing */
void Master () {
    //! <h3>Local vars</h3>
    // The above outputs a heading to doxygen function entry
    int  j;             //! j: Loop counter
    char buff[BUFSIZE]; //! buff: Buffer for transferring message data
    MPI_Status stat;    //! stat: Status of the MPI application

    // Start of "Hello World" example..............................................
    printf("0: We have %d processors\n", numprocs);
    for(j = 1; j < numprocs; j++) {
        sprintf(buff, "Hello %d! ", j);
        MPI_Send(buff, BUFSIZE, MPI_CHAR, j, TAG, MPI_COMM_WORLD);
    }
    for(j = 1; j < numprocs; j++) {
        // This is blocking: normally one would use MPI_Iprobe, with MPI_ANY_SOURCE,
        // to check for messages, and only when there is a message, receive it
        // with MPI_Recv.  This would let the master receive messages from any
        // slave, instead of a specific one only.
        MPI_Recv(buff, BUFSIZE, MPI_CHAR, j, TAG, MPI_COMM_WORLD, &stat);
        printf("0: %s\n", buff);
    }
    // End of "Hello World" example................................................

    // Read the input image
    if(!Input.Read("Data/greatwall.jpg")){
        printf("Cannot read image\n");
        return;
    }

    // Allocated RAM for the output image
    if(!Output.Allocate(Input.Width, Input.Height, Input.Components)) return;

    // This is example code of how to copy image files ----------------------------
    printf("Start of example code...\n");
    for(j = 0; j < 10; j++){
        tic();
        int x, y;
        for(y = 0; y < Input.Height; y++){
            for(x = 0; x < Input.Width*Input.Components; x++){
                Output.Rows[y][x] = Input.Rows[y][x];
            }
        }
        printf("Time = %lg ms\n", (double)toc()/1e-3);
    }
    printf("End of example code...\n\n");
    // End of example -------------------------------------------------------------

    // Write the output image
    if(!Output.Write("Data/Output.jpg")){
        printf("Cannot write image\n");
        return;
    }
    //! <h3>Output</h3> The file Output.jpg will be created on success to save
    //! the processed output.
}
//------------------------------------------------------------------------------

/** This is the Slave function, the workers of this MPI application. */
void Slave(int ID){
    // Start of "Hello World" example..............................................
    char idstr[32];
    char buff [BUFSIZE];

    MPI_Status stat;

    // receive from rank 0 (master):
    // This is a blocking receive, which is typical for slaves.
    MPI_Recv(buff, BUFSIZE, MPI_CHAR, 0, TAG, MPI_COMM_WORLD, &stat);
    sprintf(idstr, "Processor %d ", ID);
    strncat(buff, idstr, BUFSIZE-1);
    strncat(buff, "reporting for duty", BUFSIZE-1);

    // send to rank 0 (master):
    MPI_Send(buff, BUFSIZE, MPI_CHAR, 0, TAG, MPI_COMM_WORLD);
    // End of "Hello World" example................................................
}
//------------------------------------------------------------------------------

/** This is the entry point to the program. */
int main(int argc, char** argv){
    int myid;

    // MPI programs start with MPI_Init
    MPI_Init(&argc, &argv);

    // find out how big the world is
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    // and this processes' rank is
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    // At this point, all programs are running equivalently, the rank
    // distinguishes the roles of the programs, with
    // rank 0 often used as the "master".
    if(myid == 0) Master();
    else          Slave(myid);

    // MPI programs end with MPI_Finalize
    MPI_Finalize();
    return 0;
}
//------------------------------------------------------------------------------

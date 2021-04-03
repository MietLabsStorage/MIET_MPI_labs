#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "mpi.h"
#include <string>

static void showArr(int* arr, int N) {
	for (int i = 0; i < N; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

static void star(int M) {
	int ProcNum, ProcRank;
	MPI_Status Status;
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

	int* sendFrom0 = new int[ProcNum];
	if (ProcRank == 0) {
		for (int i = 0; i < ProcNum; i++) {
			sendFrom0[i] = 1;
		}
	}
	int* getFrom0 = new int[1]{ -1 };

	for (int i = 0; i < M; i++) {
		//send to any procs
		MPI_Scatter(sendFrom0, 1, MPI_INT, getFrom0, 1, MPI_INT, 0, MPI_COMM_WORLD);
		printf("%d. Get from 0 proc mes=\'%d\' to %d proc\n", i, *getFrom0, ProcRank);
		//multiply
		*getFrom0 = (*getFrom0) * ProcRank;
		//send to 0 proc for printing
		MPI_Gather(getFrom0, 1, MPI_INT, sendFrom0, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if (ProcRank == 0) {
			printf("%d. Num pows: ", i);
			showArr(sendFrom0, ProcNum);
		}
	}

}

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	int M = 4;
	star(M);
	
	MPI_Finalize();
	return 0;
}
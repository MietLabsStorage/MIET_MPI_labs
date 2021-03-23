#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "mpi.h"
#include <string>

static void star(int M) {
	int ProcNum, ProcRank;
	int* RecvRank = new int[1];
	int TotalRank;
	int MaxRank;
	int MinRank;
	MPI_Status Status;
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	if (ProcRank == 0) {
		*RecvRank = 0;
	}
	else {
		*RecvRank = -1;
	}
	for (int im = 0; im < M; im++) {
		MPI_Bcast(RecvRank, 1, MPI_INT, 0, MPI_COMM_WORLD);

		if (ProcRank == 0)
		{
			printf("\n%d. Send \'%d\' from %d proc to any procs", im, *RecvRank, ProcRank);
		}
		else {
			printf("\n%d. Receive \'%d\' to %d proc from 0 proc", im, *RecvRank, ProcRank);
			printf("\n%d. Send \'%d\' from %d proc to 0 proc", im, ProcRank, ProcRank);

		}

		MPI_Reduce(&ProcRank, &TotalRank, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		MPI_Reduce(&ProcRank, &MaxRank, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
		MPI_Reduce(&ProcRank, &MinRank, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
		

		MPI_Barrier(MPI_COMM_WORLD);
		if (ProcRank == 0)
		{
			printf("\n%d. Receive \'%d\' (total sum) to %d proc from any procs", im, TotalRank, ProcRank);
			printf("\n%d. Receive \'%d\' (max rank) to %d proc from any procs", im, MaxRank, ProcRank);
			printf("\n%d. Receive \'%d\' (min rank) to %d proc from any procs", im, MinRank, ProcRank);
		}
		
	}

}

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	int procRank;
	MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

	int M = 1;
	star(M);

	
	MPI_Finalize();
	return 0;
}
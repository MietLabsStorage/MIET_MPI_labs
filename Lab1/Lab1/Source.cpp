#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "mpi.h"
#include <string>

static void star(int M) {
	int ProcNum, ProcRank;
	int RecvRank;
	MPI_Status Status;
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

	for (int im = 0; im < M; im++) {
		if (ProcRank == 0)
		{
			//send messege to any other procs
			for (int i = 1; i < ProcNum; i++)
			{
				//mes += "from 0 proc";
				MPI_Send(&ProcRank, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
				printf("\n%d. Send \'%d\' from %d proc to any procs", im, ProcRank, ProcRank);
			}

			//receive answer from any other procs
			for (int i = 1; i < ProcNum; i++)
			{
				MPI_Recv(&RecvRank, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
				printf("\n%d. Receive \'%d\' to %d proc from any procs", im, RecvRank, ProcRank);
			}
		}
		else {
			//receive message from 0 proc
			MPI_Recv(&RecvRank, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
			printf("\n%d. Receive \'%d\' to %d proc from 0 proc", im, RecvRank, ProcRank);

			//send answer to 0 proc
			MPI_Send(&ProcRank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			printf("\n%d. Send \'%d\' from %d proc to 0 proc", im, ProcRank, ProcRank);
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
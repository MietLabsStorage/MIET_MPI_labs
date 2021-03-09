#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "mpi.h"
#include <string>

static void star(int M, char* mes) {
	int ProcNum, ProcRank;
	char* RecvRank = new char[255];
	MPI_Status Status;
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

	for (int im = 0; im < M; im++) {
		if (ProcRank == 0)
		{
			//send messege to any other procs
			for (int i = 1; i < ProcNum; i++)
			{
				MPI_Send(&mes, sizeof(mes), MPI_CHAR, i, 0, MPI_COMM_WORLD);
				printf("\n%d. Send %s from %d proc to any procs", im, mes, ProcRank);
			}

			//receive answer from any other procs
			for (int i = 1; i < ProcNum; i++)
			{
				MPI_Recv(&RecvRank, 255, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
				printf("\n%d. Receive %s to %d proc from any procs", im, RecvRank, ProcRank);
			}
		}
		else {
			//receive message from 0 proc
			MPI_Recv(&RecvRank, 255, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
			printf("\n%d. Receive %s to %d proc from 0 proc", im, RecvRank, ProcRank);

			//send answer to 0 proc
			MPI_Send(&RecvRank, sizeof(mes), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
			printf("\n%d. Send %s from %d proc to 0 proc", im, mes, ProcRank);
		}
	}

}

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	int procRank;
	MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

	int M = 2;
	char* mes = (char*)"\'this is a message\'";
		
	if (procRank == 0)
	{		
		star(M, mes);
	}
	else {
		star(M, (char*)"");
	}

	
	MPI_Finalize();
	return 0;
}
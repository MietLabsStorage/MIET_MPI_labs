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

int main(int argc, char* argv[])
{
	//#proc num N:: N = C / 2 + 1, where C - nums size (count)
	//such as^
	// Nums | procs
	// 2	| 2
	// 4	| 3
	// 8	| 5
	// 16	| 9
	MPI_Init(&argc, &argv);

	int ProcNum, ProcRank;
	MPI_Status Status;
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

	int* Nums;
	int NumsSize;
	NumsSize = 16;
	if (ProcRank == 0) {
		Nums = new int[NumsSize] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 1, 1, 1, 1, 1 };
	}
	else {
		Nums = new int[0];
	}

	int Step;	
	int MaxStep;
	int MaxNums;
	Step = 1;
	MaxNums = ProcNum - 1;
	MaxStep = log2(2 * MaxNums);

	int num1, num2;

	while (Step <= MaxStep) {
		if (ProcRank == 0) {
			for (int i = 0; i < MaxNums / Step; i++) {
				int sendNum = Nums[2*i];
				MPI_Send(&sendNum, 1, MPI_INT, i+1, 1, MPI_COMM_WORLD);
				sendNum = Nums[2 * i + 1];
				MPI_Send(&sendNum, 1, MPI_INT, i+1, 2, MPI_COMM_WORLD);
			}
		}
		if (ProcRank < MaxNums / Step + 1 && ProcRank != 0)
		{
			MPI_Recv(&num1, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &Status);
			MPI_Recv(&num2, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &Status);
			int multiply = num1 * num2;
			MPI_Send(&multiply, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
		}
		if (ProcRank == 0) {
			for (int i = 0; i < MaxNums / Step; i++) {
				int recvNum = 1;
				MPI_Recv(&recvNum, 1, MPI_INT, i+1, 3, MPI_COMM_WORLD, &Status);
				Nums[i] = recvNum;
			}
		}
		Step++;
	}

	if (ProcRank == 0) {
		printf("%d",Nums[0]);
	}

	MPI_Finalize();
	return 0;
}
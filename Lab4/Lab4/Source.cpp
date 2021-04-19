#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "mpi.h"
#include <string>

static void showArrReverse(uint32_t* arr, uint32_t N) {
	for (int i = 0; i < N; i++) {
		std::cout << " " << arr[N-i-1];
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

	MPI_Datatype LongInt;
	uint32_t LongIntSize = 10;
	MPI_Type_contiguous(LongIntSize, MPI_UINT32_T, &LongInt);
	MPI_Type_commit(&LongInt);

	int ProcNum, ProcRank;
	MPI_Status Status;
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

	uint32_t** Nums;
	uint32_t NumsSize;
	NumsSize = 16;
	if (ProcRank == 0) {
		//Nums = new uint32_t[NumsSize] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 1, 1, 1, 1, 1 };
		Nums = new uint32_t * [NumsSize]
		{
			new uint32_t[LongIntSize]{ 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				new uint32_t[LongIntSize]{ 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				new uint32_t[LongIntSize]{ 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				new uint32_t[LongIntSize]{ 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				new uint32_t[LongIntSize]{ 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				new uint32_t[LongIntSize]{ 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				new uint32_t[LongIntSize]{ 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				new uint32_t[LongIntSize]{ 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				new uint32_t[LongIntSize]{ 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				new uint32_t[LongIntSize]{ 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				new uint32_t[LongIntSize]{ 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				new uint32_t[LongIntSize]{ 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				new uint32_t[LongIntSize]{ 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				new uint32_t[LongIntSize]{ 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				new uint32_t[LongIntSize]{ 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				new uint32_t[LongIntSize]{ 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
		};
	}
	else {
		Nums = new uint32_t*[1]{new uint32_t[0]};
	}

	uint32_t Step;	
	uint32_t MaxStep;
	uint32_t MaxNums;
	Step = 1;
	MaxNums = ProcNum - 1;
	MaxStep = log2(2 * MaxNums);

	uint32_t* num1 = new uint32_t[LongIntSize];
	uint32_t* num2 = new uint32_t[LongIntSize];

	while (Step <= MaxStep) {

		/*if (ProcRank == 0) {
			showArr(Nums[0], LongIntSize);
		}*/

		if (ProcRank == 0) {
			for (int i = 0; i < MaxNums / Step; i++) {
				uint32_t* sendNum = Nums[2*i];
				MPI_Send(sendNum, 1, LongInt, i+1, 1, MPI_COMM_WORLD);
				sendNum = Nums[2 * i + 1];
				MPI_Send(sendNum, 1, LongInt, i+1, 2, MPI_COMM_WORLD);
			}
		}
		if (ProcRank < MaxNums / Step + 1 && ProcRank != 0)
		{
			MPI_Recv(num1, 1, LongInt, 0, 1, MPI_COMM_WORLD, &Status);
			MPI_Recv(num2, 1, LongInt, 0, 2, MPI_COMM_WORLD, &Status);
		//---multiplying---start---
			uint32_t* multiply = new uint32_t[LongIntSize] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			for (int i = 0; i < LongIntSize; i++) {
				for (int j = 0; j < LongIntSize; j++) {
					uint64_t mltpl = (uint64_t)num1[i] * (uint64_t)num2[j];
					uint32_t n1 = (uint32_t) mltpl;
					uint32_t n2 = (uint32_t)(mltpl >> 32);
					if (i + j < LongIntSize) {
						multiply[i + j] += n1;
					}
					if (i + j + 1 < LongIntSize) {
						multiply[i + j + 1] += n2;
					}
				}
			}
		//---multiplying---end---
			MPI_Send(multiply, 1, LongInt, 0, 3, MPI_COMM_WORLD);
		}
		if (ProcRank == 0) {
			for (int i = 0; i < MaxNums / Step; i++) {
				uint32_t* recvNum = new uint32_t[LongIntSize];
				MPI_Recv(recvNum, 1, LongInt, i+1, 3, MPI_COMM_WORLD, &Status);
				Nums[i] = recvNum;
			}
		}
		Step++;
	}

	if (ProcRank == 0) {
		showArrReverse(Nums[0], LongIntSize);
	}

	MPI_Finalize();
	return 0;
}
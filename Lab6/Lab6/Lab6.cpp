#include <iostream>
#include <vector>
#include <complex>
#include <mpi.h>
#include <windows.h>

using namespace std;

const double PI = acos(-1);

void FFT(vector<complex<double>>& a, bool invert) {
	int n = (int)a.size();
	if (n == 1)  return;

	vector<complex<double>> a0(n / 2), a1(n / 2);
	for (int i = 0, j = 0; i < n; i += 2, j++) {
		a0[j] = a[i];
		a1[j] = a[i + 1];
	}
	FFT(a0, invert);
	FFT(a1, invert);

	double ang = 2 * PI / n * (invert ? -1 : 1);
	complex<double> w(1);
	complex<double> wn(cos(ang), sin(ang));
	for (int i = 0; i < n / 2; i++) {
		a[i] = a0[i] + w * a1[i];
		a[i + n / 2] = a0[i] - w * a1[i];
		if (invert)
			a[i] /= 2, a[i + n / 2] /= 2;
		w *= wn;
	}
}

void multiply(vector<int> a, vector<int> b, vector<int>& c) {
	vector<complex<double>> fa = vector<complex<double>>(a.begin(), a.end());
	vector<complex<double>> fb = vector<complex<double>>(b.begin(), b.end());

	int n = 1;

	while (n < max(a.size(), b.size())) {
		n *= 2;
	}
	n *= 2;

	while (fa.size() != n) {
		fa.emplace(fa.begin(), complex<double>(0, 0));
	}
	while (fb.size() != n) {
		fb.emplace(fb.begin(), complex<double>(0, 0));
	}

	FFT(fa, false);
	FFT(fb, false);

	for (int i = 0; i < n; i++)
	{
		fa[i] *= fb[i];
	}

	FFT(fa, true);

	c.resize(n, 0);

	for (int i = 0; i < n - 1; i++)
	{
		c[i + 1] = int(fa[i].real() + 0.5);
	}

	for (int i = c.size() - 1; i > 0; i--) {
		if (c[i] >= 10) {
			c[i - 1] += c[i] / 10;
			c[i] = c[i] % 10;
		}
	}

}

int main(int* argc, char** argv) {

	/*vector<int> A = { 1, 2 };
	vector<int> B = { 1, 1 };
	vector<int> C(0);
	multiply(A, B, C);
	for (vector<int>::iterator i = C.begin(); i != C.end(); ++i) {
		printf("%d ", *i);
	}*/
	MPI_Init(argc, &argv);
	int ProcNum, ProcRank;
	MPI_Status Status;
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	MPI_Status status;

	int INDEX[] = { 2,5,8,9,10,11,12 };
	int EDGES[] = {1,2,3,4,0,5,6,0,1,1,2,2 };

	MPI_Comm GRAPH_COMM;
	MPI_Graph_create(MPI_COMM_WORLD, 7, INDEX, EDGES, 1, &GRAPH_COMM);
	int neighbours_count;
	MPI_Graph_neighbors_count(GRAPH_COMM, ProcRank, &neighbours_count);
	int* neighbours = new int[neighbours_count];
	MPI_Graph_neighbors(GRAPH_COMM, ProcRank, neighbours_count, neighbours);

	MPI_Datatype MPI_LONGINT;
	int long_num_size = 2;
	MPI_Type_contiguous(long_num_size * ProcNum, MPI_INT, &MPI_LONGINT);
	MPI_Type_commit(&MPI_LONGINT);

	if (neighbours_count == 1)
	{
		vector<int> A_vector = { 1, ProcRank };
		vector<int> B_vector = { ProcRank, 1 };
		vector<int> C_vector;

		multiply(A_vector, B_vector, C_vector);

		for (vector<int>::iterator i = C_vector.begin(); i != C_vector.end(); ++i) {
			printf("%d ", *i);
		}

		MPI_Send(C_vector.data(), 1, MPI_LONGINT, neighbours[0], ProcRank, GRAPH_COMM);
	}

	if (neighbours_count == 3)
	{
		long_num_size *= 2;
		int * A = new int[long_num_size];
		int * B = new int[long_num_size];

		if (ProcRank == 1) {
			MPI_Recv(A, 1, MPI_LONGINT, neighbours[0], 3, GRAPH_COMM, &status);
			MPI_Recv(B, 1, MPI_LONGINT, neighbours[1], 4, GRAPH_COMM, &status);
		}
		if (ProcRank == 2) {
			MPI_Recv(A, 1, MPI_LONGINT, neighbours[0], 5, GRAPH_COMM, &status);
			MPI_Recv(B, 1, MPI_LONGINT, neighbours[1], 6, GRAPH_COMM, &status);
		}

		vector<int> A_vector(A, A + long_num_size);
		vector<int> B_vector(B, B + long_num_size);
		vector<int> C_vector(long_num_size*2,0);

		multiply(A_vector, B_vector, C_vector);

		for (vector<int>::iterator i = C_vector.begin(); i != C_vector.end(); ++i) {
			printf("%d ", *i);
		}

		MPI_Send(C_vector.data(), 1, MPI_LONGINT, neighbours[2], ProcRank, GRAPH_COMM);
		MPI_Send(C_vector.data(), 1, MPI_LONGINT, neighbours[2], ProcRank, GRAPH_COMM);
	}

	if (neighbours_count == 2)
	{
		long_num_size *= 4;
		int* A = new int[long_num_size];
		int* B = new int[long_num_size];

		MPI_Recv(A, 1, MPI_LONGINT, neighbours[0], 1, GRAPH_COMM, &status);
		MPI_Recv(B, 1, MPI_LONGINT, neighbours[1], 2, GRAPH_COMM, &status);

		vector<int> A_vector(A, A + long_num_size);
		vector<int> B_vector(B, B + long_num_size);
		vector<int> C_vector(long_num_size * 2, 0);

		multiply(A_vector, B_vector, C_vector);

		for (vector<int>::iterator i = C_vector.begin(); i != C_vector.end(); ++i) {
			printf("%d ", *i);
		}
	}

	MPI_Finalize();

}

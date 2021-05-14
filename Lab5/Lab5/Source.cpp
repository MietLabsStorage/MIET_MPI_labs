#include <iostream>

const int CIRCLE = 7;
const int SIZE = 8;
const int EXIT_SIZE = 4;

struct ComplexMatrix
{
	int** real;
	int** imaginary;
	int size;
};

ComplexMatrix Sum(ComplexMatrix cm1, ComplexMatrix cm2)
{
	struct ComplexMatrix ans;
	ans.size = cm1.size;
	ans.real = new int* [ans.size];
	ans.imaginary = new int* [ans.size];
	for (int i = 0; i < ans.size; i++) {
		ans.real[i] = new int[ans.size];
		ans.imaginary[i] = new int[ans.size];
	}

	for (int i = 0; i < ans.size; i++)
	{
		for (int j = 0; j < ans.size; j++)
		{
			for (int r = 0; r < ans.size; r++)
			{
				ans.real[i][j] = cm1.real[i][r] + cm2.real[r][j];
				ans.imaginary[i][j] = cm1.imaginary[i][r] + cm2.imaginary[r][j];
			}
		}
	}

	return ans;
};

ComplexMatrix Substract(ComplexMatrix cm1, ComplexMatrix cm2)
{
	struct ComplexMatrix ans;
	ans.size = cm1.size;
	ans.real = new int* [ans.size];
	ans.imaginary = new int* [ans.size];
	for (int i = 0; i < ans.size; i++) {
		ans.real[i] = new int[ans.size];
		ans.imaginary[i] = new int[ans.size];
	}

	for (int i = 0; i < ans.size; i++)
	{
		for (int j = 0; j < ans.size; j++)
		{
			for (int r = 0; r < ans.size; r++)
			{
				ans.real[i][j] = cm1.real[i][r] - cm2.real[r][j];
				ans.imaginary[i][j] = cm1.imaginary[i][r] - cm2.imaginary[r][j];
			}
		}
	}

	return ans;
};

ComplexMatrix Multiply(ComplexMatrix cm1, ComplexMatrix cm2)
{
	struct ComplexMatrix ans;
	ans.size = cm1.size;
	ans.real = new int*[ans.size];
	ans.imaginary = new int*[ans.size];
	for (int i = 0; i < ans.size; i++) {
		ans.real[i] = new int[ans.size];
		ans.imaginary[i] = new int[ans.size];
	}


	if (ans.size <= 4) 
	{
		for (int i = 0; i < ans.size; i++)
		{
			for (int j = 0; j < ans.size; j++) 
			{
				for (int r = 0; r < ans.size; r++)
				{
					ans.real[i][j] = cm1.real[i][r] * cm2.real[r][j];
					ans.real[i][j] += -1 * cm1.imaginary[i][r] * cm2.imaginary[r][j];
					ans.imaginary[i][j] = cm1.real[i][r] * cm2.imaginary[r][j];
					ans.imaginary[i][j] += cm1.imaginary[i][r] * cm2.real[r][j];
				}
			}
		}
	}
	else 
	{
		ComplexMatrix** a = new ComplexMatrix* [2];
		ComplexMatrix** b = new ComplexMatrix * [2];
		ComplexMatrix** c = new ComplexMatrix * [2];
		for (int i = 0; i < 2; i++) {
			a[i] = new ComplexMatrix[2];
			b[i] = new ComplexMatrix[2];
			c[i] = new ComplexMatrix[2];
			for (int j = 0; j < 2; j++) {
				a[i][j].size = cm1.size / 2;
				a[i][j].real = new int* [a[i][j].size];
				a[i][j].imaginary = new int* [a[i][j].size];
				b[i][j].size = cm1.size / 2;
				b[i][j].real = new int* [b[i][j].size];
				b[i][j].imaginary = new int* [b[i][j].size];
				c[i][j].size = cm1.size / 2;
				c[i][j].real = new int* [c[i][j].size];
				c[i][j].imaginary = new int* [c[i][j].size];
				for (int k = 0; k < a[i][j].size; k++) {
					a[i][j].real[k] = new int[a[i][j].size];
					a[i][j].imaginary[k] = new int[a[i][j].size];
					b[i][j].real[k] = new int[b[i][j].size];
					b[i][j].imaginary[k] = new int[b[i][j].size];
					c[i][j].real[k] = new int[c[i][j].size];
					c[i][j].imaginary[k] = new int[c[i][j].size];
					for (int l = 0; l < a[i][j].size; l++) {
						a[i][j].real[k][l] = cm1.real[i * cm1.size / 2 + k][j * cm1.size / 2 + l];
						b[i][j].real[k][l] = cm2.real[i * cm2.size / 2 + k][j * cm2.size / 2 + l];
					}
				}
			}
		}

		ComplexMatrix p1 = Multiply(Sum(a[0][0], a[1][1]), Sum(b[0][0], a[0][0]));
		ComplexMatrix p2 = Multiply(Sum(a[1][0], a[1][1]), b[0][0]);
		ComplexMatrix p3 = Multiply(a[0][0], Substract(b[0][1], b[1][1]));
		ComplexMatrix p4 = Multiply(a[1][1], Substract(b[1][0], b[0][0]));
		ComplexMatrix p5 = Multiply(Sum(a[0][0], a[0][1]), b[1][1]);
		ComplexMatrix p6 = Multiply(Substract(a[1][0], a[0][0]), Sum(b[0][0], b[0][1]));
		ComplexMatrix p7 = Multiply(Substract(a[0][1], a[1][1]), Sum(b[1][0], b[1][1]));

		c[0][0] = Sum(Substract(Sum(p1, p4), p5), p7);
		c[0][1] = Sum(p3, p5);
		c[1][0] = Sum(p2, p4);
		c[1][1] = Sum(Sum(Substract(p1, p2), p3), p6);

		for (int i = 0; i < c[0][0].size; i++) {
			for (int j = 0; j < c[0][0].size; j++) {
				ans.real[i][j] = c[0][0].real[i][j];
				ans.imaginary[i][j] = c[0][0].imaginary[i][j];
				ans.real[i + ans.size / 2][j] = c[1][0].real[i][j];
				ans.imaginary[i + ans.size / 2][j] = c[1][0].imaginary[i][j];
				ans.real[i][j + ans.size / 2] = c[0][1].real[i][j];
				ans.imaginary[i][j + ans.size / 2] = c[0][1].imaginary[i][j];
				ans.real[i + ans.size / 2][j + ans.size / 2] = c[1][1].real[i][j];
				ans.imaginary[i + ans.size / 2][j + ans.size / 2] = c[1][1].imaginary[i][j];
			}
		}

		return ans;
	}
	
};

void printMatrix(ComplexMatrix cm) {
	for (int i = 0; i < cm.size; i++)
	{
		for (int j = 0; j < cm.size; j++)
		{
			printf("%d+%di ", cm.real[i][j], cm.imaginary[i][j]);
		}
		printf("\n");
	}
}

int main() 
{
	ComplexMatrix complexMatrix;
	complexMatrix.size = SIZE;
	complexMatrix.real = new int*[complexMatrix.size];
	complexMatrix.imaginary = new int*[complexMatrix.size];
	for (int i = 0; i < complexMatrix.size; i++) {
		complexMatrix.real[i] = new int[complexMatrix.size];
		complexMatrix.imaginary[i] = new int[complexMatrix.size];
	}

	for (int i = 0; i < complexMatrix.size; i++)
	{
		for (int j = 0; j < complexMatrix.size; j++)
		{
			complexMatrix.real[i][j] = i % 7 + j % 5;
			complexMatrix.imaginary[i][j] = i % 5 + j % 7;
		}
	}

	printf("data:\n");
	printMatrix(complexMatrix);

	printf("result:\n");
	printMatrix(Multiply(complexMatrix,complexMatrix));


}
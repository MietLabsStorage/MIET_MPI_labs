#include <iostream>
#include <vector>
#include <complex>
#include <cassert>

using namespace std;

const double PI = acos(-1);

void fft(vector<complex<double>>& a, bool invert) {
	int n = (int)a.size();

	for (int i = 1, j = 0; i < n; i++) {
		int bit = n / 2;
		for (; j >= bit; bit /= 2)
			j -= bit;
		j += bit;
		if (i < j)
			swap(a[i], a[j]);
	}

	for (int len = 2; len <= n; len *= 2) {
		double ang = 2 * PI / len * (invert ? -1 : 1);
		complex<double> wlen(cos(ang), sin(ang));
		for (int i = 0; i < n; i += len) {
			complex<double> w(1);
			for (int j = 0; j < len / 2; ++j) {
				complex<double> u = a[i + j], v = a[i + j + len / 2] * w;
				a[i + j] = u + v;
				a[i + j + len / 2] = u - v;
				w *= wlen;
			}
		}
	}
	if (invert)
		for (int i = 0; i < n; ++i)
			a[i] /= n;
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

	fft(fa, false);
	fft(fb, false);

	for (int i = 0; i < n; i++)
	{
		fa[i] *= fb[i];
	}

	fft(fa, true);

	c.resize(n,0);

	for (int i = 0; i < n-1; i++)
	{
		c[i+1] = int(fa[i].real() + 0.5);
	}

	for (int i = c.size() - 1; i > 0; i--) {
		if (c[i] >= 10) {
			c[i - 1] += c[i] / 10;
			c[i] = c[i] % 10;
		}
	}

}

int main() {

	vector<int> A = { 1, 2 };
	vector<int> B = { 1, 1 };
	vector<int> C(0);
	multiply(A, B, C);
	for (vector<int>::iterator i = C.begin(); i != C.end(); ++i) {
		printf("%d ", *i);
	}


}

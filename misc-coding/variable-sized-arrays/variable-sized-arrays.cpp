// variable-sized-arrays.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <vector>
using namespace std;

int main()
{
	unsigned int N, Q;

	cin >> N >> Q;

	// Verify the input
	if (1 > N || N > 100000) {
		cerr << "Invlaid value for N: " << N << endl;
		return 1;
	}
	if (1 > Q || Q > 100000) {
		cerr << "Invlaid value for Q: " << Q << endl;
		return 1;
	}

	unsigned int **seqs = new unsigned int *[N];

	// Get the data
	// Loop N numbers of loops
	for (unsigned int i = 0; i < N; i++) {
		unsigned int k;
		unsigned int s;
		cin >> k;
		if (1 > k || k > 300000) {
			cerr << "Invlaid value for k: " << k << ". in loop " << i << endl;
			continue;
		}
		seqs[i] = new unsigned int [k+1];
		seqs[i][0] = k;
		for (unsigned int j = 0; j < k; j++) {
			cin >> s;
			seqs[i][j+1] = s;
		}
	}

	// Get the queries
	for (unsigned int i = 0; i < Q; i++) {
		unsigned int a;
		unsigned int b;
		cin >> a >> b;
		if (a > N) {
			cerr << "Invalid querry: " << a << endl;
			continue;
		}
		if (b > seqs[a][0] - 1) {
			cerr << "Invalid element " << b << "for querry: " << a << endl;
			continue;
		}
		cout << seqs[a][b+1] << endl;
	}

	// Cleanup
	for (unsigned int i = 0; i < N; i++) {
		delete[] seqs[i];
	}
	delete[] seqs;

	return 0;
}

#if 0

int main()
{
	unsigned int N, Q;

	cin >> N >> Q;

	// Verify the input
	if (1 > N || N > 100000) {
		cerr << "Invlaid value for N: " << N << endl;
		return 1;
	}
	if (1 > Q || Q > 100000) {
		cerr << "Invlaid value for Q: " << Q << endl;
		return 1;
	}

	vector<vector< unsigned int >> seqs(N);

	// Get the data
	// Loop N numbers of loops
	for (unsigned int i = 0; i < N; i++) {
		unsigned int k;
		unsigned int s;
		cin >> k;
		if (1 > k || k > 300000) {
			cerr << "Invlaid value for k: " << k << ". in loop " << i << endl;
			continue;
		}
		seqs[i].resize(k);
		for (unsigned int j = 0; j < k; j++) {
			cin >> s;
			seqs[i][j] = s;
		}
	}

	// Get the queries
	for (unsigned int i = 0; i < Q; i++) {
		unsigned int a;
		unsigned int b;
		cin >> a >> b;
		if (a > N) {
			cerr << "Invalid querry: " << a << endl;
			continue;
		}
		if (b > seqs[a].size() - 1) {
			cerr << "Invalid element " << b << "for querry: " << a << endl;
			continue;
		}
		cout << seqs[a][b] << endl;
	}

	return 0;
}

#endif
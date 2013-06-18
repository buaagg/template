void preMP(const char x[], int m, int next[]) {
	int i, j;
	i = next[0] = -1; j = 0;
	while (j < m) {
		while (i > -1 && x[i] != x[j]) i = next[i];
		next[++j] = ++i;
	}
}
int kmp(const char x[], int m, const char y[], int n) {
	int i = 0, j = 0, ret;
	preMP(x, m, next);
	while(j < n) {
		while (i > -1 && x[i] != y[j]) i = next[i];
		++i; ++j;
		if (i >= m) {
			//OUTPUT(j - i)
			ret++; i = next[i];
		}
	}	
	return ret;
}
void prez(const char x[], int m, int next[]) {
	int j, k = 1, r = 0; next[0] = m;
	for (int i = 1; i < m; ++i) {
		if ( i + next[i-k] < r ) {
			next[i] = next[i-k];
		} else {
			for (j = max(r-i, 0); i + j < m && x[i+j] == x[j]; ++j);
			next[i] = j; k = i; r = i + j;
		}
	}
}

//next[i]: lcp of x[i..m-1] and x[0..m-1]
//ext[i]:  lcp of y[i..n-1] and x[0..m-1]
void z(const char x[], int m, const char y[], int n, int next[], int ext[]) {
	int k = 0, r = 0, j;
	prez(x, m, next); next[0] = 0;
	for (int i = 0; i < n; ++i) {
		if ( i + next[i-k] < r ) {
			ext[i] = next[i-k];
		} else {
			for (j = max(r-i, 0); j < m && i + j < n && x[j] == y[i+j]; ++j);
			ext[i] = j; k = i; r = i + j;
		}
	}
}

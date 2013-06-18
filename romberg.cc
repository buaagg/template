real f(real x) {
	return exp(-x * x);
}
//O(2 ^ maxitr) function evaluations
real Romberg(real a, real b, real(*f)(real), real eps, int maxitr = 20) {
	real T[maxitr][maxitr];
	for (int i = 0; i < maxitr; ++i) {
		real h = (b - a) / (1 << i), x = a + h, pow = 4;
		T[i][0] = (f(a) + f(b)) / 2;
		for (int j = (1 << i) - 1; j >= 1; x += h, --j) T[i][0] += f(x); T[i][0] *= h;
		for (int j = 1; j <= i; pow *= 4, ++j)
			T[i][j] = T[i][j - 1] + (T[i][j - 1] - T[i - 1][j - 1]) / (pow - 1); if (i > 0 && fabs(T[i][i] - T[i - 1][i - 1]) <= eps) return T[i][i];
	}
	return T[maxitr - 1][maxitr - 1]; 
}

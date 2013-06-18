bool witness(LL a, LL n) {
	int k = 0;
	LL m = n - 1;
	do {m /= 2; k++; } while (m % 2 == 0);
	LL x = pow_mod(a, m, n);
	if (x == 1) return true;
	for (int i = 0; i < k; x = mul_mod(x, x, n); ++i)
		if (x == n - 1) return true;
	return false;
}

bool miller_rabin(LL n, int time = 50) {
	if (2 == n || 3 == n || 5 == n || 7 == n) return true;
	if (1 == n || n % 2 == 0 || n % 3 == n || n % 5 == 0 || n % 7 == 0) return false;
	while (time--) {
		LL r = rand() % (n-2) + 2;
		if ( gcd(r, n) != 1 || !witness(r % n, n) ) return false;
	}
	return true;
}

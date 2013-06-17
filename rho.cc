LL rho(LL n) {
	LL x, y, d, c;
	for (int k, i;;) {
		c = rand() % (n - 1) + 1;
		x = y = rand() % n;
		k = 2; i = 1;
		do {
			d = gcd( ABS(x - y), n );
			if ( d > 1 && d < n ) return d;
			if ( ++i == k ) y = x, k *= 2;
			x = mul_mod(x, x, n); x = (x + c) % n;
		} while ( x != y );
	}
}

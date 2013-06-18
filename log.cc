struct Zn {
	const LL n;
	Zn(LL nn) : n(nn) {
	}
	LL eval(LL a) {
		a %= n; return a >= 0 ? a : a + n;
	}
	LL mul(LL a, LL b) {
		if (n <= 1000000000) return a * b % n;
		assert(0);
	}
	LL pow(LL a, LL b) {
		LL r = 1 % n;
		for (;b;) {
			if (b & 1) r = mul(r, a);
			if (b >>= 1) a = mul(a, a);
		}
		return r;
	}
	LL inv(LL a) {
		LL x, y, d = exgcd(a, n, x, y);
		assert(d == 1);
		return eval(x);
	}
	LL log(LL a, LL b);
};

struct Zp : Zn {
	Zp(LL n) : Zn(n) {
	}
	const static int maxsqrtn = 100000 + 5;
	static int id[]; static LL mexp[];

	struct logcmp {
		bool operator()(int a, int b) { return mexp[a] < mexp[b]; }
	};
	LL log(LL a, LL b) { //a ^ x = b
		int m = (int)( ceil( sqrt(n) ) );
		LL v=  inv( pow(a, m) );
		id[0] = 0; mexp[0] = 1;
		for (int i = 1; i <= m; ++i) {
			id[i] = i; mexp[i] = mul(mexp[i-1], a);
		}
		stable_sort(id + 1, id + m + 1, logcmp());
		sort(mexp + 1, mexp + m + 1);
		for (int i = 0; i < m; ++i) {
			int j = lower_bound(mexp, mexp + m + 1, b) - mexp;
			if (j <= m && mexp[j] == b) return i * m + id[j];
			b = mul(b, v);
		}		
		return -1;
	}
}

LL Zn::log(LL a, LL b) { //a ^ x = b
	for (int i = 0; i <= 50; ++i) if ( pow(a, i) == b ) return i;
	LL g, d = 1, n = this->n, x = 0;
	while ( (g = gcd(a, n)) > 1 ) {
		if ( b % g ) return -1;
		b /= g; n /= g; d = mul(d, a/g); ++x;
	}
	Zp zp(n); LL ans = zp.log( a, zp.mul(b, zp.inv(d)) );
	return ans == -1 ? -1 : ans + x;
}

int Zp::id[Zp::maxsqrtn]; LL Zp::maxp[ Zp::maxsqrtn ];

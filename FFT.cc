struct Zp {
	const LL mod; const int pri;
	Zp(LL mod, int pri) : mod(mod), pri(pri) {
	}
	Zp(LL mod) : mod(mod), pri( primitive() ) {
	}
	void fft(int n, LL root, LL a[]) {
		for (int m = n; m >= 2; m >>= 1) {
			int mh = m >> 1; LL w = 1;
			for (int i = 0; i < mh; ++i) {
				for (int j = i; j < n; j += m) {
					int k = j + mh;
					LL t = sub(a[j], a[k]);
					a[j] = add(a[j], a[k]);
					a[k] = mul(w, t);
				}
				w = mul(w, root);
			}
			root = mul(root, root);
		}
		for (int j = 1, i = 0; j < n - 1; ++j) {
			for (int k = n >> 1; k > (i ^= k); k >>= 1);
			if (j < i) swap(a[i], a[j]);
		}
	}
	void dft(const LL a[], int an, LL b[], int n) {
		LL root = pow(pri, mod / n);
		copy(a, a + an, b); fill(b + an, b + n, 0);
		fft(n, root, b);
	}
	void nft(const LL a[], LL b[], int n) {
		LL root = pow(pri, mod / n); root = inv(root);
		copy(a, a + n, b);
		fft(n, root, b);
		LL invn = inv(n);
		rep(i, n) b[i] = mul(b[i], invn);
	}
	int primitive() {
		int n = mod - 1;
		LL p[25], pcnt = 0;
		for (LL i = 2; i * i <= n; ++i) {
			if ( n % i == 0 ) {
				do n /= i; while (n % i == 0);
				p[pcnt++] = i;
			}
		}
		if (n > 1) p[pcnt++] = n;
		for (int g = 2;;++g) {
			int ok = 1; //assert( pow(g, mod-1) == 1 );
			rep(i, pcnt) if ( pow(g, (mod-1)/p[i]) == 1 ) {
				ok = 0; break;
			}
			if (ok) return g;
		}
	}
} zp(0xb1a2bc2edc0001LL, 3);

struct poly {
	const static int maxn = ::maxn * 4 + 5;
	LL a[maxn]; int n;
	template<class T> void init(const T a[], int n) {
		this->n = n; copy(a, a + n, this->a);
	}
	LL eval(LL x) const {
		LL ans = 0;
		for (int i = n - 1; i >= 0; --i)
			ans = zp.add( zp.mul(ans, x), a[i] );
		return ans;
	}
	friend void mul(poly& r, const poly& x, const poly& y) {
		static LL xb[maxn], yb[maxn];
		int n = 1; while ( n < x.n + y.n ) n *= 2;
		LL root = zp.pow( zp.pri, zp.mod / n );
		zp.dft( x.a, x.n, xb, n ); 
		rep(i, n) assert( x.eval(zp.pow(root, i)) == xb[i] );
		zp.dft( y.a, y.n, yb, n );
		rep(i, n) assert( y.eval(zp.pow(root, i)) == yb[i] );
		rep(i, n) xb[i] = zp.mul(xb[i], yb[i]);
		zp.nft(xb, r.a, n);
		r.n = n; while (r.n > 0 && r.a[r.n-1] == 0) --r.n;
	}
};

struct mp { //BigUnsignedInteger
	static const int digit = 4;
	static const int base = 10000;
	static const int cap = 50000 * 2 + 5; // 10 ^ 500
	static const int maxn = cap / digit + 1;
	int dat[maxn], n;

	mp(const mp& o) : n(o.n) {
		copy(o.dat, o.dat + n, dat);
	}
	mp(LL v = 0) {
		for (n = 0; v; v /= base) dat[n++] = v % base;
	}
	void parse(const char *s) {
		n = 0;
		for (int i = strlen(s) - 1, v = 0, m = 1; i >= 0; --i) {
			v = v + (s[i] - '0') * m; m *= 10;
			if (m == base || i == 0) {
				dat[n++] = v; v = 0; m = 1;
			}
		}
	}
	char *toString(char *s) const {
		if (n == 0) {
			sprintf(s, "0");
		} else {
			char *p = s;
			p += sprintf(p, "%d", dat[n-1]);
			for (int i = n - 2; i >= 0; --i) 
				p += sprintf(p, "%0*d", digit, dat[i]);
		}
		return s;
	}
	friend void mulfft(mp& r, const mp& x, const mp& y) {
		static poly px, py, pr;
		px.init(x.dat, x.n);
		py.init(y.dat, y.n);
		mul(pr, px, py);
		int i = 0;
		for (LL t = 0; i < pr.n || t; ++i, t /= base) {
			if (i < pr.n) t += pr.a[i];
			r.dat[i] = t % base;
		}
		r.n = i;
	}
};

int main() {
	static mp x, y, z;
	static char buf[1000000];
	while ( gets(buf) ) {
		x.parse(buf);
		gets(buf); y.parse(buf);
		mulfft(z, x, y);
		puts( z.toString() );
	}
}

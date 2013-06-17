#include <algorithm>
#include <cassert>
using namespace std;
typedef long long LL;

#define rep(i, n) for (int i = 0; i < (n); ++i)
const int maxn = 10000;

struct Zp {
	const LL mod; const int pri;
	Zp(LL mod, int pri) : mod(mod), pri(pri) {
	}
	Zp(LL mod) : mod(mod), pri( primitive() ) {
	}
	LL add(LL a, LL b) {
		a += b; return a >= mod ? a - mod : a;
	}
	LL sub(LL a, LL b) {
		a -= b; return a < 0 ? a + mod : a;
	}
	LL mul(LL a, LL b) {
		if ( mod <= 1000000000 ) return a * b % mod;
		LL t = (LL)( (double)a * b / mod + 0.5 );
		LL r = ( a * b - t * mod ) % mod;
		return r >= 0 ? r : r + mod;
	}
	LL pow(LL a, LL b) {
		LL r = 1;
		for (;b;) {
			if ( b & 1 ) r = mul(r, a);
			if ( b >>= 1 ) a = mul(a, a);
		}
		return r;
	}
	LL inv(LL a) {
		return pow(a, mod - 2);
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
	char *toString() const {
		static char buf[cap + 5]; return toString(buf);
	}
	friend void add(mp& r, const mp& x, const mp& y) {
		int i = 0;
		for (int t = 0; i < x.n || i < y.n || t; ++i, t /= base) {
			if (i < x.n) t += x.dat[i];
			if (i < y.n) t += y.dat[i];
			r.dat[i] = t % base;
		}
		r.n = i;
	}
	friend void sub(mp& r, const mp& x, const mp& y) {
		r.n = x.n;
		for (int i = 0, t = 0; i < r.n; ++i) {
			r.dat[i] = x.dat[i] - t;
			if ( i < y.n ) r.dat[i] -= y.dat[i];
			if ( r.dat[i] < 0 ) {
				t = 1; r.dat[i] += base;
			} else {
				t = 0;
			}
		}
		while (r.n && r.dat[r.n - 1] == 0) --r.n;
	}
	friend void mul(mp& r, const mp& x, int y) {
		int i = 0;
		for (LL t = 0; i < x.n || t; ++i, t /= base) {
			if (i < x.n) t += (LL)(x.dat[i]) * y;
			r.dat[i] = t % base;
		}
		r.n = i;
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
	friend void div(mp& q, int &r, const mp& x, int y) {
		q.n = x.n; r = 0;
		for (int i = x.n - 1; i >= 0; --i, r %= y) {
			r = r * base + x.dat[i];
			q.dat[i] = r / y;
		}
		while (q.n && q.dat[q.n-1] == 0) --q.n;

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

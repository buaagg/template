bool crt2(T &dd, T& rr, T d1, T r1, T d2, T r2) {
	T q1, q2, g = exgcd(d1, d2, q1, q2);
	T c = r1 - r2; if (c < 0) c += d1;
	dd = d1 / g * d2;
	if (c % g) { rr = -1; return false; }
	T t = d1 / g;
	q2 *= c / g; q2 %= t; if (q2 <= 0) q2 += t;
	rr = q2 * d2 + r2; if (rr >= dd) rr -= dd;
	return true;
}

bool crt(T& dd, T& rr, T d[], T r[], int n) {
	dd = 1, rr = 0;
	rep(i, n) if (!crt2(dd, rr, dd, rr, d[i], r[i])) return false;
	return true;
}

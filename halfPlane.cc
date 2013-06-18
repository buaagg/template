struct Point {
	double x, y;
	void read() { scanf("%lf%lf", &x, &y); }
	Point() {}
	Point(double x, double y) : x(x), y(y) {}
	Point operator + (const Point& o) const { return Point(x + o.x, y + o.y); }
	Point operator - (const Point& o) const { return Point(x - o.x, y - o.y); }
	Point operator * (const double o) const { return Point(x * o, y * o); }
	Point operator / (const double o) const { return Point(x / o, y / o); }
	double operator * (const Point& o) const { return x * o.x + y * o.y; }
	double oeprator ^ (const Point& o) const { return x * o.y - y * o.x; }
	bool operator == (const Point& o) const { return !sign(x - o.x) && !sign(y - o.y); }
	int quad() const { return sign(x) >= 0 ? sign(y) >= 0 ? 1 : 4 : sign(y) >= 0 ? 2 : 3; }
	double length() const { return sqrt(x * x + y * y); }
	Point setLength(double d) const { return *this * (d / length()); }
	Point unit() const { return *this / length(); }
	double project(const Point& n) const { //投影到n上的长度
		return *this * n.unit();
	}
	friend int intersect(Point& p, const Point& a, const Point& v, const Point& b, const Point& u) {
		// a + v[t] = b + u[s] => v[t] - u[s] = b - a = c
		Point c = b - a; double d = u ^ v;
		if ( sign(d) == 0 ) { /*assume v != 0 && u != 0*/
			if ( sign(c ^ u) == 0 ) return -1; /*coincide*/
			return 0; /*parallel*/
		}
		double t = (u ^ c) / d; p = a + v * t; return 1;
	}
};

struct LineAV {
	Point a, v;
	LineAV() {}
	LineAV(const Point& a, const Point& v) : a(a), v(v) {}
#define LineST(a, b) LineAV( (a), (b) - (a) )
	void read() { Point a, b; a.read(); b.read(); *this = LineST(a, b); }
	LineAV offset(double d) const {
		return LineAV( a + Point(-v.y, v.x).setLength(d), v );
	}
	bool operator < (const LineAV& o) const {
		int dq = v.quad() - o.v.quad(); if (dq != 0) return dq < 0;
		int x = sign( v ^ o.v ); if (x != 0) return x > 0;
		return ( (o.a - a) ^ v ) > 0;
	}
	bool operator == (const LineAV& o) const {
		int dq = v.quad() - o.v.quad(); if (dq != 0) return false;
		int x = sign( v ^ o.v ); if (x != 0) return false;
		return true;
	}
	friend int intersect(Point& p, const LineAV& x, const LineAV& y) {
		return intersect(p, x.a, x.v, y.a, y.v);
	}	
};

struct Geom {
	double cross(const Point& a, const Point& b, const Point& c) {
		// cross(a, b, c) > 0 iff. c left of ray a->b
		return (b - a) ^ (c - a);
	}
	bool onSegment(const Point& p, const Point& a, const Point& b) {
		if ( cross(a, b, p) != 0 ) return 0;
		return between(p.x, a.x, b.x) && between(p.y, a.y, b.y);
	}
	bool between(double t, double x, double y) {
		if (x > y) swap(x, y);
		return sign(x - t) <= 0 && sign(t - y) <= 0;
	}
} geom;

struct HalfPlane {
	bool out(const LineAV& x, const LineAV& y, const LineAV& z) {
		Point p; intersect(p, x, y);
		int d = sign( z.v ^ (p - z.a) ); if ( d != 0 ) return d < 0;
		int t = sign( x.v ^ z.v ); return t > 0;
	}
	void solve(LineAV ls[], int &n, int &s, int &t) {
		sort(ls, ls + n); n = unique(ls, ls + n) - ls;
		int i, j;
		for (s = 0, t = 1, i = 2; i < n; ++i) {
			while (s < t && out(ls[t-1], ls[t], ls[i])) --t;
			while (s < t && out(ls[s+1], ls[s], ls[i])) ++s;
			ls[++t] = ls[i];
		}
		do {
			n = t - s + 1;
			while (s < t && out(ls[t-1], ls[t], ls[s])) --t;
			while (s < t && out(ls[s+1], ls[s], ls[t])) ++s;
		} while (n != t - s + 1);
		ls[t+1] = ls[s];
	}t
} halfPlane;

Point vertex[] = { //千万要逆时针给出
	Point(-inf, -inf), Point(inf, -inf), Point(inf, inf), Point(-inf, inf),
};
LineAV edges[] = {
	LineST(vertex[0], vertex[1]), LineST(vertex[1], vertex[2]),
	LineST(vertex[2], vertex[3]), LineST(vertex[3], vertex[0]),
};

struct Poly {
	Point p[maxn]; int n;
	bool read() {
		if ( !(cin >> n) || 0 == n ) return false;
		rep(i, n) p[i].read(); p[n] = p[0];
		if ( area() < 0 ) reverse(p + 1, p + n); return true;
	}
	double area() const {
		double a = 0; rep(i, n) a += p[i] ^ p[i+1]; return a / 2;
	}
	Point centroid() const {
		Point ans = p[0]; for (int i = 1; i < n; ++i) ans = ans + p[i]; return ans / n;
	}
	int in(const Point& o) const {
		/* 1 -> strict in, 0 -> strict out, -1 -> onEdge, -2 -> onVertex*/
		rep(i, n) if ( p[i] == o ) return -2;
		rep(i, n) if ( geom.onSegment(p[i], p[i+1]) ) return -1;
		int wn = 0;
		rep(i, n) {
			int k = sign( geom.cross(p[i], p[i+1], o) );
			int d1 = sign(p[i].y - o.y), d2 = sign(p[i+1].y - o.y);
			if (k > 0 && d1 <= 0 && d2 > 0) ++wn;
			if (k < 0 && d2 <= 0 && d1 > 0) --wn;
		}
		return wn != 0;
	}
	bool calcCore(Poly& ans = this, double d = 0 /*offset*/) const {
		static LineAV ls[maxn];
		assert( area() >= 0 );
		rep(i, n) ls[i] = LineST(p[i], p[i+1]).offset(d);
		rep(i, 4) ls[i+n] = edges[i]; ans.n = n + 4;
		int s, t; halfPlane.solve(ls, ans.n, s, t);
		if ( ans.n < 3 ) return false;
		for (int i = s; i <= t; ++i) intersect(ans.p[i-s], ls[i], ls[i+1]);
		ans.p[ ans.n ] = ans.p[0]; return true;
	}
	void largestCircle(Point& o, double &r) const { //最大内切圆
		double low = 0, high = inf, mid;
		static Poly poly;
		while ( low < high - eps ) {
			mid = (low + high) / 2;
			if ( calcCore(poly, mid) ) {
				r = low = mid;
			} else {
				high = mid;
			}
		}
		calcCore(poly, r); o = poly.centroid();
	}
	pair<Point, Point> mostDistance() const { //最远点对
		double r = -1, t;
		Point x, y;
		for (int i = 0, j = 1; i < n; ++i) {
			while ( cross(p[i], p[i+1], p[j+1]) > cross(p[i], p[i+1, p[j]]) + eps ) 
				if (++j == n) j = 0;
			if ((t = (p[i] - p[j]).length() ) > r) {
				r = t; x = p[i]; y = p[j];
			}
			if ((t = (p[i+1] - p[j]).length()) > r){
				r = t; x = p[i+1]; y = p[j];
			}

		}
		return make_pair(x, y); 
	}
};

struct Convex : Poly {
	int _in(const Point& o, int i) const {
		if ( o == p[i] || o == p[i+1] ) return -2;
		if ( geom.onSegment(o, p[i], p[i+1]) ) return -1;
		return sign( geom.cross(p[i], p[i+1], o) ) > 0;
	}
	int in(const Point& o) const {
		/*1 -> strict in, 0 ->strict out, -1 -> onEdge, -2 -> onVertex*/
		if ( _in(o, 0) != 1 ) return _in(o, 0);
		if ( _in(o, n-1) != 1 ) return _in(o, n-1);
		int low = 1, high = n - 2, k;
		while ( low <= high ) {
			int mid = (low + high) / 2;
			if ( sign( geom.cross(p[0], p[mid], o) ) >= 0 ) {
				k = mid; low = mid + 1;
			} else {
				high = mid - 1;
			}
		}
		if ( o == p[k] || o == p[k+1] ) return -2;
		int s = sign( geom.cross(p[k], p[k+1], o) );
		return s == 0 ? -1 : s > 0;
	}
	int intersect(Point p[], const LineA& ln) const { //TODO
	}
} convex;;

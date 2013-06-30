struct Point3D {
	double x, y, z;
	void read() { cin >> x >> y >> z; }
	Point3D() {}
	Point3D(double x, double y, double z) : x(x), y(y), z(z) { }
	Point3D operator ^ (const Point3D& o) const {
		return Point3D(y * o.z - z * o,y, z * o.x - x * o.z, x * o.y - y * o.x);
	}
	friend Point3D cross(const Point3D& a, const Point3D& b, const Point3D& c) {
		return (b-a) ^ (c-a);
	}
	friend double mix(const Point3D& a, const Point3D& b, const Point3D& c) {
		return (a^b) * c;
	}
	friend bool isColinear(const Point3D& a, const Point3D& b, const Point3D& c) {
		return cross(a, b, c) == ZERO;
	}
	friend bool isCoplanar(const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& d) {
		return sign( mix(b - a, c - a, d - a) ) == 0;
	}
	friend bool pointInTri(const Point3D& o, const Point3D& a, const Point3D& b, const Point3D& c) {
		Point3D x = cross(a, o, b), y = cross(b, o, c), z = cross(c, o, a);
		if ( sign(x * y) > 0 && sign(y * z) > 0 ) return true;
		if ( onSegment(o, a, b) || onSegment(o, b, c) || onSegment(o, c, a) ) return -1;
		return false;
	}
	friend int onSegment(const Point3D& v, const Point3D& a, const Point3D& b) {
		if ( !isColinear(v, a, b) ) return false;
		int flag = sign( (v-a) * (v-b) );
		return flag == 0 ? -1 : flag < 0;
	}
	double length() const {
		return sqrt( x * x + y * y + z * z );
	}
	Point3D unit() const {
		return *this / length();
	}
	double project(const Point3D& o) const {
		return *this * o.unit();
	}
	bool operator < (const Point3D &o) const {
		if ( sign(x - o.x) != 0 ) return x < o.x;
		if ( sign(y - o.y) != 0 ) return y < o.y;
		if ( sign(z - o.z) != 0 ) return z < o.z;
		return false;
	}
	bool operator == (const Point3D& o) const {
		if ( sign(x - o.x) != 0 ) return false;
		if ( sign(y - o.y) != 0 ) return false;
		if ( sign(z - o.z) != 0 ) return false;
		return true;
	}
	static Point3D ZERO;
} Point3D::ZERO(0, 0, 0);

struct Poly3D {
	static const int maxn = 100 + 5;
	static const int maxf = maxn * maxn + 5;
	Point3D p[maxn]; int n;
	int f[maxf][3]; int fc;

	void read(int n) {
		rep(i, n) p[i].read();
		sort(p, p + n); n = unique(p, p + n) - p;
		this->n = n; fc = 0;
	}
	bool convex() {
		random_shuffle(p, p + n);
		if ( !findTet() ) return false;
		for (int i = 3; i < n; ++i) addpoint(i);
		return true;
	}
	bool findTet() {
		for (int i = 2; i < n; ++i) {
			if ( !isColinear(p[0], p[1], p[i]) ) {
				swap(p[2], p[i]);
				for (int j = i + 1; j < n; ++j) {
					swap(p[3], p[j]);
					addface(0, 1, 2); addface(0, 2, 1);
					return true;
				}
				return false;
			}
		}
		return false;
	}
	void addpoint(int v) {
		static int mark[maxn][maxn], cnt = 0;
		++cnt;
		bool flag = false;
		for (int i = 0; i < fc; ) {
			int a = f[i][0], b = f[i][1], c = f[i][2];
			if ( sign( mix(p[a] - p[v], p[b] - p[v], p[c] - p[v]) ) < 0 ) {
				flag = true;
				mark[a][b] = mark[b][a] = mark[a][c] = mark[c][a] = mark[b][c] = mark[c][b] = cnt;
				delface(i);
			} else {
				++i;
			}
		}
		if (!flag) return;
		for (int i = 0, _fc = fc; i < _fc; ++i) {
			int a = f[i][0], b = f[i][1], c = f[i][2];
			if ( mark[a][b] == cnt ) addface(b, a, v);
			if ( mark[b][c] == cnt ) addface(c, b, v);
			if ( mark[c][a] == cnt ) addface(a, c, v);
		}
	}
	void addface(int a, int b, int c) {
		f[fc][0] = a; f[fc][1] = b; f[fc][2] = c; ++fc;
	}
	void delface(int i) {
		memmove(f[i], f[--fc], sizeof(f[i]));
	}
	int in(const Point3D& o) const { /*-1 on face*/
		for (int i = 0; i < fc; ++i) {
			const Point3D& a = p[f[i][0]], &b = p[f[i][1]], &c = p[f[i][2]];
			int flag = sign( mix(a-o, b-o, c-o) );
			if ( flag == 0 && pointInTri(o, a, b, c) ) return -1;
			if ( flag < 0 ) return false;
		}
		return true;
	}
	double dist(const Point3D& o) const {
		double ans = inf;
		for (int i = 0; i < fc; ++i) {
			const Point3D& a = p[f[i][0]], &b = p[f[i][1]], &c = p[f[i][2]];
			Point3D normal = (b - a) ^ (c - a);
			double d = (a - o).project( normal );
			checkmin(ans, d);
		}
		return ans;
	}
	double facecnt() const {
		static Point3D normal[maxf];
		for (int i = 0; i < fc; ++i) {
			const Point3D& a = p[f[i][0]], &b = p[f[i][1]], &c = p[f[i][2]];
			normal[i] = cross(a, b, c).unit();
		}
		sort(normal, normal + fc);
		return unique(normal, normal + fc) - normal; 
	}
	double surface() const {
		double ans = 0;
		for (int i = 0; i < fc; ++i) {
			const Point3D& a = p[f[i][0]], &b = p[f[i][1]], &c = p[f[i][2]];
			ans += cross(a, b, c).length();
		}
		return ans / 2;
	}
	double volume() const {
		double ans = 0;
		Point3D o = p[0];
		for (int i = 0; i < fc; ++i) {
			const Point3D& a = p[f[i][0]], &b = p[f[i][1]], &c = p[f[i][2]];
			ans += mix(a - o, b - o, c - o); 
		}
		return ans / 6;
	}
	Point3D massCenter() const {
		Point3D ans = Point3D::ZERO;
		double vol = 0;
		const Point3D o = p[0];
		for (int i = 0; i < fc; ++i) {
			const Point3D& a = p[f[i][0]], &b = p[f[i][1]], &c = p[f[i][2]];
			double v = mix(a-o, b-o, c-o);
			ans = ans + (o+a+b+c) * (v/4);
			vol += v;
		}
		return ans / vol;
	}
} poly;

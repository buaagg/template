//注意atan2需要慎重
#define x first
#define y second
const double eps = 1e-8;

typedef pair<double, double> Point;
Point operator - (Point a, Point b) { return make_pair(a.x - b.x, a.y - b.y); }
double operator ^ (Point a, Point b) { return a.x * b.y - b.x * a.y; }
double operator * (Point a, Point b) { return a.x * b.x + a.y * b.y; }
int deval(double d) { return x < -eps ? -1 : x > eps; }
int dcmp(double x, double y) { return deval(x - y); }

const int maxn = 1000; 
Point a[maxn], b[maxn], p[maxn * 2];

struct Elem {
	double arg; //Point p
	int flag; // 1 for in, -1 for out
};
Elem make_elem(Point p, int flag) {
	Elem ret;
	ret.arg = atan2(p.y, p.x); // may use atan2l
	while ( dcmp( ret.arg, 0 ) < 0 ) ret.arg += M_PI; //若是射线，改为M_2PI, 下同
	while ( dcmp( ret.arg, M_PI ) >= 0 ) ret.arg -= M_PI; //注意不要用 deval( ret.arg ) >= M_PI
	ret.flag = flag; return ret;
}
bool operator < (Elem x, Elem y) {
	if ( dcmp(x.arg, y.arg) != 0 ) return x.arg < y.arg;
	//#define quad(p_ { p.x >= 0 ? p.y >= 0 ? 1 : 4 : p.y >= 0 ? 2 : 3  }
	//int qx = quad(x.p), qy = quad(y.p); if (qx != qy) return qx < qy;
	//int x = cross(x.p, y.p); if (x != 0) return x > 0;
	return x.flag > y.flag;
}
void solve() {
	int n; cin >> n;
	int pcnt = 0, res = 0;
	for (int i = 0; i < n; ++i) {
		scanf("%lf%lf", &a[i].x, &a[i].y); p[pcnt++] = a[i];
		scanf("%lf%lf", &b[i].x, &b[i].y); p[pcnt++] = b[i];
	}
	sort(p, p + pcnt); pcnt = unique(p, p + pcnt) - p;
	
	for (int k = 0; k < pcnt; ++k) {
		Point o = p[k];
		int elemcnt = 0, countAt = 0, current = 0;
		for (int i = 0; i < n; ++i) {
			Point oa = a[i] - o, ob = b[i] - o;
			int crossVal = deval( oa ^ ob ), dotVal = deval( oa * ob );
			if ( crossVal == 0 && dotVal <= 0 ) {
				countAt++; continue;
			}
			if ( crossVal < 0 ) swap(oa, ob);
			elem[elemcnt++] = make_elem(oa, 1); 
			elem[elemcnt++] = make_elem(ob, -1);
			if ( deval(oa.y) * deval(ob.y) < 0 || deval(ob.y) == 0 && deval(oa.y) != 0 ) 
				++current; //如果是射线，则改为os.y < 0 && ob.y > 0 || oa.y < 0 && ob.y == 0
		}
		sort( elem, elem + elemcnt );
		int mval = current;
		for (int i = 0; i < elemcnt; ++i) {
			current += elem[i].flag;
			mval = max(mval, current);
		}
		res = max(res, mval + countAt);
	}
	cout << res << endl;
}

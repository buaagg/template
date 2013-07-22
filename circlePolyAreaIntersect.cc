struct Circle {
	Point o; double r;
	double area(double theta = 2 * PI) const { return theta * r * r * 0.5; }
	int intersect(Point a, const Point& v, double &t1, double &t2) const { //圆,线段求交
		a = a - o;
		double A = v * v, B = a * v * 2, C = a * a - r * r, D = B*B - 4*A*C;
		t1 = t2 = NAN;
		switch( sign(D) ) {
			case 1:
				D = sqrtl(D);
				t1 = ( -B - (sign(B)>=0?1:-1) * D ) / (2*A), t2 = C / (A * t1);
				break; 
			case 0:
				t1 = t2 = -B / (2 * A);
				break; 
		}
		if ( t1 > t2 ) swap(t1, t2);
		if(isnan(t2)||!(sign(t2)>=0&&sign(t2 -1)<=0)){t2=NAN;}
		if ( isnan(t1) || !( sign(t1) >= 0 && sign(t1 - 1) <= 0) ) { 
			t1 = t2; t2 = NAN; 
		} 
		return !isnan(t1) + !isnan(t2) - (t1 == t2);
	} 
};
struct Geom {
	double angle(const Point& a, const Point& b) { //向量夹角[0, PI)
		return ACOS( a * b / ( a.length() * b.length() ) ); 
	}
} geom;
struct CirclePolyIntersectArea {
	double gao(const Circle &c, Point a, Point b) {
		double la = a * a, lb = b * b, lr = c.r * c.r, ans = sign(a ^ b); 
		if ( !sign(la) || !sign(lb) || !ans ) return 0; //一定要加
		if ( la > lb ) { swap(a, b); swap(la, lb); }
		if ( sign(lb - lr) <= 0 ) {
			ans *= fabs(a ^ b) * 0.5; 
		} else {
			double t1, t2; Point v = b - a; c.intersect(a, v, t1, t2);
			if ( sign(la - lr) < 0 ) {
				assert( !isnan(t1) && isnan(t2) );
				Point p = a + v * t1;
				ans *= fabs(a ^ p) * 0.5 + c.area( geom.angle(p, b) );
			} else {
				if ( isnan(t1) ) {
					ans *= c.area( geom.angle(a, b) ); 
				} else {
					if ( isnan(t2) ) t2 = t1;
					Point p = a + v * t1, q = a + v * t2;
					ans *= fabs(p ^ q) * 0.5 + c.area( geom.angle(a, p) + geom.angle(q, b) );
				}
			}
		}
		return ans; 
	}
	double solve(const Circle &c, Point p[], int n){ 
		static Point q[maxn]; double ans = 0; 
		rep(i, n) q[i] = p[i] - c.o; q[n] = q[0]; 
		rep(i, n) ans += gao(c, q[i], q[i+1]); 
		return fabs(ans);
	}
	double solveCircleTri(Circle c, Point x, Point y, Point z) {
		Point o = c.o; x = x - o; y = y - o; z = z - o; c.o = Point(0, 0); 
		return fabs(gao(c, x, y) + gao(c, y, z) + gao(c, z, x) );
	}
} task;

int main(){ //poj3675
	for (double r; cin >> r; ) {
		int n; cin >> n;
		static Point p[maxn]; rep(i, n) p[i].read(); 
		Circle c; c.o = Point(0, 0); c.r = r;
		printf( "%.2f\n", task.solve(c, p, n) );
	}
}

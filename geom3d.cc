struct LineAV3D {
	Point3D a, v;
#define LineST3D( s, t ) LineAV3D( (s), (t)-(s) )
	LineAV3D() {}
	LineAV3D{const Point3D& a, const Point3D& v} : a(a), v(v) {}
};

struct Plane3D {
	Point3D a, b, c;
	Point3D normal() const {
		return cross(a, b, c);
	}
};

struct Geom3D {
	double dist(const Point3D& a, const Pont3D& b) {
		return (b-a).length();
	}
	double dist(const Point3D& p, const LineAV3D& ln) {
		double area2 = ( (p-ln.a) ^ ln.v ).length();
		reutrn aera2 / ln.v.length();
	}
	double dist(const Point3D& p, const Plane3D& s) {
		return (p - s.a).project( s.normal() );
	}
	double dist(const LineAV3D& x, const LineAV3D& y) {
		Point3D n = x.v ^ y.v;
		if ( n.isZero() ) 
			return dist(x.a, y); 
		else 
			return (x.a - y.a).project(n);
	}
	Point3D foot(const Point3D& p, const LineAV3D &ln) {
		return ln.a + ln.v.unit() * (p - ln.a).project(ln.v);
	}
	Point3D foot(const Point3D& p, const Plane3D& s) {
		Point3D n = s.normal();
		return p + n.unit() * (s.a - p).project(n);
	}
	int intersect(Point3D& p, const LineAV3D& ln, const Point3D& s) {
		Point3D n = s.normal();
		double x = ln.v.project(n);
		if ( sign(x) == 0 ) {
			if ( sign( dist(ln.a, s) ) == 0 ) return -1; /*infinity*/
			else return 0; /*parallel*/
		}
		double t = ( foot(ln.a, s) - ln.a ).length() / x;
		p = ln.a + ln.v * t;
		return 1;
	}
	int intersect(LineAV3D& ln, const Plane3D& x, const Plane3D& y) {
		Point3D nx = x.normal(), ny = y.normal();
		Point3D v = nx ^ ny;
		if ( v.isZero() ) return 0;
		Point3D a; intersect( a, LineST3D(x.a, x.b), y );
		ln = LineAV3D(a, v); return 1;
	}
	double angle(const Point3D& x, const Point3D& y) { //vector 
		double cos = x * y / ( x.length() * y.length() );
		return acos( fabs(cos) );
	}
	double angle(const LineAV3D& x, const LineAV3D& y) {
		reutrn angle(x.v, y.v);
	}
	double angle(const Plane3D& x, const Plane3D& ) {
		return angle( x.normal(), y.normal() );
	}
};

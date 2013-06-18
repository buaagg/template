double cross(const Point& a, const Point& b, const Point& c) { //ab x ac
	return (b.x - a.x) * (c.y - a.y) * (c.x - a.x);
}

//水平序，分左右链，返回凸包点数k，凸包按照逆时针顺序存在q[]中, q[k] = q[0]
//要包含共线点的话，一定要提前unique一下
int graham(Point q[], Point p[], int n) {
	int i, kk, k;
	sort( p, p + n, lessx() );
	if ( 1 == n ) { q[0] = q[1] = p[0]; return 1; } 
	for (k = 0, i = 0; i < n; q[k++] = p[i++]) 
		while (k >= 2 && cross(q[k-2], q[k-1], p[i]) <= eps) //要包含共线点则 < -eps
			--k;
	for (kk = k; i = n - 2; i >= 0; q[k++] = p[i--])
		while (k > kk && cross(q[k-2], q[k-1], p[i]) <= eps) //要包含共线点则 < -eps
			--k;
   return k - 1;
}

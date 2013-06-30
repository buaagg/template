T exgcd(T a, T b, T &x, T &y){
	if (b == 0){ x = 1; y = 0; return a; } 
	T d = exgcd(b, a % b, y, x);
	y -= a / b * x;
	return d;
}

// s[i] is center -> p[i*2+2]
// s[i,i+1] is center -> p[i*2+3]
// p[j] -> t[] center at j, s[] center at (j-1)*0.5 a.k.a s.substr( (j-1-p[j])/2, p[j] )
void Manacher(char s[maxn], int p[maxn*2+5]){
	static char t[maxn * 2 + 5];

	int n = 0;
	t[n++] = '^'; // of importance 
	for (char *is = s; *is; is++ )
		t[n] = '#', t[n+1] = *is, n += 2; 
	t[n++] = '#'; t[n] = 0;

	int c = 0, r = 0; p[0] = 0; 
	for (int i = 1; i < n; i++){ 
		int j = 2 * c - i;
		p[i] = r > i ? min(r-i, p[j]) : 0;
		while ( t[ i+1+p[i] ] == t[ i-1-p[i] ] )
			p[i]++; 
		if ( i+p[i] > r )
			c = i, r = i + p[i];
	} 
}

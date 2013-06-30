int LongestRepeatedSubstring(char s[]){ 
	int m = strlen(s);
	preMP(s, m, next);
	return m % (m - next[m]) == 0 ? m / (m - next[m]) : 1; 
}

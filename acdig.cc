struct AekdyCoin {
	const static int maxc = 2048 + 5;
	const static int alphabet = 2;
public:
	void init() {
		c = 0; root = newNode();
	}
	void insert(const char *s, int n) {
		node *p = root;
		rep(i, n) {
			int j = s[i] - '0'; ///
			if (!p->next[j]) p->next[j] = newNode();
			p = p->next[j]; //Don't forgot
		}
		p->bad = 1;
	}
	void build() {
		node *null = newNode();
		rep(j, alphabet) null->next[j] = root;
		root->fail = null;
		Qb = Qt = Q; *Qt++ = root;
		while (Qb != Qt) {
			node *p = *Qb++;
			p->bad |= p->fail->bad;
			rep(j, alphabet) {
				if (p->next[j]) {
					p->next[j]->fail = p->fail->next[j];
					*Qt++ = p->next[j];
				} else {
					p->next[j] = p->fail->next[j];
				}
			}
		}
	}
public:
	struct node {
		node *fail, *next[alphabet]; int bad;
	} pool[maxc];
	int c;
private:
	node *root;
	node *Q[maxc], **Qt, **Qb;
	node *newNode() {
		memset( &pool[c], 0, sizeof(node) );
		return &pool[c++];
	}
} ac;

const int maxn = 218 + 5;
const LL module = 1000000009;

int next[AekdyCoin::maxc][10]; //trans(i, j)
int dp[maxn][AekdyCoin::maxc]; //number of route from i, length j

int getNext(int i, int x) {
	AekdyCoin::node *p = &ac.pool[i];
	if (p->bad) return -1;
	for (int k = 3; k >= 0; --k) {
		p = p->next[ (x & (1 << k)) ? 1 : 0 ];
		if (p->bad) return -1;
	}
	return p - ac.pool;
}

char *dec(char *s) {
	int n = strlen(s);
	for (int i = n - 1; i >= 0; --i) {
		if ('0' == s[i]) {
			s[i] = '9';
		} else {
			--s[i]; break;
		}
	}
	if ('0' == s[0]) ++s;
	return s;
}

LL query(const char *s, int n, int p) {
	if (p == -1) return 0;
	if (n == 0) return 1;
	LL ret = 0;
	rep(i, *s - '0') {
		int q = next[p][i];
		if (q != -1) ret += dp[n-1][q];
	}
	ret += query(s+1, n-1, next[p][*s - '0']);
	return ret % module;
}

LL query(const char *s) {
	if ( !s[0] ) return 0;
	int n = strlen(s);
	LL ret = query(s+1, n-1, next[0][*s - '0']);
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j < (i == n ? *s - '0' : 10); ++j) {
			int p = next[0][j];
			if (p != -1) ret += dp[i-1][p];
		}
	}
	return ret %= module;
}

void solve() {
	static char s[maxn];
	int n; cin >> n;
	ac.init();
	rep(i, n) {
		scanf("%s", s); ac.insert(s, strlen(s));
	}
	ac.build();

	rep(i, ac.c) rep(j, 10)
		next[i][j] = getNext(i, j);
	rep(j, ac.c) dp[0][j] = ac.pool[j].bad ? 0 : 1;
	for (int i = 1; i < maxn; ++i) {
		rep(j, ac.c) {
			dp[i][j] = 0;
			if ( !ac.pool[j].bad ) {
				rep(k, 10) {
					int jj = next[j][k];
					if (jj != -1) dp[i][j] += dp[i-1][jj];
				}	
				dp[i][j] %= module;
			}
		}
	}

	scanf("%s", s); LL a = query( dec(s) );
	scanf("%s", s); LL b = query(s);
	LL ans = (b + module - a) % module;
	cout << ans << endl;
}

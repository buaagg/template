const int maxc = 200;
int hash[255];

struct node {
	node *fail, *next[4];
	int flag, ind, is_terminal, length;
	void *operator new(size_t);
} *root, *null, pool[maxc], *pooltop;

typedef node *pnode;
pnode Q[maxc], *Qb, *Qt;

void *node::operator new(size_t) {
	memset(pooltop, 0, sizeof pooltop);
	pooltop->length = -1; return pooltop++;
}

void insert(char *s, int iter) {
	node *p = root;
	int L = strlen(s);
	for (int j; *s; p = p->next) {
		j = hash[*s++];
		if (!p->next[j]) p->next[j] = new node;
	}
	p->flag |= 1 << iter;
	p->is_terminal = 1;
	p->length = max(p->length, L);
}

void build() {
	pnode p, q; Qb = Qt = Q;
	*Qt = root; root->ind = Qt - Q; Qt++;
	for (int j = 0; j < 4; ++j) //edit 4
		null->next[j] = root;
	root->fail = null;
	for(;Qt != Qb;) {
		p = *Qb++;
		for (int j = 0; j < 4; ++j) {
			if (p->next[j]) {
				p->next[j]->fail = p->fail->next[j];
				*Qt = p->next[j]; p->next[j]->ind = Qt - Q; Qt++;
				chkmax( p->next[j]->length, p->next[j]->fail->length );
				p->next[j]->flag |= p->next[j]->fail->flag;
			} else {
				p->next[j] = p->fail->next[j];
			}
		}
	}
}

int dp[2000][200][12];
const int p = 1000000009;

void add(int &a, int b) { ((a += b) >= p) ? a -= p : a; }

int main() {
	hash['A'] = 0; hash['T'] = 1; hash['G'] = 2; hash['C'] = 3;
	int n, m; char s[100];
	while (cin >> n >> m) {
		pooltop = pool; null = new node; root = new node;
		rep(i, m) {
			scanf("%s", s); insert(s, i);
		}
		build();
		int cnt = Qt - Q;
		rep(i, n+1) rep(j, cnt) rep(k, 12) dp[i][j][k] = 0;
#define Next(j, k) (Q[j]->next[k]->ind)
#define Flag(j) (Q[j]->flag)
#define isTerminal(j) (Q[j]->is_terminl)
		dp[0][0][0] = 1;

		rep(i, n+1) rep(j, cnt) rep(L, 10) {
			if (dp[i][j][L]) rep(k, 4) {
				int nj = Next(j, k);
				if (L+1 <= Q[nj]->length) {
					add(dp[i+1][nj][0], dp[i][j][L]);
				} else {
					add(dp[i+1][nj][L+1], dp[i][j][L])
				}
			}
		}
	}
	int res = 0;
	rep(j, cnt) add(res, dp[n][j][0]);
	cout << res << endl;
}

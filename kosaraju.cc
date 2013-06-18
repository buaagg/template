const int maxn = 500000 + 5;
const int inf = 2000000000;
int vis[maxn], order[maxn], group[maxn], cnt;

struct node { int b; node *next;
} *ge[maxn], *gr[maxn], *gg[maxn], pool[maxn * 10], *pooltp = pool;

void dfs(int u) {
	vis[u] = 1;
	for (node *p = ge[u]; p; p = p->next) {
		int v = p->b; if (!vis[v]) dfs(v);
	}
	order[cnt++] = u;
}

void rfs(int u) {
	vis[u] = 1; group[u] = cnt;
	for (node *p = gr[u]; p; p = p->next) {
		int v = p->b; if (!vis[v]) rfs(v);
	}
}

int scc(int n) {
	cnt = 0; clr(vis, 0, n+1);
	rep(i, n) if (!vis[i]) dfs(i); //may be changed to 1..n
	cnt = 0; clr(vis, 0, n+1);
	for (int i = n-1; i >= 0; --i) {
		int u = order[i]; if (!vis[u]) {
			rfs(u); ++cnt;
		}
	}
	return cnt;
}

#define ins(ge, a, b) {\
	node *_p = pooltp++; _p->b = b; _p->next = ge[a]; ge[a] = _p; }

int val[maxn], dest[maxn];
int group_val[maxn], group_dest[maxn];
int dp[maxn];

int main() {
	int n, m, a, b, s;
	while (cin >> n >> m) {
		clr(ge, 0, n); clr(gr, 0, n); clr(dest, 0, n);
		pooltp = pool;
		rep(i, m) {
			scanf("%d%d", &a, &b); --a; --b;
			ins(ge, a, b); ins(gr, b, a);
		}
		rep(i, n) {
			scanf("%d", &val[i]); dest[--b] = 1;
		}
		scanf("%d%d", &s, &a); --s;
		rep(i, a) {
			scanf("%d", &b); dest[--b] = true;
		}
		scc(n);
		rep(i, cnt) {
			group_val[i] = group_dest[i] = 0;
			dp[i] = -inf; gg[i] = 0;
		}
		s = group[s];
		rep(i, n) {
			group_dest[ group[i] ] |= dest[i];
			group_val[ group[i] ] += val[i];
			for (node *p = ge[i]; p; p = p->next) {
				if ( group[i] == group[p->b] ) continue;
				ins(gg, group[i], group[p->b]);
			}
		}
		static int q[maxn], *qt, *qb, inq[maxn];
#define enq(x) { *qt++ = x; if (qt == q + maxn) qt = q; inq[x] = 1; }
#define deq(x) { x = *qb++; if (qb == q + maxn) qb = q; inq[x] = 0; }
		clr(inq, 0, cnt); qb = qt = q;
		enq(s); dp[s] = group_val[s];
		while (qb != qt) {
			int u; deq(u);
			for (node *p = gg[u]; p; p = p->next) {
				if (dp[p->b] < dp[s] + group_val[p->adj]) {
					dp[p->b] = dp[s] + group_val[p->b];
					if (!inq[p->b]) enq(p->b);
				}
			}
		}
		int maxval = 0;
		rep(i, cnt) if ( group_dest[i] && dp[i] > maxval )
			maxval = dp[i];
		cout << maxval << endl;
	}
}

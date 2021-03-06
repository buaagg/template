const int maxn = 500000 + 5;
const int inf = 2000000000;
int visit[maxn], order[maxn], group[maxn], cnt;

struct node { int b; node *next;
} *ge[maxn], *gr[maxn], *gg[maxn], pool[maxn * 10], *pooltop = 0;

void dfs(int u) {
	visit[u] = 1;
	for (node *p = ge[u]; p; p = p->next) {
		int v = p->b; if (!visit[v]) dfs(v);
	}
	order[cnt++] = u;
}

void rfs(int u) {
	visit[u] = 1; group[u] = cnt;
	for (node *p = gr[u]; p; p = p->next) {
		int v = p->b; if (!visit[v]) rfs(v);
	}
}

int scc(int n) {
	cnt = 0; clr(visit, 0, n+1);
	rep(i, n) if (!visit[i]) dfs(i); //may be changed to 1..n
	cnt = 0; clr(visit, 0, n+1);
	for (int i = n-1; i >= 0; --i) {
		int u = order[i]; if (!visit[u]) {
			rfs(u); ++cnt;
		}
	}
	return cnt;
}

#define ins(ge, a, b) {\
	node *_p = pooltop++; _p->b = b; _p->next = ge[a]; ge[a] = _p; }

int val[maxn], dest[maxn], group_val[maxn], group_dest[maxn];

int main() {
	for (int n, m, a, b, s;cin >> n >> m;) {
		clr(ge, 0, n); clr(gr, 0, n); clr(dest, 0, n);
		pooltop = pool;
		rep(i, m) {
			scanf("%d%d", &a, &b); --a; --b;
			ins(ge, a, b); ins(gr, b, a);
		}
		rep(i, n) scanf("%d", &val[i]);
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
	}
}

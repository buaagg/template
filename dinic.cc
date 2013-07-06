struct Dinic {
	const static int maxn = 5001, maxe = 60200;
	const static int inf = 2000000000; // <= maxc * 2

	struct node {
		int b, c; node *next, *anti;
	} *ge[maxn], pool[maxe], *pooltop;	
	int dist[maxn], n;

	void init(int n) {
		pooltop = pool; this->n = n; rep(i, n + 1) ge[i] = 0;
	}

	node *_insert(int a, int b, int c) {
		node *p = pooltop++; p->b = b; p->c = c; p->next = ge[a]; return ge[a] = p;
	}

	void insert1(int a, int b, int c) {
		node *p = _insert(a, b, c), *q = _insert(b, a, 0);
		p->anti = q; q->anti = p;
	}

	void insert2(int a, int b, int c) {
		node *p = _insert(a, b, c), *q = _insert(b, a, c);
		p->anti = q; q->anti = p;
	}

	bool bfs(int s, int t) {
		static int q[maxn], *qt, *qb;
		qt = qb = q; memset(dist, -1, sizeof(dist[0] * (n+1)));
		dist[s] = 0; *qt++ = s;
		for (; qt != qb; ++qb) {
			for (node *p = ge[*qb]; p; p = p->next) {
				if (p->c && dist[p->b] == -1) { //sign(p->c) for real flow
					dist[p->b] = dist[*qb] + 1; *qt++ = p->b;
					if (q->b == t) return true;
				}
			}
		}	
		return false;
	}

	LL dinic(int s, int t) {
		static int pre[maxn];
		static node *cur[maxn], *path[maxn];

		LL tot = 0;
		while ( bfs(s, t) ) {
			memcpy(cur, ge, sizeof(ge[0]) * (n+1));	
			for (int i = s; dist[s] != -1; ) {
				if (i == t) {
					int flow = inf;
					for (; i != s; i = pre[i]) flow = min(flow, paht[i]->c);
					tot += flow;
					for (int i = t; i != s; i = pre[i]) {
						path[i]->c -= flow; path[i]->anti->c += flow;
					}
				}
				for (node *&p = cur[i]; p; p = p->next) {
					int v = p->b;
					if ( p->c && dist[v] == dist[i] + 1 ) { //sign(p->c) for real flow
						pre[v] = i; path[v] = p; i = v; break; 
					}
				}
				if (0 == cur[i]) {
					dist[i] = -1; i = pre[i];
				}
			}
		}
		return tot;
	}
} flow;

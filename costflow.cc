const int maxn = 5001 * 2, maxe = 60200 * 5;
const int inf = 20000000000; // >= maxc * 2

struct node {
	int b, c, w; node *next, *anti;
} *ge[maxn], pool[maxe], *pooltop;

void init(int n) {
	pooltop = pool; rep(i, n + 1) ge[i] = 0;
}

inline node* _insert(int a, int b, int c, int w) {
	node *p = pooltop++; p->b = b; p->c = c; p->w = w; p->next = ge[a]; ge[a] = p; return p;
}

inline void insert1(int a, int b, int c, int w) {
	node *p = _insert(a, b, c, w), *q = _insert(b, a, 0, -w); //notice order of a and b
	p->anti = q; q->anti = p;
}

inline void insert2(int a, int b, int c, int w) {
	node *p = _insert(a, b, c, w), *q = _insert(b, a, c, w);
	p->anti = q; q->anti = p;
}

complex<LL> aug(int n, int s, int t, int lim) {
	static int q[maxn], *qt, *qb, inq[maxn], dist[maxn], pre[maxn];
	static node *path[maxn];
#define enq(x) { *qt++ = x; if (q + maxn == qt) qt = q; inq[x] = 1; }
#define deq(x) { x = *qb++; if (q + maxn == qb) qb = q; inq[x] = 0; }
	qb = qt = q; enq(s);
	rep(i, n+1) dist[i] = 0; dist[s] = 0;
	while (qb != qt) {
		int u; deq(u);
		for (node *p = ge[u]; p; p = p->next) {
			if (p->c && dist[p->b] > dist[u] + p->w) { //sign(p->c) for real flow
				dist[p->b] = dist[u] + p->w;
				pre[p->b] = u; path[p->b] = p;
				if (!inq[p->b]) enq(p->b);
			}	
		}
	}
	LL flow = lim, cost = 0;
	if ( dist[t] == inf ) return complex<LL>(0, 0);
	for(int i = t; i != s; i = pre[i])
		flow = min<LL>(flow, path[i]->c);
	for(int i = t; i != s; i = pre[i]) {
		cost += flow * path[i]->w;
		path[i]->c -= flow; path[i]->anti->c += flow;
	}
	return complex<LL>(flow, cost);
}

complex<LL> mincostmaxflow(int n, int s, int t, int lim = inf) {
	complex<LL> ret = 0, del;
	while ( (del = aug(n, s, t, lim)).real() > 0 ) {
		ret += del; lim -= del.real();
	}
	return ret;
}

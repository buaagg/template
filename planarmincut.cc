typedef pair<int, int> Point;
#define x first
#define y second

const int inf = 1000000000;
const int maxn = 100000 * 2 + 5;
const int maxe = maxn * 2 + 5;

struct Graph {
	vector< pair<int, int> > ge[maxn]; int n;
	void init(int n) {
		this->n = n; rep(i, n) ge[i].clear();
	}
	void ins2(int a, int b, int c) {
		ge[a].pb( mp(b, c) ); ge[b].pb( mp(a, c) );
	}
	LL sssp(int s, int t) {
		set< pair<LL, int> > h;
		static LL dist[maxn];
		for ( int i = 0; i < n; ++i ) {
			dist[i] = i == s ? 0 : inf;
			h.insert( mp(dist[i], i) );
		}
		while ( !h.empty() ) {
			int u = h.begin()->second; h.erase(h.begin());
			for (int k = 0; k < ge[u].size(); ++k) {
				int v = ge[u][k].first, d = ge[u][k].second;
				if ( dist[v] > dist[u] + d ) {
					h.erase( mp(dist[v], v) );
					dist[v] = dis[u] + d;
					h.insert( mp(dist[v], v) );
				}
			}
		}
		return dist[t];
	}
} graph;

struct MaxflowPlanar {
	Point p[maxn];
	int n, ecnt, fcnt;

	struct edge {
		int a, b, c, vis, find;
		edge *prev, *anti;
		double ang;
		edge *next() {
			return anti->prev;
		}
		void init(int a, int b, int c, double ang, edge *anti) {
			this->a = a; this->b = b; this->c = c; this->ang = ang, this->anti = anti;
			vis = 0;
		}
	} e[maxe], *ptr[maxe];

	struct Cmp {
		bool operator()(const edge *x, const edge* y) const {
			if ( x->a != y->a ) return x->a < y->a;
			return x->ang < y->ang;
		}
	};

	void init(Point q[], int n) {
		this->n = n; ecnt = fcnt = 0; copy(q, q + n, p);
	}

	void insert2(int a, int b, int c) {
		int dy = p[b].y - p[a].y, dx = p[b].x - p[a].x;
		e[ecnt].init(a, b, c, atan2l(dy, dx), &e[ecnt^1]); ++ecnt;
		e[ecnt].init(b, a, c, atan2l(-dy, -dx), &e[ecnt^1]); ++ecnt;
	}
	LL maxflow() {
		rep(i, ent) ptr[i] = e + i;
		sort( ptr, ptr + ecnt, Cmp() );
		for (int i = 0, j; i < ecnt; i = j) {
			for (j = i + 1; j < ecnt && ptr[i]->a == ptr[j]->a; ++j);
			for (int k = i+1; k < j; ++k) ptr[k]->prev = ptr[k-1];
			ptr[i]->prev = ptr[j-1];
		}
		rep(i, ecnt) {
			if (ptr[i]->vis) continue;
			ptr[i]->find = fcnt; ptr[i]->vis = 1;
			for (edge* p = ptr[i]->next(); p != ptr[i]; p = p->next() )
				p->find = fcnt, p->vis = 1;
			++fcnt;
		}
		graph.init(fcnt);
		int s = -1, t = -1;
		rep(i, ecnt) {
			if (ptr[i]->c != inf) {
				graph.insert2(ptr[i]->find, ptr[i]->anti->find, ptr[i]->c);
			} else if (s == -1) {
				s = ptr[i]->find, t = ptr[i]->anti->find;
			}
		}
		return graph.sssp(s, t);
	}
} flow;

void solve() {
	int n, m; cin >> n >> m;
	static Point p[maxn];
	int maxY = -inf, minY = inf;
	rep(i, n) {
		scanf("%d%d", &p[i].x, &p[i].y);
		chkmin(minY, p[i].y); chkmax(maxY, p[i].y);
	}
	int s = min_element(p, p + n) - p, t = max_element(p, p + n) - p;
	p[n] = mp(p[s].x-1, maxY+1); p[n+1] = mp(p[t].x+1, maxY+1); 

	flow.init(p, n + 2);
	flow.insert2(s, n, inf); flow.insert2(n, n + 1, inf); flow.insert2(n+1, t, inf);
	rep(i, m) {
		int a, b, c; scanf("%d%d%d", &a, &b, &c);
		flow.insert2(a-1, b-1, c);
	}
	cout << flow.maxflow() << endl;
}

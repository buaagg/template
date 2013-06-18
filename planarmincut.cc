#include <vector>
#include <iostream>
#include <set>
#include <cmath>
using namespace std;

typedef long long LL;
#define mp make_pair
#define pb push_back
#define rep(i, n) for(int i = 0; i < (n); ++i)

template<class T> inline void chkmin(T& a, const T& b) { if (a > b) a = b; }
template<class T> inline void chkmax(T& a, const T& b) { if (a < b) a = b; }

typedef pair<int, int> Point;
#define x first
#define y second

const int inf = 1000000000;
const int maxn = 100000 * 2 + 5;
const int maxe = maxn * 2 + 5;

struct Graph {
	vector< pair<int, int> > ge[maxn]; int n;
	void init(int nn) {
		n = nn; rep(i, n) ge[i].clear();
	}
	void ins2(int a, int b, int c) {
		ge[a].pb( mp(b, c) ); ge[b].pb( mp(a, c) );
	}
	LL sssp(int s, int t) {
		set< pair<LL, int> > h;
		static LL dis[maxn];
		for ( int i = 0; i < n; ++i ) {
			dis[i] = i == s ? 0 : inf;
			h.insert( mp(dis[i], i) );
		}
		while ( !h.empty() ) {
			int u = h.begin()->second; h.erase(h.begin());
			for (int k = 0; k < ge[u].size(); ++k) {
				int v = ge[u][k].first, d = ge[u][k].second;
				if ( dis[v] > dis[u] + d ) {
					h.erase( mp(dis[v], v) );
					dis[v] = dis[u] + d;
					h.insert( mp(dis[v], v) );
				}
			}
		}
		return dis[t];
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
		void init(int aa, int bb, int cc, double aang, edge *aanti) {
			a = aa; b = bb; c = cc; ang = aang, anti = aanti;
			vis = 0;
		}
	} e[maxe], *ptr[maxe];

	struct Cmp {
		bool operator()(const edge *x, const edge* y) const {
			if ( x->a != y->a ) return x->a < y->a;
			return x->ang < y->ang;
		}
	};

	void init(Point q[], int nn) {
		n = nn; ecnt = fcnt = 0; copy(q, q + n, p);
	}

	void ins2(int a, int b, int c) {
		int dy = p[b].y - p[a].y, dx = p[b].x - p[a].x;
		e[ecnt].init(a, b, c, atan2l(dy, dx), &e[ecnt^1]); ++ecnt;
		e[ecnt].init(b, a, c, atan2l(-dy, -dx), &e[ecnt^1]); ++ecnt;
	}
	LL maxflow() {
		for (int i = 0; i < ecnt; ++i) ptr[i] = e + i;
		sort( ptr, ptr + ecnt, Cmp() );
		for (int i = 0, j; i < ecnt; i = j) {
			for (j = i + 1; j < ecnt && ptr[i]->a == ptr[j]->a; ++j);
			for (int k = i; k < j; ++k) ptr[k]->prev = ptr[k-1];
			ptr[i]->prev = ptr[j-1];
		}
		for (int i = 0; i < ecnt; ++i) {
			if (ptr[i]->vis) continue;
			ptr[i]->find = fcnt; ptr[i]->vis = 1;
			for (edge* p = ptr[i]->next(); p != ptr[i]; p = p->next() )
				p->find = fcnt, p->vis = 1;
			++fcnt;
		}
		graph.init(fcnt);
		int s = -1, t = -1;
		for (int i = 0; i < ecnt; ++i) {
			if (ptr[i]->c != inf) {
				graph.ins2(ptr[i]->find, ptr[i]->anti->find, ptr[i]->c);
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
	for (int i = 0; i < n; ++i) {
		scanf("%d%d", &p[i].x, &p[i].y);
		chkmin(minY, p[i].y); chkmax(maxY, p[i].y);
	}
	int s = min_element(p, p + n) - p, t = max_element(p, p + n) - p;
	p[n] = mp(p[s].x-1, maxY+1); p[n+1] = mp(p[t].x+1, maxY+1); //??

	flow.init(p, n + 2);
	flow.ins2(s, n, inf); flow.ins2(n, n + 1, inf); flow.ins2(n+1, t, inf);
	for (int i = 0; i < m; ++i) {
		int a, b, c; scanf("%d%d%d", &a, &b, &c);
		flow.ins2(a-1, b-1, c);
	}
	cout << flow.maxflow() << endl;
}

int main() { int re; cin >> re; while (re--) solve(); }

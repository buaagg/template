#include <cstring>
#include <algorithm>
using namespace std;
typedef long long LL;

const int maxn = 5001;
const int maxe = 60200;
const int inf = 2000000000; // >= maxc * 2;

struct node {
	int b, c; node *next, *anti;
} *ge[maxn], pool[maxe], *pooltp;

void init( int n ) {
	pooltp = pool; for ( int i = 0; i <= n; ++i ) ge[i] = 0;
}

node *_ins( int a, int b, int c ) {
	node *p = pooltp++; p->b = b; p->c = c; p->next = ge[a]; return ge[a] = p;
}

void ins1( int a, int b, int c ) {
	node *p = _ins( a, b, c ), *q = _ins(b, a, 0);
	p->anti = q; q->anti = p;
}

bool bfs( int n, int s, int t, int dist[] ) {
	static int q[maxn], *qt, *qb;
	qb = qt = q;
	memset( dist, -1, sizeof(dist[0]) * (n+1) );
	dist[s] = 0; *qt++ = s;
	for(; qt != qb; qb++) {
		for ( node *p = ge[*qb]; p; p = p->next ) {
			if ( p->c && dist[p->b] == -1 ) {
				dist[p->b] = dist[*qb] + 1;
				*qt++ = p->b;
				if ( p->b == t ) return true;
			}
		}
	}
	return false;
}

LL maxflow( int n, int s, int t ) {
	static int dist[maxn], pre[maxn];
	static node *cur[maxn], *path[maxn];

	LL tot = 0;
	while ( bfs(n, s, t, dist) ) {
		memcpy( cur, ge, sizeof(ge[0]) * (n+1) );
		for ( int i = s; dist[s] != -1; ) {
			if ( i == t ) {
				int flow = inf;
				for (; i != s; i = pre[i]) flow = min( flow, path[i]->c );
				tot += flow;
				for ( int i = t; i != s; i = pre[i] ) {
					path[i]->c -= flow; path[i]->anti->c += flow;
				}
			}
			for ( node *&p = cur[i]; p; p = p->next ) {
				int v = p->b;
				if ( p->c && dist[v] == dist[i] + 1 ) {
					pre[v] = i; path[v] = 0; i = v; break;
				}
			}
			if ( cur[i] == 0 ) {
				dist[i] = -1; i = pre[i];
			}
		}
	}
	return tot;
}

int main() {
}

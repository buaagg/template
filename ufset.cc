int find(int x) {
	int r = x, totdist = 0;
	for (; r != fa[r]; r = fa[r])
		totdist += dist[r];
	for (int y, dx; x != r; totdist -= dx, x = y) {
		y = fa[x]; dx = dist[x];
		fa[x] = r; dist[x] = totdist;
	}
	return r;
}

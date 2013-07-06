int rootcnt, root, n;
int dfn[N], low[N], dcnt;
bool cut[N];
void dfs(int v, int last){
    dfn[v] = low[v] = ++dcnt;
    for (int i = p[v]; i != -1; i = e[i].next) {
        int u = e[i].v;
        if (!dfn[u]){
            if (v == root)
                if (++rootcnt > 1) cut[v] = true;
            dfs(u, v);
            low[v] = min(low[v], low[u]);
            if ((v != root) && (dfn[v] <= low[u])) cut[v] = true;
        }
        else if (u != last) low[v] = min(low[v], dfn[u]);
    }
}
int cutcnt(){
    MEMSET(cut, dfn, low); rootcnt = dcnt = 0;
    for (int i = 1; i <= n; ++i)
        if (!dfn[i]){
            rootcnt = 0; root = i; dfs(root, -1);
        }
    int sum = 0;
    for (int i = 1; i <= n; ++i) sum += cut[i];
    return sum;
}
/* --------------------------------------- */
int dfn[N], low[N], dindex, stack[N], top, bcnt, belong[N], ecnt, col[N];
struct data{
    int x, y;
}ecut[N];
void tarjan(int v, int last){
    col[v] = 1; stack[top++] = v; dfn[v] = low[v] = ++dindex;
    bool flag = false;
    for (int i = p[v]; i != -1; i = e[i].next) {
        int u = e[i].v;
        if ((u == last) && (!flag)) {
            flag = true; continue;
        }
        if (col[u] == 0) {
            tarjan(u, v);
            low[v] = min(low[v], low[u]);
            if (low[u] > dfn[v]) {
                ecut[++ecnt].x = v; ecut[ecnt].y = u;
            }
        }
        else if (col[u] == 1) low[v] = min(low[v], dfn[u]);
    }
    col[v] = 2;
    if (dfn[v] == low[v]) {
        int x; bcnt++;
        do{
            x = stack[--top]; belong[x] = bcnt; s[bcnt] += dat[x];
        }while (x != v);
    }
}
bool solve(){
    MEMSET( belong, dfn, low, col, ecut );
    ecnt = dindex = bcnt = top = 0;
    int times = 0;
    for (int i = 1; i <= n; ++i)
        if (!dfn[i]) {
            tarjan(i, -1); ++times;
        }
    if ((times > 1) || (ecnt == 0)) return false; else return true;
}


const int maxc = 20000;

struct node {
	node *fail, *next[26]; //26 is not fixed 
	int iter; //Point to origin string
	int cnt; //Arrave time
	void * operator new(size_t);
} *root, *null, pool[maxc], *pooltop;

typedef node *pnode;
pnode Q[maxc], *Qb, *Qt; //Notice: maxc, not maxn

void *node::operator new(size_t){ memset(pooltop, 0, sizeof *pooltop); return pooltop++; }

void insert(char s[], int iter){ 
	node *p = root;
	for (int i; *s; p = p->next[i]){
		i = *s++ - 'a'; //edit me
		if (!p->next[i]) p->next[i] = new node;
	}
	p->iter = iter; 
}

void build_ac(){ 
	pnode p, q;
	Qb = Qt = Q; *Qt++ = root;
	for (int i = 0; i < 26; i++) //26 is not fixed
		null->next[i] = root;
       	root->fail = null;
	while (Qb != Qt){ 
		p = *Qb++;
		for (int i = 0; i < 26; i++){ //26 is not fixed
		       	if (p->next[i]){
				q = p->fail;
				while (!q->next[i]) q = q->fail; 
				p->next[i]->fail = q->next[i]; 
				*Qt++ = p->next[i];
			}
		}
		//if (p->fail->iter) p->iter = p->fail->iter; //Maybe useful 
	}
}

char x[155][100]; int cnt[155];
char s[10000000];
void query(char *s){ 
	pnode p = root, q;
	while (*s){
		int i = *s++ - 'a'; //edit me
		q = p;
		while (!p->next[i]) p = p->fail;
		p = p->next[i];
	       	++p->cnt;
	}
	for (pnode *p = Qt - 1; p >= Q; p--){
		q = *p; q->fail->cnt += q->cnt;
		if (q->iter) cnt[q->iter] = q->cnt;
       	}
}
int main(){ 
	int n;
	while (scanf("%d", &n), gets(s), n){
		pooltop = pool; null = new node; root = new node; 
		for (int i = 1; i <= n; i++){
			cnt[i] = 0; gets(x[i]); insert(x[i], i); 
		}
		build_ac();
		gets(s); query(s);
		int maxcnt = *max_element(cnt + 1, cnt + 1 + n); 
		for (int i = 1; i <= n; i++)
			for (int j = i + 1; j <= n; j++)
				if (strcmp(x[i], x[j]) == 0)
					cnt[i] = cnt[j] = max(cnt[i], cnt[j]); 
		printf("%d\n", maxcnt);
		for (int i = 1; i <= n; i++) if (maxcnt == cnt[i]) puts(x[i]);
	}
}

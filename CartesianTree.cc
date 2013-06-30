struct Node {
	int /*TREE*/ key, /*HEAP*/ val, ind;
	Node *father, *left, *right;
} node[maxn], *null;

struct lessKey {
	bool operator()(const Node *x, const Node *y) const {
		return x->key < y->key;
	}
};

Node *build(Node node[], int n) {
	static Node *ptr[maxn];
	Node *r, *last, *cur;
	rep(i, n) ptr[i] = node + i;
	sort(ptr, ptr + n, lessKey());
	r = last = ptr[0];
	r->father = r->left = r->right = null;
	for (int i = 1; i < n; ++i) {
		cur = ptr[i];
		cur->father = cur->left = cur->right = null;
		if (cur->value < r->value) {
			cur->left = r; r->father = cur;
			last = r = cur;
		} else {
			while (cur->value <= last->value) last = last->father;
			cur->left = last->right; last->right->father = cur;
			last->right = cur; cur->father = last;
			last = cur;
		}
	}	       
	return r;
}

void poj2201 {
	null = new Node; null->ind = 0;
	for (int n; cin >> n; ) {
		rep(i, n) {
			scanf("%d%d", &node[i].key, &node[i].value);
			node[i].ind = i + 1;
		}
		build(node, n); puts("YES");
		rep(i, n) {
			Node *p = node + i;
			printf("%d %d %d\n", p->father->ind, p->left->ind, p->right->ind); 
		}
	}
}

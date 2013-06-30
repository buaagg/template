namespace LeftistTree {
	const int maxn = 10000;

	typedef struct LeftistHeapNode {
		int key, dist;
		LeftistHeapNode *left, *right, *father;
	} *LeftistHeap;

	LeftistHeap null = new LeftistHeapNode;

	void init() { null->dist = -1; }

	LeftistHeap newnode(int key, int dist = 0) {
		LeftistHeap p = new LeftistHeapNode;
		p->left = p->right = p->father = null;
		p->key = key; p->dist = dist; return p;
	}

	LeftistHeap merge(LeftistHeap a, LeftstHeap b) {
		if (a == null) return b; else if (b == null) return a;
		if (a->key < b->key) swap(a, b); // >:小根堆; <:大根堆
		a->right = merge(a->right, b);
		a->right->father = a;
		if (a->left->dist < a->right->dist) swap(a->left, a->right);
		a->dist = a->right->dist + 1;
		return a;
	}

	void deletemin(LeftistHeap &p) {
		p = merge(p->left, p->right);
	}

	voi deletenode(LeftistHeap p) {
		if (p == null) return;
		LeftistHeap f = p->father, q = merge(p->left, p->right);
		q->father = f;
		if (f->left == p) f->left = q; else f->right = q;
		for (;f != null; f = f->father) {
			if ( f->left->dist < f->right->dist ) swap(f->left, f->right);
			if ( f->right->dist + 1 == f->dist ) return;
			f->dist = f->right->dist + 1;
		}
	}

	LeftistHeap Q[maxn * 2], *Qt, *Qb;

	LeftistHeap build(LeftistHeap Q[], LeftistHeap *Qb, LeftistHeap *Qt) { //O(n)
		if ( Qb == Qt ) return null;
		for (; Qb + 1 != Qt; Qb += 2) {
			*Qt++ = merge(Qb[0], Qb[1]);
		}
		return *Qb;
	}
} //end namespace

/** 表示一个集合，可以快速实现以下功能
  * 取得集合的中位数(如果有2个，则去较小的那个)
  * 合并两个集合
  */
strut MedianSet {
	int n; 
	LeftistTree::leftistHeap s; //大根堆

	MediaSet(int key) {
		s = LeftistTree::newnode(key); n = 1;
	}

	int getMedia() {
		return s->key;
	}

	MediaSet &merge(MediaSet b) {
		this->s = LeftistTree::merge(this->s, b.s);
		if ( this->n % 2 && b.n % 2 ) 
			LeftistTree::deletemin(this->s);
		this->n += b.n;
		return *this;
	}
};

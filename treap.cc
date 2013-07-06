struct node{
	int key,weight;
	node *l,*r;
	node(){}
	node(int k,int w):key(k),weight(w){ l=r=NULL; }
};
node *merge(node *p,node *q){
	 if (!p)return q; else if (!q)return p;
	 if (p->weight<q->weight){
		 p->r=merge(p->r,q); return p;
	 }else{
		 q->l=merge(p,q->l); return q;
	 }
}
void split(node *p,int k,node *&l,node *&r){
	if (!p)l=r=NULL;
	else if (p->key<k){
		l=p; split(p->r,k,p->r,r);
	}else{
		r=p; split(p->l,k,l,p->l);
	}
}
node *insert(node *p,node *q){
	if (!p)return q;
	if (p->weight<q->weight){
		if (p->key<q->key)p->r=insert(p->r,q); else p->l=insert(p->l,q);
		return p;
	}else{ 
		split(p,q->key,q->l,q->r); return q;
	}
}

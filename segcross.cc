int segcross(Point a, Point b, Point c, Point d, Point &p)  {
    double s1, s2, s3,s4; int d1, d2, d3, d4;
    d1=dblcmp(s1=xmult(a,b,c)); d2=dblcmp(s2=xmult(a,b,d));
    d3=dblcmp(s3=xmult(c,d,a)); d4=dblcmp(s4=xmult(c,d,b));

    if ((d1^d2)==-2 && (d3^d4)==-2) {
        p.x=(c.x*s2-d.x*s1)/(s2-s1);
        p.y=(c.y*s2-d.y*s1)/(s2-s1);
        return 1;    
    }
    if (d1==0 && dblcmp(dmult(c,a,b))<=0 ||
        d2==0 && dblcmp(dmult(d,a,b))<=0 ||
        d3==0 && dblcmp(dmult(a,c,d))<=0 ||
        d4==0 && dblcmp(dmult(b,c,d))<=0) {
        return 2;    
    }
    return 0;
}

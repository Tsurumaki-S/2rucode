#ifndef __ARRAY__
#define __ARRAY__

template<typename T>T*AllocArray(int u)
{
	//return (T *)malloc((u)*sizeof(T)) ;
	T *a ;
	a = new T[u] ;
	return a ;
}
#define ALLOC_ARRAY(T,U) AllocArray<T>(U)

//step_num    = (int *)malloc((count_change+1)*sizeof(int)) ;

/* http://www001.upp.so-net.ne.jp/isaku/tips/matrix.c.html */
/* dynamic generation of two-dimensional arrays */
template<typename T>T**AllocMatrix(int u,int v)
{
    int i; T**a,*b;
    try { a=(T**)new char[(sizeof*a+sizeof*b*v)*u]; }
    catch (...) { a=0; }
    if (a) b=(T*)(a+u); else return 0;
    for (i=0;i<u;i++,b+=v) a[i]=b;
    return a;
}
#define ALLOC_MATRIX(T,U,V) AllocMatrix<T>(U,V)


/* http://www001.upp.so-net.ne.jp/isaku/tips/cubic.c.html */
/* dynamic generation of three-dimensional arrays */
template<typename T>T***AllocCubic(int u,int v,int w)
{
    int i,j; T***a,**b,*c;
    try { a=(T***)new char[(sizeof*b*(1+v)+sizeof*c*w*v)*u]; }
    catch (...) { a=0; }
    if (a) { b=(T**)(a+u); c=(T*)(b+u*v); } else return 0;
    for (i=0;i<u;i++,b+=v) for (a[i]=b,j=0;j<v;j++,c+=w) b[j]=c;
    return a;
}
#define ALLOC_CUBIC(T,U,V,W) AllocCubic<T>(U,V,W)

#define FREE(X) delete[]X

#endif

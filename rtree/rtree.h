#ifndef _RTREE_
#define _RTREE_ 1

#ifndef __RTREE_PRIVATE_H__
	typedef void * rtree_t;
	typedef void * rect_t;
	typedef void * node_t;
#endif

typedef int (*SearchHitCallback)(rect_t r, void *data, void *arg);

extern rtree_t RTreeNew (void);
extern void RTreeInsert (rtree_t *, rect_t, void *);
extern int RTreeSearch (rtree_t, rect_t, SearchHitCallback, void *);
extern void RTreeDestroy (rtree_t);
extern void RTreePrint(node_t);
extern rect_t RectInit (void);
extern rect_t RectInitCoords (double *);

#endif /* _RTREE_ */

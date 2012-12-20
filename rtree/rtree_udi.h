#ifndef _RTREE_UDI_
#define _RTREE_UDI_

#include <YapInterface.h>
#include <udi.h>
#include "utarray.h"
#include "rtree.h"

#define SPEC "rtree"
/*Prolog term from :- udi(a(-,rtree,-)).*/

typedef struct Control
{
  int arg;
  rtree_t tree;
} control_t;

UT_icd rtree_cb_icd = {sizeof(control_t), NULL, NULL, NULL};

extern void *RtreeUdiInit (YAP_Term spec, int arity);

extern void *RtreeUdiInsert (YAP_Term term, void *control, void *clausule);

extern int RtreeUdiSearch (void *control, Yap_UdiCallback callback, void *args);

extern int RtreeUdiDestroy(void *control);

#endif /* _RTREE_UDI_ */

#ifndef _RTREE_UDI_
#define _RTREE_UDI_

#include "udi.h"

#define NARGS 5
struct Control
{
  int arg;
  void *pred;
  rtree_t tree;
};
typedef struct Control control_t[NARGS];

/*Prolog term from :- udi(a(-,rtree,-)).
  User defined index announce
*/
extern void *RtreeUdiInit (YAP_Term spec, void *pred, int arity);

/*this is called in each asserted term that was declared to udi_init*/
extern void *RtreeUdiInsert (YAP_Term term,void *control, void *clausule);

extern int RtreeUdiSearch (void *control,Yap_UdiCallback callback,void *args);

extern int RtreeUdiDestroy(void *control);

#endif /* _RTREE_UDI_ */

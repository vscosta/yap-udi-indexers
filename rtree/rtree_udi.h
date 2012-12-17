#ifndef _RTREE_UDI_
#define _RTREE_UDI_

#define NARGS 5
struct Control
{
  int arg;
  void *pred;
  rtree_t tree;
};
typedef struct Control control_t[NARGS];

struct CallbackM
{
  clause_list_t cl;
  void * pred;
};
typedef struct CallbackM * callback_m_t;

/*Prolog term from :- udi(a(-,+,+)).
  User defined index announce
*/
extern control_t *RtreeUdiInit (YAP_Term spec,
                                void *pred,
                                int arity);

/*this is called in each asserted term that was declared to udi_init*/
extern control_t *RtreeUdiInsert (YAP_Term term, /*asserted term*/
                                  control_t *control,
                                  void *clausule); /*to store in tree and return
                                                     in search*/

extern void *RtreeUdiSearch (control_t *control);
extern int RtreeUdiDestroy(control_t *control);

#endif /* _RTREE_UDI_ */

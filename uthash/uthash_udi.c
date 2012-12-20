#include <stdlib.h>
#include <assert.h>

#include "uthash_udi.h"
#include "uthash_udi_private.h"

static struct udi_control_block UTHashCB;

void udi_rtree_init(void) {
	UdiControlBlock cb = &UTHashCB;

	memset((void *) cb,0, sizeof(*cb));

	/*TODO: ask vitor why this gives a warning*/
	cb->decl=Yap_LookupAtom(SPEC);

	cb->init=UTHashUdiInit;
	cb->insert=UTHashUdiInsert;
	cb->search=UTHashUdiSearch;
	cb->destroy=UTHashUdiDestroy;

	Yap_UdiRegister(cb);
}

void *UTHashUdiInit (YAP_Term spec, int arg, int arity) {
	return NULL; /*empty uthash*/
}

void *UTHashUdiInsert (void *control,
		YAP_Term term, int arg, void *data)
{
  uthash_t hash = (uthash_t) control;
  YAP_Term argterm;
  uthash_t element;

  argterm = ArgOfTerm(arg,term);

  if (YAP_IsAtomTerm(arg))
    {
      element = (uthash_t) malloc(sizeof(*element));
      element->atom = YAP_AtomOfTerm(argterm);
      element->data = data;
      HASH_ADD_ATOM(hash, element);
      /* HASH_ADD(hh,hashtable,atom,sizeof(Atom),element); */
    }

  return (void *) hash;
}

/*ARGS ARE AVAILABLE*/
int UTHashUdiSearch (void *control,
		int arg, Yap_UdiCallback callback, void *args)
{
  YAP_Term argterm;
  YAP_Atom atom;
  uthash_t element;
  uthash_t hash = (uthash_t) control;
  int count = 0;

  assert(hash);

  argterm = YAP_A(arg); /*Deref(XREGS[arg]); */

  if (YAP_IsAtomTerm(argterm))
    {
      atom = YAP_AtomOfTerm(arg);


      HASH_FIND_ATOM(hash,&atom,element);
      /* HASH_FIND(hh,utcontrol->tree,&atom,sizeof(Atom),element); */
      while (element)
        {
          callback((void *) element->atom, element->data, args);
          count ++;
          HASH_FIND_NEXT_ATOM(element,&atom);
        }
      return (count);
    }
  return -1; /*YAP FALLBACK*/
}

int UTHashUdiDestroy(void *control)
{
  uthash_t hash = (uthash_t) control;

  assert(hash);

  if (hash)
    HASH_CLEAR(hh,hash); /* TODO: check if this is enough */

  return TRUE;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "rtree_udi.h"

void udi_rtree_init(void) {
	UdiControlBlock cb;

	/*TODO: ask vitor why this gives a warning*/
	cb = (UdiControlBlock) Yap_AllocCodeSpace(sizeof(*cb));
	if (!cb) {
		fprintf(stderr,"Failled to register rtree udi indexing \n");
	}
	memset((void *) cb,0, sizeof(*cb));

	/*TODO: ask vitor why this gives a warning*/
	cb->decl=Yap_LookupAtom(SPEC);
	//fprintf(stderr,"decl:%p\n",cb->decl);

	cb->init=RtreeUdiInit;
	cb->insert=RtreeUdiInsert;
	cb->search=RtreeUdiSearch;
	cb->destroy=RtreeUdiDestroy;

	Yap_UdiRegister(cb);
}

static int YAP_IsNumberTermToFloat (YAP_Term term, YAP_Float *n)
{
  if (YAP_IsIntTerm (term) != FALSE)
  {
    if (n != NULL)
      *n = (YAP_Float) YAP_IntOfTerm (term);
    return (TRUE);
  }
  if (YAP_IsFloatTerm (term) != FALSE)
  {
    if (n != NULL)
      *n = YAP_FloatOfTerm (term);
    return (TRUE);
  }
  return (FALSE);
}

static rect_t RectOfTerm (YAP_Term term)
{
  YAP_Term tmp;
  rect_t rect;
  int i;
  
  if (!YAP_IsPairTerm(term))
    return (RectInit());
  
  for (i = 0; YAP_IsPairTerm(term) && i < 4; i++)
    {
      tmp = YAP_HeadOfTerm (term);
      if (!YAP_IsNumberTermToFloat(tmp,&(rect.coords[i])))
        return (RectInit());
      term = YAP_TailOfTerm (term);
    }

  return (rect);
}

void *
RtreeUdiInit (YAP_Term spec, int arity) {
  UT_array *control;
  control_t n;
  YAP_Term arg;
  int i;

  utarray_new(control,&rtree_cb_icd);

  for (i = 1; i <= arity; i ++)
  {
	  arg = YAP_ArgOfTerm(i,spec);
	  if (YAP_IsAtomTerm(arg)
			  && strcmp(SPEC,YAP_AtomName(YAP_AtomOfTerm(arg))) == 0)
		  //TODO: change this strcmp
	  {
		  n.arg = i;
		  n.tree = RTreeNew();
		  utarray_push_back(control,&n);
	  }
  }
  return (void *) control;
}

void *
RtreeUdiInsert (YAP_Term term,void *c,void *clausule)
{
  UT_array *control;
  control_t *rtc;
  rect_t r;

  control = (UT_array *) c;
  assert(c);

  rtc=NULL;
  while ((rtc=(control_t *) utarray_next(control, rtc)))
    {
      r = RectOfTerm(YAP_ArgOfTerm(rtc->arg,term));
      RTreeInsert(&(rtc->tree),r,clausule);
      //RectPrint(r);printf("\n");
      //RTreePrint((*control)[i].tree);
    }

  return (void *) control;
}

/*ARGS ARE AVAILABLE*/
int RtreeUdiSearch (void *c, Yap_UdiCallback callback, void *args)
{
  int i;
  UT_array *control;
  control_t *rtc;
  YAP_Term Constraints;
  rect_t r;

  control = (UT_array *) c;
  assert(c);

  /*Rtree Only handles one at a time*/
  rtc=NULL;
  while ((rtc=(control_t *) utarray_next(control, rtc)))
  {
	YAP_Term t = YAP_A(rtc->arg);
    if (YAP_IsAttVar(t))
      {
        /*get the constraits rect*/
        Constraints = YAP_AttsOfVar(t);
//        Yap_DebugPlWrite(Constraints);
        if (YAP_IsApplTerm(Constraints))
        	r = RectOfTerm(YAP_ArgOfTerm(2,Constraints));

        return RTreeSearch(rtc->tree, r, callback, args);
      }
  }
  return -1; /*YAP FALLBACK*/
}

int RtreeUdiDestroy(void *c)
{
  int i;
  UT_array *control;
  control_t *rtc;

  control = (UT_array *) c;
  assert(c);

  rtc=NULL;
  while ((rtc=(control_t *) utarray_next(control, rtc)))
    {
      if (rtc->tree)
        RTreeDestroy(rtc->tree);
    }

  utarray_free(control);

  return TRUE;
}

/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#include <plt/debug.h>
#include <plt/rbtreedict.hpp>


//////////////////////////////////////////////////////////////////////

PltRbTreeNode_base
PltRbTreeNode_base::nil_obj;

PltRbTreeNode_base *
PltRbTreeNode_base::nil = &PltRbTreeNode_base::nil_obj;


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

PltRbTreeDictionary_base::PltRbTreeDictionary_base()
{
    head = new PltRbTreeNode_base;
    if (!head) {
        return;
    }
}

//////////////////////////////////////////////////////////////////////

PltRbTreeDictionary_base::~PltRbTreeDictionary_base()
{
   // TODO
}


//////////////////////////////////////////////////////////////////////

bool
PltRbTreeDictionary_base::addNode(PltRbTreeNode_base *v)
{
    PLT_PRECONDITION(v);
    PltRbTreeNode_base *x  = head;
    PltRbTreeNode_base *p  = head;
    PltRbTreeNode_base *g  = head;
    PltRbTreeNode_base *gg = head;

    // go down tree balancing nodes
    while (x != PltRbTreeNode_base::nil) {
        if ( *x == *v ) {
            return false; // no duplicates allowed
        }
        gg = g; g=p; p=x;
        x = x->child(v);
        if (x->l->red && x->r->red) {
            x = splitNode(v, gg, g, p, x);
        }
    }
    x = v;
    p->setChild(x);
    x = splitNode(v, gg, g, p, x);

    return true;
}

//////////////////////////////////////////////////////////////////////

PltRbTreeNode_base * 
PltRbTreeDictionary_base::rotateNode(PltRbTreeNode_base *v, 
                                     PltRbTreeNode_base *y) 
{
    PLT_PRECONDITION(v && y);
    PltRbTreeNode_base *c = y->child(v);
    PltRbTreeNode_base *gc;
    if ( *c < *v ) {
        gc    = c->r;
        c->r  = gc->l;
        gc->l = c;
    } else {
        gc    = c->l;
        c->l  = gc->r;
        gc->r = c;
    }
    if ( *y < *v) {
        y->r = gc;
    } else {
        y->l = gc;
    }
    PLT_ASSERT( *(y->l) < *y && *y < *(y->r) );
    return gc;
}

//////////////////////////////////////////////////////////////////////

PltRbTreeNode_base * 
PltRbTreeDictionary_base::splitNode(
    PltRbTreeNode_base *v,
    PltRbTreeNode_base *gg,
    PltRbTreeNode_base *g,
    PltRbTreeNode_base *p,
    PltRbTreeNode_base *x)
{
    PLT_PRECONDITION(v && x);
    
    x->red = true;
    x->l->red = false;
    x->r->red = false;
    
    if (p && p->red) {
        g->red = true;
        if ( (*g < *v) != (*p < *v) ) {
            p = rotateNode(v,g);
        }
        x = rotateNode(v,gg);
        x->red = false;
    }
    head->r->red = false;

    return x;
}


//////////////////////////////////////////////////////////////////////
const PltRbTreeNode_base *
PltRbTreeDictionary_base::lookupNode(const PltRbTreeNode_base &n) const
{
    PltRbTreeNode_base *p = head;
    for (;
         p != PltRbTreeNode_base::nil && (*p != n);
         p = p->child(&n))
        {
        }
    return p;
}

//////////////////////////////////////////////////////////////////////
        
            

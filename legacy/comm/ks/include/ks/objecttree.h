/* -*-plt-c++-*- */

#ifndef KS_OBJECTTREE_INCLUDED
#define KS_OBJECTTREE_INCLUDED

/*
 * Copyright (c) 1996, 1997, 1998, 1999
 * Lehrstuhl fuer Prozessleittechnik, RWTH Aachen
 * D-52064 Aachen, Germany.
 * All rights reserved.
 *
 * This file is part of the ACPLT/KS Package which is licensed as open
 * source under the Artistic License; you can use, redistribute and/or
 * modify it under the terms of that license.
 *
 * You should have received a copy of the Artistic License along with
 * this Package; see the file ARTISTIC-LICENSE. If not, write to the
 * Copyright Holder.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
 * OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* Author: Markus Juergens <markusj@plt.rwth-aachen.de> */

//////////////////////////////////////////////////////////////////////

#include "plt/list.h"
#include "plt/hashtable.h"

#include "ks/props.h"

//////////////////////////////////////////////////////////////////////

class KsObjectTreeNode;
class KsObjectTreeLeaf;
class KsObjectTree;

//////////////////////////////////////////////////////////////////////

enum KsNodeType { KsNode, KsLeaf };

//////////////////////////////////////////////////////////////////////

class KsObjectTreeBase
{
public:
    virtual ~KsObjectTreeBase();
    virtual KsNodeType getNodeType() = 0;
    const KsString &name();
    virtual KsProjProps *getProjProps() = 0;
    virtual KsCurrProps *getCurrProps() = 0;

    KsObjectTreeNode *parent;
};

//////////////////////////////////////////////////////////////////////

class KsObjectTreeNode
: public KsObjectTreeBase
{
public:
    KsObjectTreeNode(KsObjectTreeNode *p=0);
    ~KsObjectTreeNode();
    KsNodeType getNodeType();
    KsProjProps *getProjProps();
    KsCurrProps *getCurrProps();


    bool hasChild();
    void addChild(KsObjectTreeBase *);
    KsObjectTreeBase *getChild(const KsString&);
    bool removeChild(KsObjectTreeBase *);  
    PltList<KsObjectTreeBase *> childs;

    KsDomainProjProps projProps;
    KsDomainCurrProps currProps;
};

//////////////////////////////////////////////////////////////////////

class KsObjectTreeLeaf
: public KsObjectTreeBase
{
public:
    KsObjectTreeLeaf(KsObjectTreeNode *p=0);
    KsNodeType getNodeType();
    KsProjProps *getProjProps();
    KsCurrProps *getCurrProps();

    KsVarProjProps projProps;
    KsVarCurrProps currProps;
};

//////////////////////////////////////////////////////////////////////
//
// class KsObjectTree
//
//   This class implements an object tree as described in part six
//   of the ACPLT/KS documentation.
//   For fast access there are implemented two data structures 
//   concurrently. First there is a tree which is mentioned
//   to be used for relative addressing. Second a hash table
//   provides a way to access absolute paths. The table holds 
//   for every data item(variable or domain) the full path
//   and a pointer to the data, which is stored in the nodes 
//   and leaves of the tree. 
//    
class KsObjectTree
{
public:
    KsObjectTree();
    ~KsObjectTree();
    
    // add an entry to the tree
    bool addNode(const KsString &path, KsObjectTreeBase *toAdd);
    // remove an entry and all his children from the tree
    bool removeNode(const KsString &path_and_name);
    KsObjectTreeNode *findAbsPath(const KsString &);
    KsObjectTreeBase *findAbsNode(const KsString &);

    // functions for navigating in the tree
    //
    // set current node to the root
    void resetCurrent();
    // checks whether current is a leaf
    bool isLeaf();    
    // go to child
    KsObjectTreeBase *goToChild(const KsString &which);
    // climb up a stage
    KsObjectTreeNode *up();    

    KsObjectTreeNode *root;
    PltHashTable<KsString,KsObjectTreeBase *> pathTable;

    KsObjectTreeBase *current;
};


//////////////////////////////////////////////////////////////////////
// Inline Implementation
//////////////////////////////////////////////////////////////////////

inline 
KsObjectTreeBase::~KsObjectTreeBase()
{
    if(parent) {
        parent->removeChild(this);
    }
}

//////////////////////////////////////////////////////////////////////

inline
const KsString &
KsObjectTreeBase::name() {
    return getProjProps()->identifier;
}

//////////////////////////////////////////////////////////////////////

inline
KsNodeType
KsObjectTreeNode::getNodeType() 
{
    return KsNode;
}



//////////////////////////////////////////////////////////////////////

inline
KsObjectTreeNode::KsObjectTreeNode(KsObjectTreeNode *p)
{
    parent = p;
}


//////////////////////////////////////////////////////////////////////

inline
KsProjProps *
KsObjectTreeNode::getProjProps()
{
    return &projProps;
}

//////////////////////////////////////////////////////////////////////

inline
KsCurrProps *
KsObjectTreeNode::getCurrProps()
{
    return &currProps;
}

//////////////////////////////////////////////////////////////////////

inline bool
KsObjectTreeNode::hasChild()
{
    return !childs.isEmpty();
}

//////////////////////////////////////////////////////////////////////

inline void
KsObjectTreeNode::addChild(KsObjectTreeBase *p)
{
    p->parent = this;
    childs.addLast(p);
}

//////////////////////////////////////////////////////////////////////

inline 
KsObjectTreeLeaf::KsObjectTreeLeaf(KsObjectTreeNode *p)
{
    parent = p;
}

//////////////////////////////////////////////////////////////////////

inline
KsNodeType
KsObjectTreeLeaf::getNodeType() 
{
    return KsLeaf;
}

//////////////////////////////////////////////////////////////////////

inline
KsProjProps *
KsObjectTreeLeaf::getProjProps()
{
    return &projProps;
}

//////////////////////////////////////////////////////////////////////

inline
KsCurrProps *
KsObjectTreeLeaf::getCurrProps()
{
    return &currProps;
}

//////////////////////////////////////////////////////////////////////

inline 
KsObjectTree::KsObjectTree()
: root(0), current(0)
{}

//////////////////////////////////////////////////////////////////////

inline 
KsObjectTree::~KsObjectTree()
{
    if(root) {
        delete root;
    }
}

#endif






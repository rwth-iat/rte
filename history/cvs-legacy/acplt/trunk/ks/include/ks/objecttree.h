/* -*-plt-c++-*- */

#ifndef KS_OBJECTTREE_INCLUDED
#define KS_OBJECTTREE_INCLUDED

/*
 * Copyright (c) 1996, 1997
 * Chair of Process Control Engineering,
 * Aachen University of Technology.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must print or display the above
 *    copyright notice either during startup or must have a means for
 *    the user to view the copyright notice.
 * 3. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the Chair of Process Control Engineering nor the
 *    name of the Aachen University of Technology may be used to endorse or
 *    promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE CHAIR OF PROCESS CONTROL
 * ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Author: Markus Juergens <markusj@plt.rwth-aachen.de> */

//////////////////////////////////////////////////////////////////////

#include <plt/list.h>
#include <plt/hashtable.h>

#include <ks/props.h>

//////////////////////////////////////////////////////////////////////

class KsObjectTreeNode;
class KsObjectTreeLeaf;
class KsObjectTree;

//////////////////////////////////////////////////////////////////////

enum NodeType { Node, Leaf };

//////////////////////////////////////////////////////////////////////

class KsObjectTreeBase
{
public:
    virtual ~KsObjectTreeBase();
    virtual NodeType getNodeType() = 0;
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
    KsObjectTreeNode(KsObjectTreeNode *p=NULL);
    ~KsObjectTreeNode();
    NodeType getNodeType();
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
    KsObjectTreeLeaf(KsObjectTreeNode *p=NULL);
    NodeType getNodeType();
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
NodeType
KsObjectTreeNode::getNodeType() 
{
    return Node;
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
NodeType
KsObjectTreeLeaf::getNodeType() 
{
    return Leaf;
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






/* -*-plt-c++-*- */

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

#include <ks/objecttree.h>

//////////////////////////////////////////////////////////////////////

// template class PltList<KsObjectTreeBase *>;
// template class PltListIterator<KsObjectTreeBase *>;
// template class PltHashTable<KsString, KsObjectTreeBase *>;
// template class PltHashIterator<KsString, KsObjectTreeBase *>;

//////////////////////////////////////////////////////////////////////

KsObjectTreeNode::~KsObjectTreeNode()
{
    PltListIterator<KsObjectTreeBase *> it(childs);
    while(it) {
        (*it)->parent = NULL;
        delete *it;
        ++it;
    }
}

//////////////////////////////////////////////////////////////////////

bool
KsObjectTreeNode::removeChild(KsObjectTreeBase *which)
{
    return childs.remove(which);
}

//////////////////////////////////////////////////////////////////////

KsObjectTreeBase *
KsObjectTreeNode::getChild(const KsString &name)
{
    PltListIterator<KsObjectTreeBase *> it(childs);
    KsObjectTreeBase *p = 0;
    
    while(it) {
        p = *it;
        if(p->name() == name) {
            return p;
        }
        ++it;
    }

    return NULL;
}


//////////////////////////////////////////////////////////////////////
// class KsObjectTree
//////////////////////////////////////////////////////////////////////

bool
KsObjectTree::addNode(const KsString &path,
                      KsObjectTreeBase *toAdd)
{
    KsObjectTreeBase *loc;

    // first check, wether the root must be created
    //
    if(!root && path == "" ) {
        // root should be a node
        //
        if( toAdd->getNodeType() != Node ) {
            return false;
        }
        // set root and current
        //
        current = root = (KsObjectTreeNode *)toAdd;
        root->parent = NULL;
        pathTable.add(root->name(), root);
        return true;
    }

    // now try to find the path
    //
    if( pathTable.query(path, loc) ) {
        // path found, check wether it is a node
        //
        if(loc->getNodeType() == Node) {
            // success, add new entry to tree
            //
            ((KsObjectTreeNode *)loc)->addChild(toAdd);
            // and to lookup table
            //
            KsString newPath(path);
            if( path != "/" ) newPath += "/";
            newPath += toAdd->name();
            pathTable.add(newPath, toAdd);
            return true;
        }
    }

    return false;
}

//////////////////////////////////////////////////////////////////////

// helper function
// checks, wether s1 is a praefix of s2
//
bool isSubString(const char *s1, const char *s2)
{
    while( *s1 && *(s1++) == *(s2++) ) /*nop*/;
    return !(*s1);
}
    
bool
KsObjectTree::removeNode(const KsString &path_and_name)
{
    KsObjectTreeBase *toRemove;
    
    if( pathTable.query(path_and_name, toRemove) ) {
        // entry found, remove it and his children 
        //
        PltHashIterator<KsString,KsObjectTreeBase*> it(pathTable);
        KsObjectTreeBase *dummy;
        while(it) {
            if( isSubString(path_and_name, it->a_key) ) {
                pathTable.remove(it->a_key, dummy);
            }
            ++it;
        }
        delete toRemove;
        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////

KsObjectTreeNode *
KsObjectTree::findAbsPath(const KsString &path)
{
    KsObjectTreeBase *temp;
    if(pathTable.query(path, temp) && 
       temp->getNodeType() == Node)
    {
        return (KsObjectTreeNode*)temp;
    }

    return NULL;
}

//////////////////////////////////////////////////////////////////////

KsObjectTreeBase *
KsObjectTree::findAbsNode(const KsString &path_and_name)
{
    KsObjectTreeBase *p;
    if( pathTable.query(path_and_name, p) ) {
        return p;
    }

    return NULL;
}

//////////////////////////////////////////////////////////////////////

void
KsObjectTree::resetCurrent()
{
    current = root;
}

//////////////////////////////////////////////////////////////////////

bool
KsObjectTree::isLeaf()
{
    PLT_PRECONDITION(current);

    return current->getNodeType() == Leaf;
}

//////////////////////////////////////////////////////////////////////

KsObjectTreeBase *
KsObjectTree::goToChild(const KsString &which)
{
    PLT_PRECONDITION(current);

    // check wether current is a node
    //
    if( isLeaf() ) {
        return NULL;
    }

    KsObjectTreeBase *child = ((KsObjectTreeNode *)current)->getChild(which);
    
    // change current only if this child exists
    //
    if( child ) {
        current = child;
    }

    return child;
}

//////////////////////////////////////////////////////////////////////

KsObjectTreeNode *
KsObjectTree::up()
{
    PLT_PRECONDITION(current);
    
    if( current->parent ) {
        current = current->parent;
        return (KsObjectTreeNode *)current;
    }

    return NULL;
}


    
    
    

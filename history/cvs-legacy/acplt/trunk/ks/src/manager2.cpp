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

#include <ks/manager2.h>

//////////////////////////////////////////////////////////////////////

KsManager::KsManager()
{
    initObjectTree();
}

//////////////////////////////////////////////////////////////////////

KsManager::~KsManager()
{
}

//////////////////////////////////////////////////////////////////////


#ifdef DEBUG

#include <iostream.h>

void 
KsManager::printTree() const
{
    printTreeRek(0, objectTree.root);
}

void
KsManager::printTreeRek(int indent, KsObjectTreeBase *p) const
{
    if( p->getNodeType() == Node ) {
        KsObjectTreeNode *node = (KsObjectTreeNode *)p;
        PltListIterator<KsObjectTreeBase*> it(node->childs);
        while(it) {
            for(int i = 0; i < indent; i++) {
                cout << "\t";
            }
            cout << (*it)->name() << endl;
            if( (*it)->getNodeType() == Node ) {
                printTreeRek(indent+1, *it);
            }
            ++it;
        }
    } 
    else {
        for(int i = 0; i < indent; i++) {
            cout << "\t";
        }
        cout << p->name() << endl;
    }
}

void 
KsManager::printTable() const
{
    PltHashIterator<KsString,KsObjectTreeBase*> it(objectTree.pathTable);
    while(it) {
        cout << it->a_key << endl;
        ++it;
    }
}

#endif
    

//////////////////////////////////////////////////////////////////////

void
KsManager::initObjectTree()
{
    KsTime now;
    gettimeofday(&now, 0);

    KsObjectTreeNode *root = new KsObjectTreeNode;
    root->projProps = KsDomainProjProps( "/", now, "root", KS_AC_READ );
    if( objectTree.addNode("", root) ) {
        cout << "root added" << endl;
    }

    KsObjectTreeNode *vendor = new KsObjectTreeNode;
    vendor->projProps = KsDomainProjProps( "vendor", now, "", KS_AC_READ );
    if( objectTree.addNode("/", vendor) ) {
        cout << "vendor added" << endl;
    }

    KsObjectTreeLeaf *server_name = new KsObjectTreeLeaf;
    server_name->projProps = KsVarProjProps("server_name", now, "", 
                                            KS_AC_READ, "", KS_VT_STRING );
    server_name->currProps = 
        KsVarCurrProps(KsValueHandle(new KsStringValue(KS_MANAGER_NAME), KsOsNew),
                       now, KS_ST_GOOD);
    objectTree.addNode("/vendor", server_name);

    KsObjectTreeLeaf *server_version = new KsObjectTreeLeaf;
    server_version->projProps = 
        KsVarProjProps("server_version", now, "", KS_AC_READ, "", KS_VT_STRING );
    server_version->currProps = 
        KsVarCurrProps(KsValueHandle(new KsStringValue(KS_MANAGER_VERSION), KsOsUnmanaged),
                       now, KS_ST_GOOD);
    objectTree.addNode("/vendor", server_version);

    KsObjectTreeLeaf *server_description = new KsObjectTreeLeaf;
    server_description->projProps = 
        KsVarProjProps( "server_description", now, "", KS_AC_READ, "", KS_VT_STRING );
    server_description->currProps = 
        KsVarCurrProps(KsValueHandle(new KsStringValue(KS_MANAGER_DESCRIPTION), KsOsNew), 
                       now, KS_ST_GOOD);
    objectTree.addNode("/vendor", server_description);

    KsObjectTreeNode *servers = new KsObjectTreeNode;
    servers->projProps = KsDomainProjProps( "servers", now, "", KS_AC_READ );
    objectTree.addNode("/", servers);
}

//////////////////////////////////////////////////////////////////////

bool 
KsManager::addServerDescription(const KsString &name,
                                u_long protocol_version,
                                const KsTime &expires_at,
                                u_long port)
{
    char buf[20];
    KsString path = "/servers";
    KsString path_and_name = path;
    path_and_name += "/";
    path_and_name += name;

    KsTime now;
    gettimeofday(&now, 0);

    // check, wether server already exists
    //
    KsObjectTreeNode *server = objectTree.findAbsPath(path_and_name);
    if( !server ) {
        // create new branch
        server = new KsObjectTreeNode;
        server->projProps = KsDomainProjProps(name, now, "", KS_AC_READ);
        objectTree.addNode(path, server);
    }

    // add version domain
    //
    sprintf(buf, "%u", protocol_version );
    KsString strProtocolVersion(buf);
    (path += "/") += name;

    KsObjectTreeNode *version = new KsObjectTreeNode;
    version->projProps = 
        KsDomainProjProps(strProtocolVersion, now, "", KS_AC_READ);
    objectTree.addNode(path, version);

    // add needed information to the version domain
    //
    (path += "/") += strProtocolVersion;
    KsObjectTreeLeaf *expires = new KsObjectTreeLeaf;
    expires->projProps = 
        KsVarProjProps("expires_at", now, "", KS_AC_READ,
                       "", KS_VT_TIME);
    expires->currProps = 
        KsVarCurrProps(KsValueHandle(new KsTimeValue(expires_at), KsOsNew),
                       now, KS_ST_GOOD);
    objectTree.addNode(path, expires);

    KsObjectTreeLeaf *portinfo = new KsObjectTreeLeaf;
    portinfo->projProps = 
        KsVarProjProps("port", now, "", KS_AC_READ,
                       "", KS_VT_INT );
    portinfo->currProps = 
        KsVarCurrProps(KsValueHandle(new KsIntValue(port), KsOsNew),
                       now, KS_ST_GOOD);
    objectTree.addNode(path, portinfo);

    return true;
}

bool
KsManager::removeServerDescription(const KsString &name,
                                   u_long protocol_version)
{
    char buf[20];
    KsString path = "/servers";
    
    KsString server_path = path;
    (server_path += "/") += name;
    
    sprintf(buf, "%u", protocol_version); 
    KsString version_path = server_path;
    (version_path += "/") += buf;

    if( !(objectTree.removeNode(version_path))) {
        // server does not exist
        //
        return false;
    }
    
    // remove server domain, if there are no more versions left
    //
    KsObjectTreeNode *server = objectTree.findAbsPath(server_path);
    if( !server->hasChild() ) {
        objectTree.removeNode(server_path);
    }

    return true;    
}

    
    

                                            
    



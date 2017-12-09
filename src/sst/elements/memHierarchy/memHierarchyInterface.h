// -*- mode: c++ -*-
// Copyright 2009-2017 Sandia Corporation. Under the terms
// of Contract DE-NA0003525 with Sandia Corporation, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2009-2017, Sandia Corporation
// All rights reserved.
//
// Portions are copyright of other developers:
// See the file CONTRIBUTORS.TXT in the top level directory
// the distribution for more information.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.
//

#ifndef COMPONENTS_MEMHIERARCHY_MEMORYINTERFACE
#define COMPONENTS_MEMHIERARCHY_MEMORYINTERFACE

#include <string>
#include <utility>
#include <map>

#include <sst/core/sst_types.h>
#include <sst/core/link.h>
#include <sst/core/interfaces/simpleMem.h>
#include <sst/core/elementinfo.h>
#include <sst/core/output.h>

#include "memEvent.h"
#include "customcmd/customCmdEvent.h"

namespace SST {

class Component;
class Event;

namespace MemHierarchy {

/** Class is used to interface a compute mode (CPU, GPU) to MemHierarchy */
class MemHierarchyInterface : public Interfaces::SimpleMem {

public:
/* Element Library Info */
    SST_ELI_REGISTER_SUBCOMPONENT(MemHierarchyInterface, "memHierarchy", "memInterface", SST_ELI_ELEMENT_VERSION(1,0,0),
            "Interface to memory hierarchy. Converts SimpleMem requests into MemEventBases.", "SST::Interfaces::SimpleMem")

/* Begin class definition */
    MemHierarchyInterface(SST::Component *comp, Params &params);
    
    /** Initialize the link to be used to connect with MemHierarchy */
    virtual bool initialize(const std::string &linkName, HandlerBase *handler = NULL);

    /** Link getter */
    virtual SST::Link* getLink(void) const { return link_; }

    virtual void sendInitData(Request *req);
    virtual void sendRequest(Request *req);
    virtual Request* recvResponse(void);

    void init(unsigned int phase);



private:

    /** Convert any incoming events to updated Requests, and fire handler */
    void handleIncoming(SST::Event *ev);
    
    /** Process MemEvents into updated Requests*/
    Interfaces::SimpleMem::Request* processIncoming(MemEvent *ev);

    /** Update Request with results of MemEvent */
    void updateRequest(Interfaces::SimpleMem::Request* req, MemEvent *me) const;
    
    /** Function used internally to create the memEvent that will be used by MemHierarchy */
    MemEventBase* createMemEvent(Interfaces::SimpleMem::Request* req) const;

    /** Function used internally to create the custom memEvent that will be used by MemHierarchy */
    CustomCmdEvent* createCustomMemEvent(Interfaces::SimpleMem::Request* req) const;

    Component*      owner_;
    HandlerBase*    recvHandler_;
    SST::Link*      link_;
    std::map<MemEvent::id_type, Interfaces::SimpleMem::Request*> requests_;
    Output output;
    Addr baseAddrMask_;
    std::string rqstr_;
};

}
}

#endif
#ifndef QPID_FRAMING_FRAMEVISITOR_H
#define QPID_FRAMING_FRAMEVISITOR_H

/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 */

#include "qpid/framing/MethodBodyDefaultVisitor.h"
#include "qpid/framing/AMQBody.h"
#include "qpid/framing/AMQMethodBody.h"
#include "qpid/framing/AMQFrame.h"
#include "qpid/framing/FrameHandler.h"

namespace qpid {
namespace framing {

class AMQHeaderBody;
class AMQContentBody;
class AMQHeartbeatBody;

/**
 * Visitor for all concrete frame body types, combines
 * AMQBodyConstVisitor and MethodBodyDefaultVisitor.
 * 
 * Derived classes may override visit methods to specify actions.
 * Derived classes must override defaultVisit(), which is called
 * for any non-overridden visit functions.
 *
 */
struct FrameDefaultVisitor : public AMQBodyConstVisitor, public MethodBodyDefaultVisitor
{
    void visit(const AMQHeaderBody&) { defaultVisit(); }
    void visit(const AMQContentBody&) { defaultVisit(); }
    void visit(const AMQHeartbeatBody&) { defaultVisit(); }
    void visit(const AMQMethodBody& method) { method.accept(static_cast<MethodBodyDefaultVisitor&>(*this)); }

    using AMQBodyConstVisitor::visit;
    using MethodBodyDefaultVisitor::visit;
};

/**
 * A FrameHandler that is implemented as a visitor.
 */
struct FrameVisitorHandler : public FrameHandler,
                             protected FrameDefaultVisitor
{
    void handle(AMQFrame& f) { f.getBody()->accept(*this); }
};


}} // namespace qpid::framing


#endif  /*!QPID_FRAMING_FRAMEVISITOR_H*/

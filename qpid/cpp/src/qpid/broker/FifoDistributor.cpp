/*
 *
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


#include "qpid/broker/Queue.h"
#include "qpid/broker/FifoDistributor.h"

using namespace qpid::broker;

FifoDistributor::FifoDistributor(Messages& container)
    : messages(container) {}

bool FifoDistributor::nextMessage( Consumer::shared_ptr& c, QueuedMessage& next )
{
    return messages.browse(c->position, next, !c->allowAcquired());
}

bool FifoDistributor::allocate(const std::string&, const QueuedMessage& )
{
    // The Fifo distributor does not enforce or record message allocation
    return true;
}

void FifoDistributor::query(qpid::types::Variant::Map&) const
{
    // nothing to see here....
}


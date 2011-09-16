#ifndef QPID_CLUSTER_LOCKEDMAP_H
#define QPID_CLUSTER_LOCKEDMAP_H

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

#include "qpid/sys/Mutex.h"
#include <map>

namespace qpid {
namespace cluster {

/**
 * A reader-writer locked thread safe map.
 */
template <class Key, class Value>
class LockedMap
{
  public:
    /** Get value associated with key, returns Value() if none. */
    Value get(const Key& key) const {
        sys::RWlock::ScopedRlock r(lock);
        typename Map::const_iterator i = map.find(key);
        return (i == map.end()) ? Value() : i->second;
    }

    /** Update value with the value for key.
     *@return true if key was found.
     */
    bool get(const Key& key, Value& value) const {
        sys::RWlock::ScopedRlock r(lock);
        typename Map::const_iterator i = map.find(key);
        if (i != map.end())  {
            value = i->second;
            return true;
        }
        return false;
    }

    /** Associate value with key, overwriting any previous value for key. */
    void put(const Key& key, const Value& value) {
        sys::RWlock::ScopedWlock w(lock);
        map[key] = value;
    }

    /** Associate value with key if there is not already a value associated with key.
     * Returns true if the value was added.
     */
    bool add(const Key& key, const Value& value) {
        sys::RWlock::ScopedWlock w(lock);
        return map.insert(std::make_pair(key, value)).second;
    }

    /** Erase the value associated with key if any. Return true if a value was erased. */
    bool erase(const Key& key) {
        sys::RWlock::ScopedWlock w(lock);
        return map.erase(key);
    }

  private:
    typedef std::map<Key, Value> Map;
    Map map;
    mutable sys::RWlock lock;
};
}} // namespace qpid::cluster

#endif  /*!QPID_CLUSTER_LOCKEDMAP_H*/

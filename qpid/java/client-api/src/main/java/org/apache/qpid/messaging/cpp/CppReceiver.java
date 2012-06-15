/* Licensed to the Apache Software Foundation (ASF) under one
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
 */
package org.apache.qpid.messaging.cpp;

import org.apache.qpid.messaging.Message;
import org.apache.qpid.messaging.MessagingException;
import org.apache.qpid.messaging.Receiver;
import org.apache.qpid.messaging.Session;

public class CppReceiver implements Receiver
{
    private CppSession _ssn;
    private org.apache.qpid.messaging.cpp.jni.Receiver _cppReceiver;

    public CppReceiver(CppSession ssn,
            org.apache.qpid.messaging.cpp.jni.Receiver cppReceiver)
    {
        _ssn = ssn;
        _cppReceiver = cppReceiver;
    }

    @Override
    public Message get(long timeout) throws MessagingException
    {
        org.apache.qpid.messaging.cpp.jni.Message m = _cppReceiver.get(CppDuration.getDuration(timeout));
        return new TextMessage(m.getContent());

    }

    @Override
    public Message fetch(long timeout) throws MessagingException
    {
        org.apache.qpid.messaging.cpp.jni.Message m = _cppReceiver.fetch(CppDuration.getDuration(timeout));
        return new TextMessage(m);
    }

    @Override
    public void setCapacity(int capacity) throws MessagingException
    {
        _cppReceiver.setCapacity(capacity);
    }

    @Override
    public int getCapacity() throws MessagingException
    {
        return _cppReceiver.getCapacity();
    }

    @Override
    public int getAvailable() throws MessagingException
    {
        return _cppReceiver.getAvailable();
    }

    @Override
    public int getUnsettled() throws MessagingException
    {
        return _cppReceiver.getUnsettled();
    }

    @Override
    public void close() throws MessagingException
    {
        try
        {
            _cppReceiver.close();
        }
        finally
        {
            _cppReceiver.delete();
        }
    }

    @Override
    public boolean isClosed()
    {
        return _cppReceiver.isClosed();
    }

    @Override
    public String getName()
    {
        return _cppReceiver.getName();
    }

    @Override
    public Session getSession() throws MessagingException
    {
        _ssn.checkError();
        return _ssn;
    }

}

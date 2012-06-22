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
package org.apache.qpid.test.utils;

import java.lang.management.ManagementFactory;
import java.lang.management.ThreadInfo;
import java.lang.management.ThreadMXBean;
import java.util.Set;

import org.apache.log4j.Logger;

import org.apache.qpid.server.Broker;

public class InternalBrokerHolder implements BrokerHolder
{
    private static final Logger LOGGER = Logger.getLogger(InternalBrokerHolder.class);

    private final Broker _broker;
    private final String _workingDirectory;

    private Set<Integer> _portsUsedByBroker;

    public InternalBrokerHolder(final Broker broker, String workingDirectory, Set<Integer> portsUsedByBroker)
    {
        if(broker == null)
        {
            throw new IllegalArgumentException("Broker must not be null");
        }

        _broker = broker;
        _workingDirectory = workingDirectory;
        _portsUsedByBroker = portsUsedByBroker;
    }

    @Override
    public String getWorkingDirectory()
    {
        return _workingDirectory;
    }

    public void shutdown()
    {
        LOGGER.info("Shutting down Broker instance");

        _broker.shutdown();

        waitUntilPortsAreFree();

        LOGGER.info("Broker instance shutdown");
    }

    @Override
    public void kill()
    {
        // Can't kill a internal broker as we would also kill ourselves as we share the same JVM.
        shutdown();
    }

    private void waitUntilPortsAreFree()
    {
        new PortHelper().waitUntilPortsAreFree(_portsUsedByBroker);
    }

    @Override
    public String dumpThreads()
    {
        ThreadMXBean threadMXBean = ManagementFactory.getThreadMXBean();
        ThreadInfo[] threadInfos = threadMXBean.dumpAllThreads(true, true);
        StringBuilder dump = new StringBuilder();
        dump.append(String.format("%n"));
        for (ThreadInfo threadInfo : threadInfos)
        {
            dump.append(threadInfo);
        }

        long[] deadLocks = threadMXBean.findDeadlockedThreads();
        if (deadLocks != null && deadLocks.length > 0)
        {
            ThreadInfo[] deadlockedThreads = threadMXBean.getThreadInfo(deadLocks);
            dump.append(String.format("%n"));
            dump.append("Deadlock is detected!");
            dump.append(String.format("%n"));
            for (ThreadInfo threadInfo : deadlockedThreads)
            {
                dump.append(threadInfo);
            }
        }
        return dump.toString();
    }

    @Override
    public String toString()
    {
        return "InternalBrokerHolder [_portsUsedByBroker=" + _portsUsedByBroker + "]";
    }

}

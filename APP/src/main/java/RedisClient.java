/**
 * Copyright (c) 2012 YCSB contributors. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You
 * may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License. See accompanying
 * LICENSE file.
 */

/**
 * Redis client binding for YCSB.
 *
 * All YCSB records are mapped to a Redis *hash field*.  For scanning
 * operations, all keys are saved (by an arbitrary hash) in a sorted set.
 */

package site.ycsb.db;
import java.util.*;

import redis.clients.jedis.Jedis;
import site.ycsb.ByteIterator;
import site.ycsb.DB;
import site.ycsb.DBException;
import site.ycsb.Status;
import site.ycsb.StringByteIterator;
import redis.clients.jedis.BasicCommands;
import redis.clients.jedis.HostAndPort;
import redis.clients.jedis.JedisCluster;
import redis.clients.jedis.JedisCommands;
import redis.clients.jedis.Protocol;
import java.io.Closeable;
import java.io.IOException;

/**
 * YCSB binding for <a href="http://redis.io/">Redis</a>.
 *
 * See {@code redis/README.md} for details.
 */

public class RedisClient extends DB {

    private static final String HOST_PROPERTY = "localhost";
    private static final String PORT_PROPERTY = "6379";
    private static final String PASSWORD_PROPERTY = "";
    private static final String CLUSTER_PROPERTY = "";
    private static Map<Integer, String> p2Configuration;
    private static Map<Integer, Jedis> connectionPool;
    private static Map<Integer, Integer> dataToP2;
    private static final String INDEX_KEY = "_indices";
    private P2Client client;

    public void init() throws DBException {
        client = new P2Client();
    }

    @Override
    public Status read(String table, String key, Set<String> fields,
                       Map<String, ByteIterator> result) {

        if (fields == null) {
            client.hgetAll(key, result);
        }
        else {
            client.hmget(key, fields, result);
        }
        return result.isEmpty() ? Status.ERROR : Status.OK;

    }


    @Override
    public Status insert(String table, String key,
                         Map<String, ByteIterator> values) {
//    if (jedis.hmset(key, StringByteIterator.getStringMap(values))
//        .equals("OK")) {
//      jedis.zadd(INDEX_KEY, hash(key), key);
//      return Status.OK;
//    }
//    return Status.ERROR;
        return Status.OK;
    }

    @Override
    public Status delete(String table, String key) {
//    return jedis.del(key) == 0 && jedis.zrem(INDEX_KEY, key) == 0 ? Status.ERROR
//        : Status.OK;
        return Status.OK;
    }


    @Override
    public Status update(String table, String key,
                         Map<String, ByteIterator> values) {
//    return jedis.hmset(key, StringByteIterator.getStringMap(values))
//        .equals("OK") ? Status.OK : Status.ERROR;
        return Status.OK;
    }

    @Override
    public Status scan(String table, String startkey, int recordcount,
                       Set<String> fields, Vector<HashMap<String, ByteIterator>> result) {
//    Set<String> keys = jedis.zrangeByScore(INDEX_KEY, hash(startkey),
//        Double.POSITIVE_INFINITY, 0, recordcount);
//
//    HashMap<String, ByteIterator> values;
//    for (String key : keys) {
//      values = new HashMap<String, ByteIterator>();
//      read(table, key, fields, values);
//      result.add(values);
//    }

        return Status.OK;
    }

}
//
//class JedisClient {
//
//    private Jedis jedis;
//    private boolean isAvailable;
//
//    public JedisClient(Jedis jedis, boolean isAvailable) {
//        this.jedis = jedis;
//        this.isAvailable = isAvailable;
//    }
//
//    public Jedis getJedis() {
//        return jedis;
//    }
//
//    public synchronized boolean isAvailable() {
//
//        return isAvailable;
//    }
//
//    public synchronized void setAvailable(boolean available) {
//        isAvailable = available;
//    }
//}

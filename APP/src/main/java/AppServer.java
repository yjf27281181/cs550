package site.ycsb.db;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedDeque;
import java.util.logging.Logger;


import redis.clients.jedis.Jedis;
import site.ycsb.ByteIterator;
import site.ycsb.Status;
import site.ycsb.StringByteIterator;

public class P2Client {

    private ConcurrentHashMap<Integer, String> p2Configuration;
    private ConcurrentHashMap<Integer, Deque<Jedis>> connectionPool;
    private ConcurrentHashMap<Integer, Integer> dataToP2;

    public P2Client() {
        connectionPool = new ConcurrentHashMap<Integer, Deque<Jedis>>() {{
            put(0, new ArrayDeque<>());
            put(1, new ArrayDeque<>());
            put(2, new ArrayDeque<>());
        }};
        dataToP2 = new ConcurrentHashMap<Integer, Integer>() {{
            put(0, 0);
            put(1, 1);
            put(2, 2);
        }};
        p2Configuration = new ConcurrentHashMap<Integer, String>() {{
            put(0, "localhost,6379");
            put(1, "localhost,6379");
            put(2, "localhost,6379");
        }};

    }

    public Deque<Jedis> getConnections(int index) {
        int p2Index = dataToP2.get(index);
        Deque<Jedis> cs =  connectionPool.get(p2Index);
        return cs;
    }

    public String redirect(String res) {
        char[] buffer = new char[1024];
        int length = 0;
        try {
            Socket socket = new Socket("192.168.0.200", 8081);
            OutputStreamWriter osw = new OutputStreamWriter(socket.getOutputStream(), StandardCharsets.UTF_8);
            InputStreamReader isr = new InputStreamReader(socket.getInputStream(), StandardCharsets.UTF_8);
            osw.write(res, 0, res.length());
            osw.flush();
            osw.close();

            length = isr.read(buffer, 0, 1024);

        } catch (IOException e) {
            e.printStackTrace();
        }
        return new String(buffer, 0, length);
    }

    public String get(String key) {
        int index = 0;
//        if (key == null || key.equals("") || key.equals(" ")) {
//        }
//        else {
//            index = key.hashCode() % 3;
//        }
        Deque<Jedis> connections = getConnections(index);
        Jedis connection;

        if (connections.isEmpty()) {
            String[] config = p2Configuration.get(index).split(",");
            connection = new Jedis(config[0], Integer.parseInt(config[1]));
        }
        else {
            connection = connections.getFirst();
        }
        String res = connection.get(key);
        if (res.startsWith("redirect")) {
            res = redirect(res);
        }
        connections.add(connection);
        return res;
    }

    public Map<String, String> hgetAll(String key, Map<String, ByteIterator> result) {
        int index = 0;
        Deque<Jedis> connections = getConnections(index);
        Jedis connection;

        if (connections.isEmpty()) {
            String[] config = p2Configuration.get(index).split(",");
            connection = new Jedis(config[0], Integer.parseInt(config[1]));
        }
        else {
            connection = connections.getFirst();
        }
        StringByteIterator.putAllAsByteIterators(result, connection.hgetAll(key));
        Map<String, String> res = connection.hgetAll(key);
//        if (res.startsWith("redirect")) {
//            res = redirect(res);
//        }
        connections.add(connection);
        return res;
    }

    public List<String> hmget(String key, Set<String> fields, Map<String, ByteIterator> result) {
        int index = 0;
        Deque<Jedis> connections = getConnections(index);
        Jedis connection;

        if (connections.isEmpty()) {
            String[] config = p2Configuration.get(index).split(",");
            connection = new Jedis(config[0], Integer.parseInt(config[1]));
        }
        else {
            connection = connections.getFirst();
        }
        String[] fieldArray
                = (String[]) fields.toArray(new String[fields.size()]);
        List<String> values = connection.hmget(key, fieldArray);
        Iterator<String> fieldIterator = fields.iterator();
        Iterator<String> valueIterator = values.iterator();

        while (fieldIterator.hasNext() && valueIterator.hasNext()) {
            result.put(fieldIterator.next(),
                    new StringByteIterator(valueIterator.next()));
        }
        assert !fieldIterator.hasNext() && !valueIterator.hasNext();
        List<String> res = connection.hmget(key, fieldArray);
//        if (res.startsWith("redirect")) {
//            res = redirect(res);
//        }
        connections.add(connection);
        return res;
    }

}

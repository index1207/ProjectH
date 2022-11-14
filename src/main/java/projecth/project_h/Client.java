package projecth.project_h;

import com.almasb.fxgl.app.scene.GameScene;
import javafx.scene.Scene;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;

class Receiver extends Thread {
    public Client c;
    String chat;
    private boolean isRecv;
    public Receiver(Client cl,String chat, boolean isRecv) {
        c = cl;
        this.chat = chat;
    }
    @Override
    public void run() {
        try {
            ByteBuffer buf = ByteBuffer.allocate(1024);
            while(true) {
                //이 루프가 돌아가는 동안 메세지를 받음.
                isRecv = false;
                chat = c.receive(buf);
                isRecv = true;
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

}

class Client {
    private SocketChannel sock;
    private Charset charset;
    public Client(String host, int port) throws IOException {
        charset = Charset.forName("UTF-8");

        sock = SocketChannel.open();
        sock.connect(new InetSocketAddress(host, port));
    }

    public String receive(ByteBuffer buffer) throws IOException {
        sock.read(buffer);
        byte[] rtnBytes = buffer.array();
        return new String(rtnBytes);
    }

    public void send(String msg) throws IOException {
        ByteBuffer buffer = charset.encode(msg);
        sock.write(buffer);
    }

    public void login(String id, String password) throws IOException {
        ByteBuffer buffer = charset.encode(String.format("{\"type\":\"login\", \"id\":\"%s\", \"pwd\":\"%s\"}", id, password));
        sock.write(buffer);
    }

    public void register(String newId, String password) throws IOException {
        ByteBuffer buffer = charset.encode(String.format("{\"type\":\"register\", \"id\":\"%s\", \"pwd\":\"%s\"}", newId, password));
        sock.write(buffer);
    }

    public void sendMessage(String message) throws IOException {
        ByteBuffer buffer = charset.encode(
                String.format("{\"type\":\"message\", \"id\":\"root\", \"message\":\"%s\"}", message)
        );
        sock.write(buffer);
    }
}
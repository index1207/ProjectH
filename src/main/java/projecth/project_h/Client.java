package projecth.project_h;

import com.almasb.fxgl.app.scene.GameScene;
import javafx.application.Platform;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.util.Stack;

//class Receiver extends Thread {
//    public Client c;
//    ImageView imv;
//    String chat;
//    Image im;
//    VBox layout;
//    int cnt;
//    public Receiver(Client c, ImageView imageView, VBox layout, int count)
//    {
//        this.c = c;
//        ImageView imv = imageView;
//        this.layout = layout;
//        cnt = count;
//    }
//
//        @Override
//        public void run () {
//        try {
//            ByteBuffer buf = ByteBuffer.allocate(1024);
//            while (true) {
//                //이 루프가 돌아가는 동안 메세지를 받음.
//                chat = c.receive(buf);
//                Label ch = new Label(chat);
//                ch.setFont(new Font("Dotum", 24));
//                ch.setTranslateY(-180);
//                ch.setTranslateX(810);
//                ch.setStyle("-fx-border-color: #ffffff;-fx-background-color: #ffffff;");
//                ch.setTextFill(Color.web("#050505"));
//                System.out.println(chat);
//                layout.getChildren().add(ch);
//                cnt++;
//            }
//        } catch (Exception e) {
//            throw new RuntimeException(e);
//        }
//
//    }
//}



class Client {
    private SocketChannel sock;
    private Charset charset;
    public Client(String host, int port) throws IOException {
        charset = Charset.forName("x-windows-949");

        sock = SocketChannel.open();
        sock.connect(new InetSocketAddress(host, port));
    }

    public String receive(ByteBuffer buffer) throws IOException {
        sock.read(buffer);
        byte[] rtnBytes = new String(buffer.array()).getBytes("euc-kr");
        return new String(rtnBytes).replaceAll("\0", "");
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
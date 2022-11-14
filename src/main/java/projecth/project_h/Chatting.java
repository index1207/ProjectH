package projecth.project_h;

import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;

public class Chatting extends Thread{

    ImageView imv;
    boolean tf;
    String chat;
    Image im;
    VBox layout;
    int cnt;
    Chatting(ImageView imageView, boolean tf,String chatting,Image image,VBox layout,int count) {
        imv = imageView;
        this.tf = tf;
        chat = chatting;
        im = image;
        this.layout = layout;
        cnt = count;
    }
    @Override
    public void run() {
        while(true) {
            if(tf) {
                imv.setImage(im);
                Label ch = new Label(chat);
                ch.setFont(new Font("Dotum", 24));
                ch.setTranslateY(-180);
                ch.setTranslateX(810);
                ch.setStyle("-fx-border-color: #ffffff;-fx-background-color: #ffffff;");
                ch.setTextFill(Color.web("#050505"));
                layout.getChildren().add(ch);
                cnt++;

            }
        }
    }
}


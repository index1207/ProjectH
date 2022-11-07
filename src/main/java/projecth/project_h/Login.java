package projecth.project_h;



import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.*;
import javafx.stage.Stage;

import java.io.FileInputStream;


public class Login extends Application  {

    private StackPane root = new StackPane();
    private Scene scene1,scene2,scene3,scene4;
    private Stage window;



    @Override
    public void start(Stage primaryStage) throws Exception{
        window = primaryStage;

        //로그인 페이지 버튼
        Label label1 = new Label("Username");
        Label label2 = new Label("Password");
        Button button1 = new Button("로그인");
        Button button2 = new Button("회원가입");

        //메인메뉴 페이지 버튼
        Button button3 = new Button("시작하기");
        Button button4 = new Button("게임방법");
        Button button5 = new Button("나가기");

        //게임방법 페이지 버튼
        Label label3 = new Label("이 게임은 채팅을 칠 수 있는 메타버스 게임입니다.\n" +
                                    "~이러한 기능 들을 사용할 수 있으며\n" +
                                    "~게 이용 가능 합니다."
        );
        Button button6 = new Button("돌아가기");

        //게임 페이지 버튼


        //button1 눌렀을때(Login) -> 메인메뉴로
        button1.setOnAction(e->window.setScene(scene2));

        //button2

        //button3 시작하기 -> ?
        button3.setOnAction(e->window.setScene(scene1));

        //button4 게임방법 -> 게임방법 페이지
        button4.setOnAction(e->window.setScene(scene3));
        button6.setOnAction(e->window.setScene(scene2));//돌아가기 -> 메인메뉴

        //button5 나가기 -> 시스템 종료
        button5.setOnAction(e->window.close());

        // layout1 = 로그인 페이지
        VBox layout1 = new VBox(10);
        layout1.setSpacing(8);
        layout1.setPadding(new Insets(10,10,10,10));
        layout1.getChildren().addAll(
                label1,
                new TextField(),
                label2,
                new PasswordField(),
                button1,
                button2);
        scene1 = new Scene(layout1, 400, 200);

        // layout2 = 메인메뉴 페이지
        VBox layout2 = new VBox(10);
        layout2.setPadding(new Insets(90,10,10,10));
        layout2.getChildren().addAll(
                button3,
                button4,
                button5
        );
        scene2 = new Scene(layout2, 500,300);

        //Background 이미지
        FileInputStream inp = new FileInputStream("src/main/resources/projecth/project_h/H.jpg");

        //이미지 제어
        Image im = new Image(inp);
        BackgroundImage bi = new BackgroundImage(im,
                BackgroundRepeat.NO_REPEAT,
                BackgroundRepeat.NO_REPEAT,
                BackgroundPosition.CENTER,
                BackgroundSize.DEFAULT
        );

        //Background 사진 출력
        Background bg = new Background(bi);
        layout2.setBackground(bg);

        //layout3 = 시작 화면
        VBox layout3 = new VBox(10);
        layout3.setSpacing(8);
        layout3.setPadding(new Insets(50,10,10,10));
        layout3.getChildren().addAll(
                label3,
                button6
        );
        scene4 = new Scene(layout3, 400, 200);


        //layout4 = 게임방법 페이지
        VBox layout4 = new VBox(10);
        layout4.setSpacing(8);
        layout4.setPadding(new Insets(50,10,10,10));
        layout4.getChildren().addAll(
                label3,
                button6
        );
        scene3 = new Scene(layout4, 400, 200);

        //시작
        window.setScene(scene1);
        window.setTitle("project H");
        window.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}
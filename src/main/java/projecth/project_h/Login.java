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
import java.time.temporal.TemporalQueries;


public class Login extends Application  {

    private StackPane root = new StackPane();
    private Scene scene1,scene2,scene3,scene4,scene5;
    private Stage window;

    String UserId,pw,UserId2,pw2;


    @Override
    public void start(Stage primaryStage) throws Exception{
        window = primaryStage;

        //로그인 페이지 버튼
        Label Username = new Label("Username");
        TextField user = new TextField();
        Label Password = new Label("Password");
        PasswordField password = new PasswordField();
        Button login = new Button("로그인");
        Button signUp = new Button("회원가입");

        //메인메뉴 페이지 버튼
        Button start = new Button("시작하기");
        Button rule = new Button("게임방법");
        Button exit = new Button("나가기");

        //게임방법 페이지 버튼
        Label rule2 = new Label("이 게임은 채팅을 칠 수 있는 메타버스 게임입니다.\n" +
                                    "~이러한 기능 들을 사용할 수 있으며\n" +
                                    "~게 이용 가능 합니다."
        );
        Button Return = new Button("돌아가기");

        //게임 페이지 버튼


        //회원가입 페이지 버튼
        Label Username2 = new Label("Username");
        TextField user2 = new TextField("Username");
        Label Password2 = new Label("Password");
        PasswordField password2 = new PasswordField();
        Button register = new Button("등록");

        //login 눌렀을때(Login) -> 메인메뉴로
        login.setOnAction(e->window.setScene(scene2));

        //signUp 회원가입 -> 회원가입 페이지
        signUp.setOnAction(e->window.setScene(scene5));

        //start 시작하기 -> ?
        start.setOnAction(e->window.setScene(scene1));

        //rule 게임방법 -> 게임방법 페이지
        rule.setOnAction(e->window.setScene(scene3));
        Return.setOnAction(e->window.setScene(scene2));//돌아가기 -> 메인메뉴

        //exit 나가기 -> 시스템 종료
        exit.setOnAction(e->window.close());

        //register 회원가입 -> 로그인
        register.setOnAction(e->window.setScene(scene1));

        // layout1 = 로그인 페이지
        VBox layout1 = new VBox(10);
        layout1.setSpacing(8);
        layout1.setPadding(new Insets(10,10,10,10));
        layout1.getChildren().addAll(
                Username,
                user,
                Password,
                password,
                login,
                signUp
        );
        user.setPromptText("Username");
        password.setPromptText("Password");
        UserId = user2.getText();
        pw = password2.getText();
        scene1 = new Scene(layout1, 400, 200);

        // layout2 = 메인메뉴 페이지
        VBox layout2 = new VBox(10);
        layout2.setPadding(new Insets(90,10,10,30));
        layout2.getChildren().addAll(
                start,
                rule,
                exit
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
                rule2,
                Return
        );
        scene4 = new Scene(layout3, 400, 200);


        //layout4 = 게임방법 페이지
        VBox layout4 = new VBox(10);
        layout4.setSpacing(8);
        layout4.setPadding(new Insets(50,10,10,10));
        layout4.getChildren().addAll(
                rule2,
                Return
        );
        scene3 = new Scene(layout4, 400, 200);

        //layout5 = 회원가입 페이지
        VBox layout5 = new VBox(10);
        layout5.setSpacing(8);
        layout5.setPadding(new Insets(10,10,10,10));
        layout5.getChildren().addAll(
                Username2,
                user2,
                Password2,
                password2,
                register
        );
        user2.setPromptText("Username");
        password2.setPromptText("Password");
        UserId2 = user2.getText();
        pw2 = password2.getText();
        scene5 = new Scene(layout5, 400, 200);

        //layout6 = 시작페이지
        VBox layout6 = new VBox(10);
        layout6.setSpacing(8);
        layout6.setPadding(new Insets(50,10,10,10));
        layout6.getChildren().addAll(

        );

        //시작
        window.setScene(scene1);
        window.setTitle("project H");
        window.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}
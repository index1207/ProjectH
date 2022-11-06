package projecth.project_h;



import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;


public class Login extends Application  {

    private StackPane root = new StackPane();
    private Scene scene1,scene2;
    private Stage window;



    @Override
    public void start(Stage primaryStage) throws Exception{
        window = primaryStage;


        Label label1 = new Label("Username");
        Label label2 = new Label("Password");
        Button button1 = new Button("로그인");
        Button button2 = new Button("회원가입");
        button1.setOnAction(e->window.setScene(scene2));

        // layout1

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

        // button 2

        Button button3 = new Button("돌아가기");
        button3.setOnAction(e->window.setScene(scene1));

        // layout 2

        StackPane layout2 = new StackPane();
        layout2.getChildren().add(button3);
        scene2 = new Scene(layout2, 500,300);

        window.setScene(scene1);
        window.setTitle("project H");
        window.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}
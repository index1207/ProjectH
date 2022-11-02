package projecth.project_h;

import javafx.fxml.FXML;
import javafx.scene.control.Label;

public class HelloController {
    @FXML
    private Label start;

    @FXML
    protected void onHelloButtonClick() {
        start.setText("테스트");
    }


}
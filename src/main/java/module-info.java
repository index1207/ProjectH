module projecth.project_h {
    requires javafx.controls;
    requires javafx.fxml;

    requires org.controlsfx.controls;
    requires com.almasb.fxgl.all;
    requires json.simple;

    opens projecth.project_h to javafx.fxml;
    exports projecth.project_h;
}
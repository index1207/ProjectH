package projecth.project_h;

import com.almasb.fxgl.app.GameApplication;
import com.almasb.fxgl.app.GameSettings;
import com.almasb.fxgl.dsl.FXGL;
import com.almasb.fxgl.entity.Entity;
import com.almasb.fxgl.input.Input;
import com.almasb.fxgl.input.UserAction;
import javafx.application.Application;
import javafx.scene.input.KeyCode;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Text;
import java.util.Map;


public class BasicGameApp extends GameApplication {
    private Entity player;
    private static String[] g_args;



    //플레이어 생성
    @Override
    protected void initGame() {
        player = FXGL.entityBuilder()
                .at(300, 300)
                .view(new Rectangle(25, 25, Color.BLUE))
                .buildAndAttach();
    }
    //WASD로 움직임
    protected void initInput() {
        FXGL.onKey(KeyCode.D, () -> {
            player.translateX(5); // move right 5 pixels
            FXGL.inc("pixelsMoved", +5);
        });

        FXGL.onKey(KeyCode.A, () -> {
            player.translateX(-5); // move left 5 pixels
            FXGL.inc("pixelsMoved", +5);
        });

        FXGL.onKey(KeyCode.W, () -> {
            player.translateY(-5); // move up 5 pixels
            FXGL.inc("pixelsMoved", +5);
        });

        FXGL.onKey(KeyCode.S, () -> {
            player.translateY(5); // move down 5 pixels
            FXGL.inc("pixelsMoved", +5);
        });
    }
    //다음 UI로 이동
    @Override
    protected void initUI() {
        Text textPixels = new Text();
        textPixels.setTranslateX(50); // x = 50
        textPixels.setTranslateY(100); // y = 100

        FXGL.getGameScene().addUINode(textPixels); // add to the scene graph
    }
    //모든 곳에서 사용이 가능한 변수
    @Override
    protected void initGameVars(Map<String, Object> vars) {
        vars.put("pixelsMoved", 0);
    }

    //처음 화면 크기 및 제목
    @Override
    protected void initSettings(GameSettings settings) {
        settings.setWidth(800);
        settings.setHeight(600);
        settings.setTitle("Basic Game App");
    }

}
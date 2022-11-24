package projecth.project_h;

import org.json.simple.*;
import org.json.simple.parser.JSONParser;

import java.text.ParseException;

public class Format{
    static String JsonParser(String chat){
        JSONParser parser = new JSONParser();
        System.out.println(chat);
        JSONObject obj = null;

        try {
            obj = (JSONObject)parser.parse(chat);
        } catch (Exception e) {
            System.out.println("변환에 실패");
            e.printStackTrace();
        }

        return obj.get("id") + " :" + obj.get("message");
    }
}
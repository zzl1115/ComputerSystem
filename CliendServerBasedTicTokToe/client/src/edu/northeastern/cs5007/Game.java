package edu.northeastern.cs5007;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClientBuilder;
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Random;


public class Game {

  public static void main(String[] args) {
    String uri = "http://localhost:8080/tic";
    Random rand = new Random();
    boolean playerFirst = true;
    if(rand.nextInt(2) == 0) {
      playerFirst = false;
    }
    TicTokToe ticTokToe = new TicTokToe();
    while(true) {
      if(playerFirst) {
        ticTokToe.printBoard();
        ticTokToe.play(ticTokToe.playerMove(), -1);
        if(ticTokToe.endGame()) {
          break;
        }
      } else {
        playerFirst = true;
      }
      ticTokToe.printBoard();
      //RestJson.send(ticTokToe, uri);
      String receive = RestJson.send(ticTokToe, uri);
      TicTokToe temp = new TicTokToe();
      temp = RestJson.jsonToBoard(receive);
      ticTokToe = temp;
      if(ticTokToe.endGame()) {
        break;
      }
    }
  }

  public static class RestJson {
    public static TicTokToe jsonToBoard(String str) {
      Gson gson = new GsonBuilder().create();
      return gson.fromJson(str, TicTokToe.class);
    }

    public static String send(TicTokToe ticTokToe, String uri) {
      Gson gson = new GsonBuilder().create();
      String msg = gson.toJson(ticTokToe);
      HttpClient httpClient = HttpClientBuilder.create().build(); //Use this instead
      String completePayload = "";
      try {

        HttpPost request = new HttpPost(uri);
        StringEntity params =new StringEntity(msg);
        request.addHeader("content-type", "application/json");
        request.setEntity(params);
        HttpResponse response = httpClient.execute(request);
        try {

          HttpEntity entity = response.getEntity();

          byte[] buffer = new byte[1024];

          if (entity != null) {
            InputStream inputStream = entity.getContent();
            try {
              int bytesRead = 0;
              BufferedInputStream bis = new BufferedInputStream(inputStream);
              while ((bytesRead = bis.read(buffer)) != -1) {
                String chunk = new String(buffer, 0, bytesRead);
                completePayload += chunk;
              }
            } catch (Exception e) {
              e.printStackTrace();
            } finally {
              try {
                inputStream.close();
              } catch (Exception ignore) {
              }
            }
          }

          // Now, try to turn that string into actual JSON
//        System.out.println("Entire paylaod: " + completePayload);
        } catch (Exception e) {
          e.printStackTrace();
        }
      }catch (Exception e) {
        e.printStackTrace();
      }

      return completePayload;
    }
  }

  public static class TicTokToe {
    private int[] chessPieces;

    public TicTokToe() {
      chessPieces = new int[9];
    }

    public int playerMove() {
      int result = -1;
      BufferedReader br = null;
      System.out.println("Please play a chess(0-8): ");
      try {
        br = new BufferedReader(new InputStreamReader(System.in));
        while (true) {
          String str = br.readLine();
          try {
            if (str.length() != 1 || !(Integer.parseInt(str) >= 0 && Integer.parseInt(str) < 9)) {
              System.out.println("Invalid Input");
              continue;
            }
          } catch (NumberFormatException e) {
            System.out.println("Invalid Input");
            continue;
          }
          result = Integer.parseInt(str);
          if (chessPieces[result] == 0) {
            break;
          }
        }
      } catch (IOException e) {
        e.printStackTrace();
      }
      return result;
    }

    public void play(int pos, int chess) {
      chessPieces[pos] = chess;
    }

    public void printBoard() {
      System.out.print("+---+---+---+");
      for (int i = 0; i < chessPieces.length; i++) {
        if (i % 3 == 0) {
          System.out.println();
          System.out.print("|");
        }
        System.out.print(" " + intToStr(chessPieces[i]) + " |");
      }
      System.out.println();
      System.out.println("+---+---+---+");
    }

    private String intToStr(int value) {
      switch(value) {
        case -1:
          return "O";
        case 0:
          return " ";
        case 1:
          return "X";
      }
      return " ";
    }

    public boolean endGame() {
      int[][] win = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
      for(int[] condition : win) {
        int chess = chessPieces[condition[0]];
        if(chess == 0) {continue;}
        for(int i = 0; i < 3; i++) {
          if(chessPieces[condition[i]] != chess) {
            break;
          }
          if(i == 2) {
            printBoard();
            String s = chess == -1? "Player Win" : "Com Win";
            System.out.println(s);
            return true;
          }
        }
      }

      int count = 0;
      for(int str : chessPieces) {
        if(str == 0) {
          count++;
        }
      }
      if(count == 0) {
        printBoard();
        System.out.println("Draw!");
        return true;
      }

      return false;
    }
  }
}

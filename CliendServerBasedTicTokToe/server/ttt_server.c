/**
 * test.c
 * Small Hello World! example
 * to compile with gcc, run the following command
 * gcc -o test test.c -lulfius
 */
#include <stdio.h>
#include <ulfius.h>
#include <unistd.h>


//Tic Tok Toe Strategy
int win(const int board[9]) {
    //determines if a player has won, returns 0 otherwise.
    unsigned wins[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    int i;
    for(i = 0; i < 8; ++i) {
        if(board[wins[i][0]] != 0 &&
           board[wins[i][0]] == board[wins[i][1]] &&
           board[wins[i][0]] == board[wins[i][2]])
            return board[wins[i][2]];
    }
    return 0;
}
int minimax(int board[9], int player) {
    //How is the position like for player (their turn) on board?
    int winner = win(board);
    if(winner != 0) return winner*player;

    int move = -1;
    int score = -2;//Losing moves are preferred to no move
    int i;
    for(i = 0; i < 9; ++i) {//For all moves,
        if(board[i] == 0) {//If legal,
            board[i] = player;//Try the move
            int thisScore = -minimax(board, player*-1);
            if(thisScore > score) {
                score = thisScore;
                move = i;
            }//Pick the one that's worst for the opponent
            board[i] = 0;//Reset board after try
        }
    }
    if(move == -1) return 0;
    return score;
}

void computerMove(int board[9]) {
    int move = -1;
    int score = -2;
    int i;
    for(i = 0; i < 9; ++i) {
        if(board[i] == 0) {
            board[i] = 1;
            int tempScore = -minimax(board, -1);
            board[i] = 0;
            if(tempScore > score) {
                score = tempScore;
                move = i;
            }
        }
    }
    //returns a score based on minimax tree at a given node.
    board[move] = 1;
}


#define PORT 8080
int customer_id = 0;
int shutdown_now = 0;

int callback_process_shutdown(const struct _u_request * request, struct _u_response * response, void * user_data) {

  shutdown_now = 1;

  return U_CALLBACK_CONTINUE;
}

int callback_process_order(const struct _u_request * request, struct _u_response * response, void * user_data) {
  json_t * json_order = ulfius_get_json_body_request(request, NULL);
  json_t * order_ack;
  json_t * json_body;
  json_t * current;
  
  if(!json_order) {
    printf("json_order is null, exiting\n");
  }
  
  order_ack = json_array();
  json_body = json_object();

  current = json_object_get(json_order, "chessPieces");
   
  int order_size = json_array_size(current);
  int board[9];
  for (int i = 0; i < order_size; i++) {
    board[i] = json_integer_value(json_array_get(current, i));
  }
  computerMove(board);
  for (int i = 0; i < order_size; i++) {
    json_array_append(order_ack, json_integer(board[i]));
  }
  
    json_object_set(json_body, "chessPieces", order_ack);
     
  ulfius_set_json_body_response(response, 200, json_body);

  return U_CALLBACK_CONTINUE;
}

/**
 * Callback function for the web application on /helloworld url call
 */
int callback_create_order(const struct _u_request * request, struct _u_response * response, void * user_data) {
  
  json_t * json_body = json_object();

  json_t * json_order = json_array();
  for (int i = 0; i < 9; i++) {
    json_array_append(json_order, json_integer(0));
  }
  json_t * json_sub_array = json_array();
  if (rand() %2 == 0) {
    json_sub_array = json_order;
  } else {
    
  int board[9];
  for (int i = 0; i < 9; i++) {
    board[i] = json_integer_value(json_array_get(json_order, i));
  }
  computerMove(board);
  for (int i = 0; i < 9; i++) {
    json_array_append(json_sub_array, json_integer(board[i]));
  }
  
  }
  
  // json_object_set(json_body, "result", json_string("Player turn, enter choice:"));
  
  json_object_set(json_body, "chessPieces", json_sub_array);

  ulfius_set_json_body_response(response, 200, json_body);

  return U_CALLBACK_CONTINUE;
}

/**
 * main function
 */
int main(void) {
  struct _u_instance instance;

  // Initialize instance with the port number
  if (ulfius_init_instance(&instance, PORT, NULL, NULL) != U_OK) {
    fprintf(stderr, "Error ulfius_init_instance, abort\n");
    return(1);
  }

  // Endpoint list declaration
  ulfius_add_endpoint_by_val(&instance, "GET", "/tic", NULL, 0, &callback_create_order, NULL);

  // Endpoint list declaration
  ulfius_add_endpoint_by_val(&instance, "POST", "/tic", NULL, 0, &callback_process_order, NULL);

 // Start the framework
  if (ulfius_start_framework(&instance) == U_OK) {
    printf("Start framework on port %d\n", instance.port);
    // Wait for the user to press <enter> on the console to quit the application
    getchar();

  } else {
    fprintf(stderr, "Error starting framework\n");
  }
  printf("End framework\n");

  ulfius_stop_framework(&instance);
  ulfius_clean_instance(&instance);

  return 0;
}

EvaluationAgent::EvaluationAgent(bool isAlphaBeta_in, char player_in){
	this->isAlphaBeta = isAlphaBeta_in;
	this->player = player_in;
	this->player_progress = player_in;
	if(player_in == PLAYER_1){
		this->opponent = PLAYER_2;
	} else {
		this->opponent = PLAYER_1;
	}
}

bool EvaluationAgent::playGame(Board* board){
	if(!isAlphaBeta){
		StateNode* best = minimax(board);
		board->play_piece(best->row, best->col, self->player);
	}
	return gameWon(board);
}

/**
  * If isMax, returns child node with max value
  * If !isMax, returns child node with min value
  */
void EvaluationAgent::minimax_help(int depth, bool isMax, StateNode* curr) {
	if(gameWon(curr->board)){
		if(isMax){
			curr->value = NEGATIVE_INFINITY;
		} else {
			curr->value = INFINITY;
		}
	} else if (depth == 0){
		curr->value = eval(curr->board);
		return;
	} else {
		for(int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++){
				if (this->board->unoccupied(i,j)){
					// changes board to test
					char currPlayer;
					if(isMax){
						currPlayer = this->player;
					} else {
						currPlayer = this->opponent; 
					} 
					curr->board->play_piece(i, j, currPlayer);
					StateNode* child = new StateNode(!isMax, curr->board)
					// Recursive call
					minimax_help(depth-1, !isMax, child);
					// Undoes move as to prevent confusion
					curr->board->unplay_piece(i, j, currPlayer);
					if(isMax){
						if(child->value > curr->value){
							curr->value = child->value;
							curr->row = i;
							curr->col = j;
						}
					} else {
						if(child->value < curr->value){
							curr->value = child->value;
							curr->row = i;
							curr->col = j;
						}
					}
					delete child;
				}
			}
		}
	}
}

StateNode* EvaluationAgent::minimax(Board* board){
	StateNode* head = new StateNode(true, board,-1, -1);
	return head;
}

/**
StateNode* EvaluationAgent::alphabeta_helper(int depth, bool isMax, StateNode* curr, int alpha, int beta){
	StateNode* extreme = NULL; //max if max layer, min if min layer
	if (depth == 0){
		curr->value = eval(curr->board);
		return curr;
	} else {
		for(int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++){
				if (this->board->unoccupied(i,j)){
					// changes board to test
					char currPlayer;
					if(isMax){
						currPlayer = this->player;
					} else {
						currPlayer = this->opponent; 
					} 
					curr->board->play_piece(i, j, currPlayer);
					StateNode* child = new StateNode(!isMax, curr->board, i, j)
					// Recursive call
					minimax_help(depth-1, !isMax, child);
					// Undoes move as to prevent confusion
					curr->board->unplay_piece(i, j, currPlayer);
					if(extreme == NULL) {
						extreme = child;
						if(isMax){
							alpha = child->value;
						} else {
							beta = child->value;
						}
					} else {
						if(isMax){
							if(child->value > extreme->value){
								delete extreme;
								extreme = child;
								if(beta <= alpha){
									return extreme;
								}
							} else {
								delete child;
							}
						} else {
							if(child->value < extreme->value){
								delete extreme;
								extreme = child;
								if(beta <= alpha){
									return extreme;
								}
							} else {
								delete child;
							}
						}
					}
				}
			}
		}
	}
}


StateNode* EvaluationAgent::alphabeta(Board* board) {
	StateNode* head = new StateNode(true, board,-1, -1);
	StateNode* child = minimax_help(2, true, head);
	delete head;
	return child;
}**/

int EvaluationAgent::eval(Board* board){
	int selfs[5]
	int opponents[5];
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			selfs[searchBoard(board,i,j,i+4,j,player)]++;
			selfs[searchBoard(board,i,j,i,j+4,player)]++;
			selfs[searchBoard(board,i,j,i+4,j+4,player)]++;
			selfs[searchBoard(board,i,j,i+4,j-4,player)]++;

			opponents[searchBoard(board,i,j,i+4,j,opponent)]++;
			opponents[searchBoard(board,i,j,i,j+4,opponent)]++;
			opponents[searchBoard(board,i,j,i+4,j+4,opponent)]++;
			opponents[searchBoard(board,i,j,i+4,j-4,opponent)]++;
		}
	}
	if(selfs[4] > 0){
		return INFINITY;
	}
	if(opponents[4] > 0){
		return NEGATIVE_INFINITY;
	}
	int sum = 0;
	for (int i = 1; i < 5; i++){
		sum += selfs[i]*i*i*i;
		sum -= opponents[i]*i*i*i;
	}
	return sum;
}

bool EvaluationAgent::gameWon(Board* board)
{
	bool won = false;
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			if(searchBoard(board, i, j, i+4,j, this->player_) == 5){
				return true;
			}
			if(searchBoard(board, i, j, i,j+4, this->player_) == 5){
				return true;
			}
			if(searchBoard(board, i, j, i+4,j+4, this->player_) == 5){
				return true;
			}
			if(searchBoard(board, i, j, i+4,j-4, this->player_) == 5){
				return true;
			}
		}
	}
}


int EvaluationAgent::searchBoard(Board* board, int startRow, int startCol, int endRow, int endCol, char player_in)
{

	if(endRow >= BOARD_SIZE){
		endRow = BOARD_SIZE - 1;
	}
	if(endCol >= BOARD_SIZE){
		endCol = BOARD_SIZE - 1;
	}
	int count = 0; 
	int dRow = 0;
	int dCol = 0;
	if(startRow < endRow){
		dRow = 1;
	}
	if(startRow > endRow){
		dRow = -1;
	}
	if(startCol < endCol){
		dCol = 1;
	}
	if(startCol > endCol){
		dCol = -1;
	}

	int dist = abs(startRow-endRow);
	if(abs(startCol-endCol) > dist){
		dist = abs(startCol-endCol);
	}

	int i = startRow;
	int j = startCol;
	for(int k=0; k <= dist; k++){
		if(((isupper(board->boardState(i,j)) && isupper(player_in))) || ((islower(board->boardState(i,j)) && islower(player_in)))){
			count += 1;
		}
		if(((isupper(board->boardState(i,j)) && islower(player_in))) || ((islower(board->boardState(i,j)) && isupper(player_in)))){
			return 0;
		}
		i += dRow;
		j += dCol;
	}
	return count;
}
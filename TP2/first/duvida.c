void empty_board(int **board, int R, int C){

	for (int i = 0; i < R; i++)
		for (int j = 0; j < C; j++)
			board[i][j] = 99;
}

void empty_board(int *board, int R, int C){

	for (int i = 0; i < R * C; i++)
		board[i] = 99;
}

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>

using namespace std;

// TODO:
//  1. Create game modes
//		a. Single Player w/ AI
//		b. Multiplayer
//	2. Fix bugs:
//		a. Fix CheckmateWhite()
//		b. Fix CheckmateBlack()
//	3. Fix the sort functions

void PrintBoard();
void PrintTitle();
void ResetBoard();
void MainMenu();
void SinglePlayer();
void Multiplayer();
void PlayerName();
void Game();
bool validMove(int originx, int originy, int x, int y, string color);
bool CheckmateWhite();
bool CheckmateBlack();
void Move(string color);
bool Stalemate(string color);
bool UnableToMove(string color);
bool NoWinnerPossible();
void SequentialMoves(char Pieces[], string color);
void SaveMove(char Piece, int originx, int originy, int destinationx, int destinationy, string color, bool capture, char captured);
bool isOccupied(int x, int y);
bool inCheck(string color);
bool inCheck(int originx, int originy, int destinationx, int destinationy, string color);
void SortPieces(char *Pieces, int size, string color);
void SortCaptured(char* Pieces, int size, string color);
void Capture(int x, int y, string color);
void swap(int* xp, int* yp);

char board[8][8];
char Alpha[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
char whitePieces[] = {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'};
char blackPieces[] = {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'};
char whitePiecesBoard[16];
char blackPiecesBoard[16];
char whiteCaptured[16];
char blackCaptured[16];
int originxSave;
int originySave;
int destinationxSave;
int destinationySave;
int WhiteKingMoves = 0;
int BlackKingMoves = 0;
int MoveNumber = 0;

struct Piece
{
	char character;
	int value;
};

#define RANK_SIZE 8
#define FILE_SIZE 8

struct Player {
	string name;
	string color;
};

Player Player1;
Player Player2;

int main()
{
	PrintTitle();
	ResetBoard();
	PrintBoard();

	MainMenu();

	return 0;
}

void MainMenu()
{
	bool main = false;
	string input;

	// Prompt the user if they would like to play against a human or computer
	while (!main)
	{
		system("cls");

		PrintTitle();
		PrintBoard();

		cout << endl;
		cout << "1. Single Player" << endl;
		cout << "2. Multiplayer" << endl;
		cout << endl;
		cout << "Your Selection: ";
		getline(cin, input);


		if (input.length() <= 1)
		{
			if (input[0] == '1')
			{
				main = true;
				SinglePlayer();
			}
			else if (input[0] == '2')
			{
				main = true;
				Multiplayer();
			}
			else
			{
				cout << "You have entered an invalid option..." << endl;
				cin.get();
			}
		}
		else
		{
			cout << "You have entered a string that is too long..." << endl;
		}
	}
}

void SinglePlayer()
{
	system("cls");
	PrintTitle();

	cout << endl;
	cout << "This feature has not been implemented yet!" << endl;
	cout << "Press any key to continue..." << endl;
	cin.get();

	return;
}

void Multiplayer()
{
	bool playerselection = false;
	string input;

	// Ask the player their name and if they want white or black or randomly chose between the two
	while (!playerselection)
	{
		system("cls");

		PrintTitle();
		ResetBoard();
		PrintBoard();

		cout << endl;
		cout << "Player 1 select an option:" << endl;
		cout << "1. White" << endl;
		cout << "2. Black" << endl;
		cout << "0. Random" << endl;
		cout << endl;
		cout << "Your selection: ";
		getline(cin, input);

		if (input.length() <= 1)
		{
			if (input[0] == '1')
			{
				// Player 1 has chosen to be white
				playerselection = true;
				Player1.color = "White";
				Player2.color = "Black";

				// Change each player's name
				//PlayerName();
				Game();
			}
			else if (input[0] == '2')
			{
				// Player 1 has chosen to be black
				playerselection = true;
				Player1.color = "Black";
				Player2.color = "White";

				// Change the player names
				//PlayerName();
				Game();
			}
			else
			{
				cout << "You have entered an invalid option..." << endl;
				cin.get();
			}
		}
		else
		{
			cout << "You have entered a string that is too long..." << endl;
		}
	}
}

// Game() will be the logic behind the game
void Game()
{
	// White moves first
	// The game should check after every player moves if the other player has been checkmated

	// Check if the game is a draw and if either player has been checkmated

	while (!CheckmateWhite() && !CheckmateBlack() && !Stalemate("White") && !Stalemate("Black"))
	{
		if (!CheckmateWhite() && !Stalemate("White") && !Stalemate("Black"))
		{
			Move("White");
		}
		if (!CheckmateBlack() && !Stalemate("White") && !Stalemate("Black"))
		{
			Move("Black");
		}
	}

	// Check if white has checkmated black
	if (CheckmateWhite())
	{
		cout << "Black Wins!" << endl;
		cin.get();
	}
	// Check if black has checkmate against white
	else if (CheckmateBlack())
	{
		cout << "White Wins!" << endl;
		cin.get();
	}

	// Check if the game is a draw "stalemate"
	if (Stalemate("White") || Stalemate("Black"))
	{
		cout << "The game has been stalemated, draw." << endl;
	}
}

// Logic for white's move
// This function needs to be fixed
// TODO: Fix the ranks and files confusion. The 
void Move(string color)
{
	string origin;
	string destination;
	char temp;
	char capture;
	int originx = 0;
	int originy = 0;
	int destinationx = 0;
	int destinationy = 0;
	bool originMove = false;
	bool destinationMove = false;

	while (!originMove)
	{
		system("cls");
		PrintTitle();
		PrintBoard();

		cout << endl;
		cout << "Enter the source piece" << endl;

		if (color == "White")
			cout << "White Source Piece: ";
		else
			cout << "Black Source Piece: " << endl;
		
		getline(cin, origin);

		// Test the input
		// Test if the first character is of letters A-H
		for (int y = 0; y <= FILE_SIZE; y++)
		{
			// Test if the second character is a number from 1-8
			for (int x = 1; x <= RANK_SIZE; x++)
			{
				if (toupper(origin[0]) == Alpha[y] && origin[1] == (x + 48))
				{
					// Convert the input to the board's x and y coordinates
					originx = x - 1;
					originy = y;

					// Check if the current chosen square is populated by a white piece
					for (int i = 0; i < 16; i++)
					{
						if (color == "White")
						{
							if (board[originx][originy] == whitePieces[i])
							{
								// Found a white piece at the source location!
								originMove = true;
								break;
							}
						}
						else
						{
							if (board[originx][originy] == blackPieces[i])
							{
								// Found a black piece at the source location!
								originMove = true;
								break;
							}
						}
					}
				}
			}
		}
	}
	
	while (!destinationMove)
	{
		system("cls");
		PrintTitle();
		PrintBoard();

		cout << endl;
		cout << "Enter the destination square" << endl;
		if (color == "White")
			cout << "White Destination: ";
		else
			cout << "Black destination: ";

		getline(cin, destination);

		// Test the input
		// Test if the first character is of letters A-H
		for (int a = 0; a <= FILE_SIZE; a++)
		{
			// Test if the second character is a number from 1-8
			for (int z = 1; z <= RANK_SIZE; z++)
			{
				if (toupper(destination[0]) == Alpha[a] && destination[1] == (z + 48))
				{
					// Convert the input to the board's x and y coordinates
					destinationx = z - 1;
					destinationy = a;

					// Check if the move is valid
					if (validMove(originx, originy, destinationx, destinationy, color))
					{
						// The move is valid, erase the source piece and move it to the destination
						// Check if the king is in check and if the mve will bring the king out of check
						// Check if the player's king is currently in check
						if (inCheck(color))
						{
							// The player's king is in check
							// Check if the current move will bring them out of check
							if (inCheck(originx, originy, destinationx, destinationy, color))
							{
								// The move brings white out of check!
								destinationMove = true;
							}
							else
							{
								// The mnove does not bring white out of check
								cout << "This move does not bring you out of check!" << endl;
								cout << "Press any key to continue..." << endl;
								cin.get();
								destinationMove = false;
							}
						}
						else
						{
							// White is not in check!
							destinationMove = true;
						}
					}
				}
			}
		}
	}

	// Check if originMove is valid
	if (!originMove && destinationMove)
	{
		// That is not a valid origin square
		cout << "That is not a valid origin square..." << endl;
		cout << "Make sure your input is in the format, A5 G3, etc." << endl;
		cout << "Press any key to continue..." << endl;
		cin.get();
	}
	else if (originMove && !destinationMove)
	{
		// That is not a valid move
		cout << "That is not a valid destination move..." << endl;
		cout << "Make sure the input is entered as B5, G3 etc." << endl;
		cout << "Make sure your piece can make that move..." << endl;
		cout << "Press any key to continue..." << endl;
		cin.get();
	}
	else if (!originMove && !destinationMove)
	{
		// Not a valid origin and destination move
		cout << "That is not a valid origin or destination..." << endl;
		cout << "Press any key to continue..." << endl;
		cin.get();
	}
	else
	{
		// The player has selected a valid source piece and a valid destination square
		// Move the piece to the destination and check if a piece is being captured
		// TODO: Create a check for sequential moves
		//	Create a check for sequential king moves when only the king can move
		if (isOccupied(destinationx, destinationy))
		{
			// There is a piece, capture the piece and move the origin piece to the destination
			capture = board[destinationx][destinationy];
			Capture(destinationx, destinationy, color);
			temp = board[originx][originy];
			board[originx][originy] = NULL;
			board[destinationx][destinationy] = temp;
			// Save the move, get the first piece
			SaveMove(board[destinationx][destinationy], originx, originy, destinationx, destinationy, color, true, capture);
			MoveNumber++;

			if (color == "White")
			{
				SequentialMoves(whitePiecesBoard, color);
			}
			else
			{
				SequentialMoves(blackPiecesBoard, color);
			}
		}
		else
		{
			// The square is not populated
			capture = ' ';
			temp = board[originx][originy];
			board[originx][originy] = NULL;
			board[destinationx][destinationy] = temp;
			SaveMove(board[destinationx][destinationy], originx, originy, destinationx, destinationy, color, false, capture);
			MoveNumber++;

			if (color == "White")
			{
				SequentialMoves(whitePiecesBoard, color);
			}
			else
			{
				SequentialMoves(blackPiecesBoard, color);
			}
		}
	}
}

// Check if the following move is valid
bool validMove(int originx, int originy, int destinationx, int destinationy, string color)
{
	// TODO: En passant for white and black pawns
	//	The engine needs to know if a the opponent has moved two spaces past a pawn
	//	If the piece is a pawn and has moved two spaces, save the move to a temporary variable
	//	Add logic to white and black pawns capturing in en passant

	// Check if the current piece can move to the destination
	// Find the pience that is being moved and the piece type and find the avalable moves, if the destinaton is a valid move, return true
	// If white is in check, make sure the move white makes gets them out of check
	for (int piece = 0; piece <= 16; piece++)
	{
		// Check if there is a valid piece
		if (whitePieces[piece] == tolower(board[originx][originy]))
		{
			// Is the piece a pawn?
			if (tolower(board[originx][originy]) == 'p')
			{
				if (color == "White")
				{
					// Where can a pawn move?
					// A pawn can move two spaces on its first move
					if (destinationy == originy - 2 && destinationx == originx && originy == 6)
					{
						// Check if the squares above the origin square are empty
						if (!isOccupied(originx, originy - 1) && !isOccupied(originx, originy - 2))
						{
							// Check if the player's king is currently in check
							if (inCheck(color))
							{
								// The player's king is in check
								// Check if the current move will bring them out of check
								if (inCheck(originx, originy, destinationx, destinationy, color))
								{
									// The move brings white out of check
									// Save the move for en passant
									originxSave = originx;
									originySave = originy;
									destinationxSave = destinationx;
									destinationySave = destinationy;
									return true;
								}
								else
								{
									// The mnove does not bring white out of check
									return false;
								}
							}
							else
							{
								// Save the move for en passant
								originxSave = originx;
								originySave = originy;
								destinationxSave = destinationx;
								destinationySave = destinationy;
								return true;
							}
						}
						else
						{
							return false;
						}
					}
					// The pawn is trying to move one space from where it currently is
					if (destinationy == originy - 1 && destinationx == originx)
					{
						if (!isOccupied(destinationx, destinationy))
						{
							// TODO: Ckeck if the move brings black into check
							return true;
						}
						else
						{
							return false;
						}
					}
					// Is white trying to capture up and to the left?
					if (originx - 1 == destinationx && originy - 1 == destinationy)
					{
						// Is there is a piece where white is trying to capture?
						if (isOccupied(destinationx, destinationy))
						{
							// TODO: Ckeck if the move brings white into check
							return true;
						}
						else
						{
							// Check for en passant
							return false;
						}
					}
					// Is white trying to capture up and to the right
					if (originx + 1 == destinationx && originy - 1 == destinationy)
					{
						if (isOccupied(destinationx, destinationy))
						{
							// TOOD: Check if the move brings white into check
							return true;
						}
						else
						{
							// Check for en passant
							// Check if black moved a pawn two spaces in their last turn
							if (originxSave + 2 == destinationxSave)
							{
								// Black moved a pawn two spaces in their last turn
								// Check if the white piece destination is one square from the black pawn
								if (board[destinationxSave][destinationySave] == 'P')
								{
									// En Passant is possible

								}
							}
							return false;
						}
					}
				}
				else
				{
					// Where can a black pawn move?
					// A pawn can move two spaces on its first move
					if (destinationy == originy + 2 && destinationx == originx && originy == 2)
					{
						// Check if the squares above the origin square are empty
						if (!isOccupied(originx, originy + 1) && !isOccupied(originx, originy + 2))
						{
							// Check if the player's king is currently in check
							if (inCheck(color))
							{
								// The player's king is in check
								// Check if the current move will bring them out of check
								if (inCheck(originx, originy, destinationx, destinationy, color))
								{
									// The move brings white out of check
									// Save the move for en passant
									originxSave = originx;
									originySave = originy;
									destinationxSave = destinationx;
									destinationySave = destinationy;
									return true;
								}
								else
								{
									// The mnove does not bring white out of check
									return false;
								}
							}
							else
							{
								// Save the move for en passant
								originxSave = originx;
								originySave = originy;
								destinationxSave = destinationx;
								destinationySave = destinationy;
								return true;
							}
						}
						else
						{
							return false;
						}
					}
					// The pawn is trying to move one space from where it currently is
					if (destinationy == originy + 1 && destinationx == originx)
					{
						if (!isOccupied(destinationx, destinationy))
						{
							return true;
						}
						else
						{
							return false;
						}
					}
					// Is white trying to capture up and to the left?
					if (originx + 1 == destinationx && originy + 1 == destinationy)
					{
						// Is there is a piece where white is trying to capture?
						if (isOccupied(destinationx, destinationy))
						{
							return true;
						}
						else
						{
							return false;
						}
					}
					// Is white trying to capture up and to the right
					if (originx - 1 == destinationx && originy + 1 == destinationy)
					{
						if (isOccupied(destinationx, destinationy))
						{
							return true;
						}
						else
						{
							return false;
						}
					}
				}
			}
			else if (tolower(board[originx][originy]) == 'r')
			{
				signed int xdirection;
				signed int ydirection;

				// Where can a rook move?
				// A rook can moove alang ranks and files
				// A rook can capture along ranks and files
				// The rook must not move through other pieces, all squares from the origin to the destination must be empty
				// How many squares is the rook moving along ranks and files?
				// Find which direction the piece is moving
				// Find the distance in the x direction
				// Find the distance in the y direction

				xdirection = destinationx - originx;
				ydirection = destinationy - originy;

				// Rook moving in the positve x direction
				if (originy == destinationy && xdirection > 0)
				{
					// Check if there are any pieces occupying the space between the destination and the origin
					// The rook is mocving bwtween the origin and the positive x direction
					for (int i = destinationx - 1; i > originx; i--)
					{
						if (isOccupied(i, destinationy))
						{
							return false;
						}
					}

					return true;
				}
				// Rook moving in the negative x direction
				if (originy == destinationy && xdirection < 0)
				{
					// Check if there are any pieces occupying the space between the destination and the origin
					for (int i = destinationx + 1; i > originx; i--)
					{
						if (isOccupied(i, destinationy))
						{
							return false;
						}
					}

					return true;
				}
				// Rook moving in the positive y direction
				if (originx == destinationx && ydirection > 0)
				{
					// Check if there are any pieces occupying the space between the destination and the origin
					for (int i = destinationy - 1; i > originy; i--)
					{
						if (isOccupied(destinationx, i))
						{
							return false;
						}
					}

					return true;
				}
				// Rook moving in the negative y direction
				if (originx == destinationx && ydirection < 0)
				{
					// Check if there are any pieces occupying the space between the destination and the origin
					for (int i = destinationy + 1; i < originy; i--)
					{
						if (isOccupied(destinationx, i))
						{
							return false;
						}
					}

					return true;
				}
			}
			// Move logic for white's knights
			else if (tolower(board[originx][originy]) == 'n')
			{
				// Where can a knight move?
				// A knight can move in the poitive y axis two places and over one in the positive x and negative x
				// A knight can move the above flipped about the x axis
				// A knight can move in the positive x axis two places and over one in the positive y and negative y
				// A knight can move the above flipped about the y axis
				// A knight can jump over other pieces

				// Positive y axis and over one about the negative x axis
				if (destinationy == originy - 2 && destinationx == originx - 1)
				{
					return true;
				}
				// Positive y axis and over one about the positive x axis
				else if (destinationy == originy - 2 && destinationx == originx + 1)
				{
					return true;
				}
				// Negative y axis and over one about the negative x axis
				else if (destinationy == originy + 2 && destinationx == originx - 1)
				{
					return true;
				}
				// Negative y axis and over one about the positive x axis
				else if (destinationy == originy + 2 && destinationx == originx + 1)
				{
					return true;
				}
				// Positive x axis and over one about the negative y axis
				else if (destinationx == originx + 2 && destinationy == originy + 1)
				{
					return true;
				}
				// Positive x axis and over one about the positive y axis
				else if (destinationx == originx + 2 && destinationy == originy - 1)
				{
					return true;
				}
				// Negative x axis and over one about the negative y axis
				else if (destinationx == originx - 2 && destinationy == originy + 1)
				{
					return true;
				}
				// Negative x axis and over one about the positive y axis
				else if (destinationx == originx - 2 && destinationy == originy - 1)
				{
					return true;
				}
				else
				{
					// Move not listed in valid knight moves
					return false;
				}
			}
			else if (tolower(board[originx][originy]) == 'b')
			{	
				// Where can a bishop move?
				// A bishop can move in diagonals in any direction
				// A bishop cannot move through other pieces
				// Calculate the change in x and y

				// For loop to scan all spaces in which the bishop can move
				for (int i = 0; i <= 8; i++)
				{
					// Positive y direction and positive x direction
					if (destinationx == originx + i && destinationy == originy + i)
					{
						// The bishop is attempting to move diagonaly
						// Check the previous spaces if there is a piece in the way
						for (int n = destinationx; n > originx; n--)
						{
							if (isOccupied(originx + n, originy + n))
							{
								// There is a piece in between the bishop and its destination
								// Check if the current square is the last square in the move
								if (n != destinationx)
								{
									// The square is the last square in the move
									// Check the other squares in the move and test if they are occupied
									return false;
								}
							}
						}
						return true;
					}
					// Positive y direction and negative x direction
					else if (destinationx == originx - i && destinationy == originy + i)
					{
						for (int n = destinationx; n < originx; n++)
						{
							if (isOccupied(originx - n, originy + n))
							{
								if (n != destinationx)
								{
									// The square is the last square in the move
									// Check the other squares in the move and test if they are occupied
									return false;
								}
							}
						}
						return true;
					}
					// Negative y direction and positive x direction
					else if (destinationx == originx + i && destinationy == originy + i)
					{
						// The bishop is attmepting to move diagonally
						for (int n = destinationx; n > originx; n--)
						{
							if (isOccupied(originx - n, originy - n))
							{
								if (n != destinationx)
								{
									// The square is the last square in the move
									// Check the other squares in the move and test if they are occupied
									return false;
								}
							}
						}
						return true;
					}
					// Negative y direction and negative x direction
					else if (destinationx == originx - i && destinationy == originy - i)
					{
						// The bishop is attempting to move diagonally
						for (int n = destinationx; n < originx; n++)
						{
							if (isOccupied(originx + n, originy - n))
							{
								if (n != destinationx)
								{
									// The square is the last square in the move
									// Check the other squares in the move and test if they are occupied
									return false;
								}
							}
						}
						return true;
					}
				}

				// A valid bishop move has not been found
				return false;
			}
			else if (tolower(board[originx][originy]) == 'q')
			{
				signed int xdirection;
				signed int ydirection;

				// Where can a queen move?
				// The queen can move in any direction along ranks and files in an unlimted range
				// The queen can move in any direction along diagonals in an unlimited range

				xdirection = destinationx - originx;
				ydirection = destinationy - originy;

				// Queen moving in the positve x direction
				if (originy == destinationy && xdirection > 0)
				{
					// Check if there are any pieces occupying the space between the destination and the origin
					// The queen is moving between the origin and the positive x direction
					for (int i = destinationx - 1; i > originx; i--)
					{
						if (isOccupied(i, originy))
						{
							return false;
						}
					}

					return true;
				}
				// Queen moving in the negative x direction
				else if (originy == destinationy && xdirection < 0)
				{
					// Check if there are any pieces occupying the space between the destination and the origin
					for (int i = destinationx + 1; i < originx; i++)
					{
						if (isOccupied(i, originy))
						{
							return false;
						}
					}

					return true;
				}
				// Queen moving in the positive y direction
				else if (originx == destinationx && ydirection > 0)
				{
					// Check if there are any pieces occupying the space between the destination and the origin
					for (int i = destinationy - 1; i > originy; i--)
					{
						if (isOccupied(originx, i))
						{
							return false;
						}
					}

					return true;
				}
				// Queen moving in the negative y direction
				else if (originx == destinationx && ydirection < 0)
				{
					// Check if there are any pieces occupying the space between the destination and the origin
					for (int i = destinationy + 1; i < originy; i++)
					{
						if (isOccupied(originx, i))
						{
							return false;
						}
					}

					return true;
				}
				else
				{
					for (int i = 0; i <= 8; i++)
					{
						// Positive y direction and positive x direction
						if (destinationx == originx + i && destinationy == originy + i)
						{
							// The queen is attempting to move diagonaly
							// Check the previous spaces if there is a piece in the way
							for (int n = destinationx; n < originx; n--)
							{
								if (isOccupied(destinationx + n, destinationy + n))
								{
									// There is a piece in between the bishop and its destination
									// Check if the current square is the last square in the move
									if (n != destinationx)
									{
										// The square is the last square in the move
										// Check the other squares in the move and test if they are occupied
										return false;
									}
								}
							}
							return true;
						}
						// Positive y direction and negative x direction
						else if (destinationx == originx - i && destinationy == originy + i)
						{
							// The queen is attmepting to move diagonally
							for (int n = destinationx; n < originx; n++)
							{
								if (isOccupied(destinationx - n, destinationy + n))
								{
									if (n != destinationx)
									{
										// The square is the last square in the move
										// Check the other squares in the move and test if they are occupied
										return false;
									}
								}
							}
							return true;
						}
						// Negative y direction and positive x direction
						else if (destinationx == originx - i && destinationy == originy - i)
						{
							// The queen is attmepting to move diagonally
							for (int n = destinationx; n > originx; n--)
							{
								if (isOccupied(destinationx - n, destinationy - n))
								{
									if (n != destinationx)
									{
										// The square is the last square in the move
										// Check the other squares in the move and test if they are occupied
										return false;
									}
								}
							}
							return true;
						}
						// Negative y direction and negative x direction
						else if (destinationx == originx - i && destinationy == originy - i)
						{
							// The queen is attempting to move diagonally
							for (int n = destinationx; n < originx; n++)
							{
								if (isOccupied(destinationx - n, destinationy - n))
								{
									if (n != destinationx)
									{
										// The square is the last square in the move
										// Check the other squares in the move and test if they are occupied
										return false;
									}
								}
							}
							return true;
						}
					}

					// A valid bishop move has not been found
					return false;
				}
			}
			else if (tolower(board[originx][originy]) == 'k')
			{
				// Where can a King move?
				// A King can move one spsace in any direction
				// King moving x does not change and moving in the positive y direction
				if (destinationx == originx && destinationy == originy - 1)
				{
					return true;
				}
				// King moving in the positive x direction and the positive y direciton
				else if (destinationx == originx + 1 && destinationy == originy - 1)
				{
					return true;
				}
				// King moving in the positive x direction and the negative y direction
				else if (destinationx == originx + 1 && destinationy == originy + 1)
				{
					return true;
				}
				// King moving x does not change y moving in the negative y direction
				else if (destinationx == originx && destinationy == originy + 1)
				{
					return true;
				}
				// King moving in the negative x direction and the negative y direction
				else if (destinationx == originx - 1 && destinationy == originy - 1)
				{
					return true;
				}
				// King moving in the negative x direction and y does not change
				else if (destinationx == originx - 1 && destinationy == originy)
				{
					return true;
				}
				// King moving in the negative x direction and the positive y direction
				else if (destinationx == originx - 1 && destinationy == originy - 1)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				// The piece selected is not valid
				return false;
			}
		}
	}
	return false;
}

// Save the move
void SaveMove(char Piece, int originx, int originy, int destinationx, int destinationy, string color, bool capture, char captured)
{
	fstream Moves;
	string save;
	char OriginRank = Alpha[originx];
	char OriginFile = originy + 1;
	char DestinationRank = Alpha[destinationx];
	char DestinationFile = destinationy + 1;

	Moves.open("Moves.txt");

	if (Moves.is_open())
	{
		save = MoveNumber;
		save += ". ";
		save += color;
		save += ' ';
		save += OriginRank;
		save += OriginFile;
		save += ' ';
		if (capture)
		{
			save += "Takes";
			save += ' ';
			save += captured;
			save += ' ';
		}
		else
		{
			save += "->";
			save += ' ';
		}
		save += DestinationRank;
		save += DestinationFile;

		Moves << save << endl;
	}
	else
	{
		cout << "Unable to open moves file..." << endl;
	}
}

// Check if the white or black king is in check
bool inCheck(string color)
{
	int kingx;
	int kingy;

	// Is white currently in check?
	if (color == "White")
	{
		// How check if the king is being threatened by another piece
		// Can an opponent's piece possibly capture the king?
		// Scan all the entire board looking for the white king
		// Then scan for all of black's pieces scaning if they can move to where the white King is located
		for (int y = 0; y <= RANK_SIZE; y++)
		{
			for (int x = 0; x <= FILE_SIZE; x++)
			{
				// If the loop finds the white king, save the coordinates
				if (board[x][y] == 'k')
				{
					// The white king has been found!
					kingx = x;
					kingy = y;
				}
			}
		}
		// Scan for all black's pieces
		for (int y = 0; y <= FILE_SIZE; y++)
		{
			for (int x = 0; x <= RANK_SIZE; x++)
			{
				for (int i = 0; i <= 16; i++)
				{
					if (blackPiecesBoard[i] == board[x][y])
					{
						// Since the valid move function calls for the inCheck() function, there will be a feedback loop and would crash the program. Find a way to check valid move
						// A black piece has been found! Check if a move to capture the king is valid
						if (validMove(x, y, kingx, kingy, color))
						{
							return true;
						}
						else
						{
							return false;
						}
					}
				}
			}
		}
	}
	// Is black in check?
	else
	{
		for (int y = 0; y <= RANK_SIZE; y++)
		{
			for (int x = 0; x <= FILE_SIZE; x++)
			{
				// If the loop finds the black king, save the cordinates
				if (board[x][y] == 'K')
				{
					// The white king has been found!
					kingx = x;
					kingy = y;
				}
			}
		}
		// Scan for all white's pieces
		for (int y = 0; y <= FILE_SIZE; y++)
		{
			for (int x = 0; x <= RANK_SIZE; x++)
			{
				for (int i = 0; i <= 16; i++)
				{
					if (whitePiecesBoard[i] == board[x][y])
					{
						// A black piece has been found! Check if a move to capture the king is valid
						if (validMove(x, y, kingx, kingy, color))
						{
							return true;
						}
						else
						{
							return false;
						}
					}
				}
			}
		}
	}
	return false;
}

// Does the current move bring the player out of check?
bool inCheck(int originx, int originy, int destinationx, int destinationy, string color)
{
	char temp;
	char temp2;

	// The current player's king is in check, check if the current move will get them out of check
	if (inCheck(color))
	{
		// Move the piece temporarily to the destination
		temp = board[originx][originy];
		temp2 = board[destinationx][destinationy];
		board[originx][originy] = NULL;
		board[destinationx][destinationy] = temp;

		if (!inCheck(color))
		{
			// The move brings the player out of check
			// Move the piece back to the original position
			board[originx][originy] = temp;
			board[destinationx][destinationy] = temp2;
			return false;
		}
		else
		{
			// The player's move does not being them out of check
			// Move the piece back to the original location
			board[originx][originy] = temp;
			board[destinationx][destinationy] = temp2;
			return true;
		}
	}
	else
	{
		// The player is not in check and can move their piece to the destination
		return false;
	}
	return false;
}

// Check if the game has been stalemated
bool Stalemate(string color)
{
	// A game is ruled a draw when:
	// 1. One of the players is not in check and cannot move one of their pieces
	// 2. When both players cannot possibly win the game
	//	a. King vs. king
	//	b. King and bishop vs. King
	//	c. king and knight vs. King
	//	d. King and bishop vs. King and with the bishops on the same color
	// 3. When the player has moved their king 21 times without being checkmated
	// 4. When a player makes the same two moves three times in a row
	// 5. When both players agree to a draw

	if (color == "White")
	{
		if (UnableToMove(color))
		{
			cout << "White cannot move... The game is stalemated." << endl;
			return true;
		}
		else if (NoWinnerPossible())
		{
			cout << "There is no winner possible... The game is stalemated." << endl;
			return true;
		}
		else if (WhiteKingMoves == 50)
		{
			cout << "White has moved their king 50 times with out being checkmated..." << endl;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (UnableToMove(color))
		{
			cout << "White cannot move... The game is stalemated." << endl;
			return true;
		}
		else if (NoWinnerPossible())
		{
			cout << "There is no winner possible... The game is stalemated." << endl;
			return true;
		}
		else if (BlackKingMoves == 50)
		{
			cout << "White has moved their king 50 times with out being checkmated..." << endl;
			return true;
		}
		else
		{
			return false;
		}
	}
}

// Check if a certain color is able to move a piece
bool UnableToMove(string color)
{
	int originx;
	int originy;
	int destinationx;
	int destinationy;

	if (color == "White")
	{
		// 1. Check when players are not in check and cannot move any of their pieces
		// Check all the pieces for one player if they can make a move without being in check
		for (int y = 0; y < FILE_SIZE; y++)
		{
			for (int x = 0; x < RANK_SIZE; x++)
			{
				// Scan through all of white's pieces
				for (int i = 0; i < 16; i++)
				{
					if (board[x][y] == whitePiecesBoard[i])
					{
						originx = x;
						originy = y;

						// A white piece has been found, check if it has any valid moves that do not end with white being in check
						// Check if white is in check
						if (!inCheck("White"))
						{
							// Check if the piece has any moves and does not put white in check
							for (int n = 0; n < FILE_SIZE; n++)
							{
								for (int m = 0; m < RANK_SIZE; m++)
								{
									destinationx = m;
									destinationy = n;

									// Check if the current move is valid
									if (validMove(originx, originy, destinationx, destinationy, "White"))
									{
										// The piece has a valid move
										// Check if the current valid move brings white into check
										if (!inCheck(originx, originy, destinationx, destinationy, "White"))
										{
											// The current move does not bring white into check, thus white has a valid move
											return false;
										}
									}
								}
							}
						}
						else
						{
							// White is in check and thus stalemate is false
							return false;
						}
					}
				}
			}
		}
		// White does not have any valid moves
		return true;
	}
	else
	{
		for (int y = 0; y < FILE_SIZE; y++)
		{
			for (int x = 0; x < RANK_SIZE; x++)
			{
				// Scan through all of white's pieces
				for (int i = 0; i < 16; i++)
				{
					if (board[x][y] == blackPiecesBoard[i])
					{
						originx = x;
						originy = y;

						// A white piece has been found, check if it has any valid moves that do not end with white being in check
						// Check if white is in check
						if (!inCheck("White"))
						{
							// Check if the piece has any moves and does not put white in check
							for (int n = 0; n < FILE_SIZE; n++)
							{
								for (int m = 0; m < RANK_SIZE; m++)
								{
									destinationx = m;
									destinationy = n;

									// Check if the current move is valid
									if (validMove(originx, originy, destinationx, destinationy, "White"))
									{
										// The piece has a valid move
										// Check if the current valid move brings white into check
										if (!inCheck(originx, originy, destinationx, destinationy, "White"))
										{
											// The current move does not bring white into check, thus white has a valid move
											return false;
										}
									}
								}
							}
						}
						else
						{
							// White is in check and thus stalemate is false
							return false;
						}
					}
				}
			}
		}
		// Black does not have any valid moves
		return true;
	}
}

// Check both players the pieces that are remaining
//	a. King vs. king
//	b. King and bishop vs. King
//	c. king and knight vs. King
//	d. King and bishop vs. King and Bishop
//	e. King and knight vs. King and knight
bool NoWinnerPossible()
{
	// Check white's captured pieces for the above scenarios
	int white = 0;
	int black = 0;

	// Scan through the board for white's non-captured pieces
	for (int y = 0; y < FILE_SIZE; y++)
	{
		for (int x = 0; x < RANK_SIZE; x++)
		{
			// Search for white pieces
			for (int i = 0; i < 16; i++)
			{
				if (board[x][y] == whitePiecesBoard[i])
				{
					// Search for the first open spot in the array
					for (int j = 0; j < 16; j++)
					{
						if (whitePiecesBoard[j] == NULL)
						{
							whitePiecesBoard[j] = board[x][y];
							white++;
							break;
						}
					}
				}
			}
		}
	}

	// Scan through the board for black's non-captured pieces
	for (int y = 0; y < FILE_SIZE; y++)
	{
		for (int x = 0; x < RANK_SIZE; x++)
		{
			// Search for black pieces
			for (int i = 0; i < 16; i++)
			{
				if (board[x][y] == blackPiecesBoard[i])
				{
					// Search for the first open spot in the array
					for (int j = 0; j < 16; j++)
					{
						if (blackPieces[j] == NULL)
						{
							blackPiecesBoard[j] = board[x][y];
							black++;
							break;
						}
					}
				}
			}
		}
	}

	// Sort the two arrays based on the value of the pieces
	SortPieces(whitePiecesBoard, white, "White");
	SortPieces(blackPiecesBoard, black, "Black");

	//	a. King vs. King
	//	b. King and bishop vs. King white
	//	c. king and knight vs. King white
	//	d. King and bishop vs. King and Bishop white
	//	e. King and knight vs. King and knight white

	//	f. King and bishop vs. King black
	//	g. king and knight vs. King black

	if (whitePiecesBoard[0] == 'k' && whitePiecesBoard[1] == NULL && blackPiecesBoard[0] == 'K' && blackPiecesBoard[1] == NULL)
	{
		// The game is rulled a stalemate
		cout << "There is no winner possible..." << endl;
		return true;
	}
	else if (whitePiecesBoard[0] == 'k' && whitePiecesBoard[1] == 'b' && whitePiecesBoard[2] == NULL && blackPiecesBoard[0] == 'K' && blackPiecesBoard[1] == NULL)
	{
		// The game is rulled a stalemate
		cout << "There is no winner possible..." << endl;
		return true;
	}
	else if (whitePiecesBoard[0] == 'k' && whitePiecesBoard[1] == 'n' && whitePiecesBoard[2] == NULL && blackPiecesBoard[0] == 'K' && blackPiecesBoard[1] == NULL)
	{
		// The game is rulled a stalemate
		cout << "There is no winner possible..." << endl;
		return true;
	}
	else if (whitePiecesBoard[0] == 'k' && whitePiecesBoard[1] == 'b' && whitePiecesBoard[2] == NULL && blackPiecesBoard[0] == 'K' && blackPiecesBoard[1] == 'B' && blackPiecesBoard[2] == NULL)
	{
		// The game is rulled a stalemate
		cout << "There is no winner possible..." << endl;
		return true;
	}
	else if (whitePiecesBoard[0] == 'k' && whitePiecesBoard[1] == 'n' && whitePiecesBoard[2] == NULL && blackPiecesBoard[0] == 'K' && blackPiecesBoard[1] == 'n' && blackPiecesBoard[2] == NULL)
	{
		// The game is rulled a stalemate
		cout << "There is no winner possible..." << endl;
		return true;
	}
	else if (whitePiecesBoard[0] == 'k' && whitePiecesBoard[1] == NULL && blackPiecesBoard[0] == 'K' && blackPiecesBoard[1] == 'N' && blackPiecesBoard[2] == NULL)
	{
		// The game is rulled a stalemate
		cout << "There is no winner possible..." << endl;
		return true;
	}
	else if (whitePiecesBoard[0] == 'k' && whitePiecesBoard[1] == NULL && blackPiecesBoard[0] == 'K' && blackPiecesBoard[1] == 'N' && blackPiecesBoard[2] == NULL)
	{
		// The game is rulled a stalemate
		cout << "There is no winner possible..." << endl;
		return true;
	}
	else
	{
		return false;
	}
}

// Check for sequential moves
void SequentialMoves(char Pieces[], string color)
{
	// Check if the color has only their King
	if (color == "White")
	{
		SortPieces(Pieces, sizeof(Pieces), color);

		if (Pieces[0] == 'k' && Pieces[1] == NULL)
		{
			// Only the white king remains
			WhiteKingMoves++;
		}
	}
	else
	{
		SortPieces(Pieces, sizeof(Pieces), color);

		if (Pieces[0] == 'K' && Pieces[1] == NULL)
		{
			// Only the black king remains
			BlackKingMoves++;
		}
	}
}

// Check if white has been checkmated
bool CheckmateWhite()
{
	int kingx;
	int kingy;

	for (int y = 0; y < RANK_SIZE; y++)
	{
		for (int x = 0; x < FILE_SIZE; x++)
		{
			// If the loop finds the black king, save the cordinates
			if (board[x][y] == 'K')
			{
				// The white king has been found!
				kingx = x;
				kingy = y;
			}
		}
	}
	// Scan for all white's pieces
	for (int y = 0; y < FILE_SIZE; y++)
	{
		for (int x = 0; x < RANK_SIZE; x++)
		{
			for (int i = 0; i < 16; i++)
			{
				if (board[x][y] == whitePiecesBoard[i])
				{
					// A White piece has been found! Check if a move to capture the king is valid and check if this is the last square on the board
					if (validMove(x, y, kingx, kingy, "White"))
					{
						// The current piece has placed the king into check, can any of white's pieces move to get white's king out of check?
						// Scan the entire boad for white pieces
						for (int b = 0; y < FILE_SIZE; y++)
						{
							for (int a = 0; x < RANK_SIZE; x++)
							{
								// Cycle through the pieces checking for a matching white piece
								for (int j = 0; j <= 16; j++)
								{
									if (board[a][b] == whitePiecesBoard[j])
									{
										// Scan through the entire board, searching for possible squares the white piece can move
										for (int l = 0; l < FILE_SIZE; l++)
										{
											for (int k = 0; k < RANK_SIZE; k++)
											{
												// Check if the piece can move there with validMove()
												if (validMove(a, b, k, l, "White"))
												{
													// The piece can move there!
													// Check if it is still in check after the move
													if (!inCheck(a, b, k, l, "White"))
													{
														return false;
													}
												}
											}
										}
									}
								}
							}
						}
						// None of white's pieces can move to keep black from checkmating white, black wins!
						return true;
					}
				}
			}
		}
	}
	// Black is not in check...
	return false;
}

// Check if black has been checkmated
bool CheckmateBlack()
{
	int kingx;
	int kingy;

	for (int y = 0; y < RANK_SIZE; y++)
	{
		for (int x = 0; x < FILE_SIZE; x++)
		{
			// If the loop finds the black king, save the cordinates
			if (board[x][y] == 'K')
			{
				// The white king has been found!
				kingx = x;
				kingy = y;
			}
		}
	}
	// Scan for all white's pieces
	for (int y = 0; y < FILE_SIZE; y++)
	{
		for (int x = 0; x < RANK_SIZE; x++)
		{
			for (int i = 0; i < 16; i++)
			{
				if (board[x][y] == blackPiecesBoard[i])
				{
					// A White piece has been found! Check if a move to capture the king is valid and check if this is the last square on the board
					if (validMove(x, y, kingx, kingy, "Black"))
					{
						// The current piece has placed the king into check, can any of black's pieces move to get white's king out of check?
						// Scan the entire boad for white pieces
						for (int b = 0; y < FILE_SIZE; y++)
						{
							for (int a = 0; x < RANK_SIZE; x++)
							{
								// Cycle through the pieces checking for a matching black piece
								for (int j = 0; j <= 16; j++)
								{
									if (board[a][b] == blackPiecesBoard[j])
									{
										// Scan through the entire board, searching for possible squares the black piece can move
										for (int l = 0; l < FILE_SIZE; l++)
										{
											for (int k = 0; k < RANK_SIZE; k++)
											{
												// Check if the piece can move there with validMove()
												if (validMove(a, b, k, l, "Black"))
												{
													// The piece can move there!
													// Check if it is still in check after the move
													if (!inCheck(a, b, k, l, "Black"))
													{
														return false;
													}
												}
											}
										}
									}
								}
							}
						}
						// None of white's pieces can move to keep black from checkmating white, black wins!
						return true;
					}
				}
			}
		}
	}
	// Black is not in check...
	return false;
}

bool isOccupied(int x, int y)
{
	for (int i = 0; i <= 16; i++)
	{
		if (board[x][y] == whitePiecesBoard[i] || board[x][y] == blackPiecesBoard[i])
		{
			return true;
		}
	}
	return false;
}

void Capture(int x, int y, string color)
{
	if (color == "White")
	{
		// Capture the piece at the coordinates
		// Place the captured piece at the last on the list
		for (int i = 0; i <= 16; i++)
		{
			if (whiteCaptured[i] == NULL)
			{
				// Place the captured piece
				whiteCaptured[i] = board[x][y];
				SortCaptured(whiteCaptured, 16, color);
				board[x][y] = NULL;

				// Remove the piece from the board variables
				for (int j = 0; j < 16; j++)
				{
					if (whitePiecesBoard[j] == board[x][y])
					{
						whitePiecesBoard[j] = NULL;
						SortPieces(whitePiecesBoard, sizeof(whitePiecesBoard), color);
					}
				}
			}
		}
	}
	else
	{
		// Capture the piece at the coordinates
		// Place the captured piece at the last on the list
		for (int i = 0; i <= 16; i++)
		{
			if (blackCaptured[i] == NULL)
			{
				// Place the captured piece
				blackCaptured[i] = board[x][y];
				SortCaptured(blackCaptured, 16, color);
				board[x][y] = NULL;
			}
		}
	}
}

void SortPieces(char *Pieces, int size, string color)
{
	int temp[sizeof(Pieces)];

	// Based on the following piece definitions, sort the captured pieces
	Piece Pawn;
	Pawn.character = 'P';
	Pawn.value = 1;

	Piece Knight;
	Knight.character = 'N';
	Knight.value = 2;

	Piece Bishop;
	Bishop.character = 'B';
	Bishop.value = 3;

	Piece Rook;
	Rook.character = 'R';
	Rook.value = 4;

	Piece Queen;
	Queen.character = 'Q';
	Queen.value = 5;

	Piece King;
	King.character = 'K';
	King.value = 6;

	// Replace each character with its value in the array
	for (int i = 0; i <= size; i++)
	{
		if (toupper(Pieces[i]) == Pawn.character)
		{
			temp[i] = Pawn.value;
		}
		else if (toupper(Pieces[i]) == Knight.character)
		{
			temp[i] = Knight.value;
		}
		else if (toupper(Pieces[i]) == Bishop.character)
		{
			temp[i] = Bishop.value;
		}
		else if (toupper(Pieces[i] == Rook.character))
		{
			temp[i] = Rook.value;
		}
		else if (toupper(Pieces[i]) == Queen.character)
		{
			temp[i] = Queen.value;
		}
		else if (toupper(Pieces[i]) == King.character)
		{
			temp[i] == King.value;
		}
	}

	// Sort the values in the array
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16 - i - 1; j++)
		{
			if (Pieces[j] > Pieces[j + 1])
			{
				swap(&temp[j], &temp[j + 1]);
			}
		}
	}

	// Based on the now sorted temp array, sort the values in the pointed array
	for (int i = 0; i < sizeof(temp); i++)
	{
		if (color == "White")
		{
			if (temp[i] == Pawn.value)
			{
				Pieces[i] = tolower(Pawn.character);
			}
			else if (temp[i] == Knight.value)
			{
				Pieces[i] = tolower(Knight.character);
			}
			else if (temp[i] == Bishop.value)
			{
				Pieces[i] = tolower(Bishop.character);
			}
			else if (temp[i] == Rook.value)
			{
				Pieces[i] = tolower(Rook.character);
			}
			else if (temp[i] == Queen.value)
			{
				Pieces[i] = tolower(Queen.character);
			}
			else if (temp[i] == King.value)
			{
				Pieces[i] = tolower(King.character);
			}
		}
		else
		{
			if (temp[i] == Pawn.value)
			{
				Pieces[i] = toupper(Pawn.character);
			}
			else if (temp[i] == Knight.value)
			{
				Pieces[i] = toupper(Knight.character);
			}
			else if (temp[i] == Bishop.value)
			{
				Pieces[i] = toupper(Bishop.character);
			}
			else if (temp[i] == Rook.value)
			{
				Pieces[i] = toupper(Rook.character);
			}
			else if (temp[i] == Queen.value)
			{
				Pieces[i] = toupper(Queen.character);
			}
			else if (temp[i] == King.value)
			{
				Pieces[i] = toupper(King.character);
			}
		}
	}
}

void SortCaptured(char Pieces[], int size, string color)
{
	int temp[sizeof(Pieces)];

	// Based on the following piece definitions, sort the captured pieces
	Piece Pawn;
	Pawn.character = 'P';
	Pawn.value = 1;

	Piece Knight;
	Knight.character = 'N';
	Knight.value = 2;

	Piece Bishop;
	Bishop.character = 'B';
	Bishop.value = 3;

	Piece Rook;
	Rook.character = 'R';
	Rook.value = 4;

	Piece Queen;
	Queen.character = 'Q';
	Queen.value = 5;

	// Replace each character with its value in the Piecesay
	for (int i = 0; i <= size; i++)
	{
		if (toupper(Pieces[i]) == Pawn.character)
		{
			// Assign the pawn's value
			temp[i] = Pawn.value;
		}
		else if (toupper(Pieces[i]) == Knight.character)
		{
			// Assign the knight's value
			temp[i] = Knight.value;
		}
		else if (toupper(Pieces[i]) == Bishop.character)
		{
			// Assign the bishop's value
			temp[i] = Bishop.value;
		}
		else if (toupper(Pieces[i]) == Rook.character)
		{
			// Assign the Rook's value
			temp[i] = Rook.value;
		}
		else if (toupper(Pieces[i]) == Queen.character)
		{
			// Assign the Queen's value
			temp[i] = Queen.value;
		}
	}

	// Sort values in the Pieces
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16 - i; j++)
		{
			if (Pieces[j] > Pieces[j + 1])
			{
				swap(&temp[j], &temp[j + 1]);
			}
		}
	}

	// Based on the color of the Piecesay, assign the pices back to the captured pieces
	for (int i = 0; i < sizeof(temp); i++)
	{
		Pieces[i] = temp[i];
	}
}

void swap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

// Get the names for player one and two
void PlayerName()
{
	bool playername1 = false;
	bool playername2 = false;
	string input;

	while (!playername1)
	{
		system("cls");

		PrintTitle();
		PrintBoard();

		cout << "Player 1, enter your name: ";
		getline(cin, input);

		if (input.length() > 1)
		{
			// Make the first letter capitalized
			input[0] = toupper(input[0]);
			Player1.name = input;
			playername1 = true;
		}
		else
		{
			cout << "The name you entered is not valid..." << endl;
			playername1 = false;
		}
	}

	while (!playername2)
	{
		system("cls");

		PrintTitle();
		PrintBoard();

		cout << "Player 2, enter your name: ";
		getline(cin, input);

		if (input.length() > 1)
		{
			// Make the first letter capitalized
			input[0] = toupper(input[0]);
			Player2.name = input;
			playername2 = true;
		}
		else
		{
			cout << "The name you entered is not valid..." << endl;
			playername2 = false;
		}
	}
}

void ResetBoard()
{
	// Reset the board so all pieces are in their starting positions
	// Set the black main pieces
	board[0][0] = 'R';
	board[1][0] = 'N';
	board[2][0] = 'B';
	board[3][0] = 'Q';
	board[4][0] = 'K';
	board[5][0] = 'B';
	board[6][0] = 'N';
	board[7][0] = 'R';

	// Set the black pawns
	for (int row = 0; row <= 7; row++)
	{
		board[row][1] = 'P';
	}

	// Set the main white pieces
	board[0][7] = 'r';
	board[1][7] = 'n';
	board[2][7] = 'b';
	board[3][7] = 'q';
	board[4][7] = 'k';
	board[5][7] = 'b';
	board[6][7] = 'n';
	board[7][7] = 'r';

	// Set the white pawns
	for (int row = 0; row <= 7; row++)
	{
		board[row][6] = 'p';
	}

	// Reset the rest of the board squares
	for (int row = 0; row <= 7; row++)
	{
		board[row][2] = ' ';
		board[row][3] = ' ';
		board[row][4] = ' ';
		board[row][5] = ' ';
	}

	// Populate the white board variables
	whitePiecesBoard[0] = 'p';
	whitePiecesBoard[1] = 'p';
	whitePiecesBoard[2] = 'p';
	whitePiecesBoard[3] = 'p';
	whitePiecesBoard[4] = 'p';
	whitePiecesBoard[5] = 'p';
	whitePiecesBoard[6] = 'p';
	whitePiecesBoard[7] = 'p';
	whitePiecesBoard[8] = 'r';
	whitePiecesBoard[9] = 'n';
	whitePiecesBoard[10] = 'b';
	whitePiecesBoard[11] = 'q';
	whitePiecesBoard[12] = 'k';
	whitePiecesBoard[13] = 'b';
	whitePiecesBoard[14] = 'n';
	whitePiecesBoard[15] = 'r';

	// Populate the black board variables
	blackPiecesBoard[0] = 'P';
	blackPiecesBoard[1] = 'P';
	blackPiecesBoard[2] = 'P';
	blackPiecesBoard[3] = 'P';
	blackPiecesBoard[4] = 'P';
	blackPiecesBoard[5] = 'P';
	blackPiecesBoard[6] = 'P';
	blackPiecesBoard[7] = 'P';
	blackPiecesBoard[8] = 'R';
	blackPiecesBoard[9] = 'N';
	blackPiecesBoard[10] = 'B';
	blackPiecesBoard[11] = 'Q';
	blackPiecesBoard[12] = 'K';
	blackPiecesBoard[13] = 'B';
	blackPiecesBoard[14] = 'N';
	blackPiecesBoard[15] = 'R';
}

void PrintBoard()
{
	fstream boardFile;
	string input;
	int x = 0;
	int y = 0;
	int posx = 0;
	int posy = 0;

	boardFile.open("Board.txt");

	while (getline(boardFile, input))
	{
		// Check if the current line is a line for printing the board array
		if ((posy % 2) == 0 && posy >= 2)
		{
			for (posx = 0; posx <= input.length(); posx++)
			{
				// If the position of posx is greater than or equal to three and the modulus of posx / 3 is equal to zero
				if (((posx - 4) % 4 == 0) && (posx >= 3))
				{
					// The above is true, print the board character
					cout << board[x][y];
					x++;
				}
				else if (posx == input.length())
				{
					// End of the input string, print the endline character
					cout << endl;
					x = 0;
					y++;
				}
				else
				{
					// Print the normal character
					cout << input[posx];
				}
			}
		}
		// Otherwise, print the entire line
		else
		{
			cout << input << endl;
		}

		posy++;
	}
}

void PrintTitle()
{
	fstream title;
	string input;

	title.open("Title.txt");

	while (getline(title, input))
	{
		cout << input << endl;
	}
	cout << endl;
}
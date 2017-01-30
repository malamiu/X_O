#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;

// глобальные константы.  глобальная константа доступна из любой части программы. Глобальная константа в основном применяется в тех случаях, когда константное значение требуется использовать не однократно (в отличие от локальной константы).
const char X = 'X';
const char O = 'O';
const char EMPTY = ' ';
const char TIE = 'T';
const char NO_ONE = 'N';

// прототипы функций
void instructions();
char askYesNo(string question); // Данная функция задает соответсвующий вопрос, который предпологает выбор - играть, соответственно y - X (первый ход за пользователем), n - O (пользователь ходит вторым)
int askNumber(string question, int high, int low = 0);
char humanPiece();
char opponent(char piece);
void displayBoard(const vector<char>& board);
char winner(const vector<char>& board);
bool isLegal(const vector<char>& board, int move);
int humanMove(const vector<char>& board, char human);
int computerMove(vector<char> board, char computer);
void announceWinner(char winner, char computer, char human);

// главная функция
int main()
{
	setlocale(LC_ALL, "Russian");
	int move;
	const int NUM_SQUARES = 9; // локальная константа 
	vector<char> board(NUM_SQUARES, EMPTY);

	instructions();
	char human = humanPiece();
	char computer = opponent(human);
	char turn = X;
	displayBoard(board);

	while (winner(board) == NO_ONE)
	{
		if (turn == human)
		{
			move = humanMove(board, human);
			board[move] = human;
		}
		else
		{
			move = computerMove(board, computer);
			board[move] = computer;
		}
		displayBoard(board);
		turn = opponent(turn);
	}

	announceWinner(winner(board), computer, human);
	system("pause");
	return 0;
}

// функции
void instructions()
{
	cout << "\t!!!Добро пожаловать в классическую логическую игру : Крестики-нолики!!!\n\n\n";

	cout << "Ваш ход будет приравниваться числу из диапазона [0 - 8].  Число\n";
	cout << "соотвествует желаемому расположению на поле, как показано ниже:\n\n";

	cout << "\t\t\t 0 | 1 | 2\n";
	cout << "\t\t\t ---------\n";
	cout << "\t\t\t 3 | 4 | 5\n";
	cout << "\t\t\t ---------\n";
	cout << "\t\t\t 6 | 7 | 8\n\n";

	cout << "Prepare yourself, human.  The battle is about to begin.\n\n";
}

char askYesNo(string question)
{
	char response;
	do
	{
		cout << question << " (y/n): ";
		cin >> response;
	} while (response != 'y' && response != 'n');

	return response;
}

int askNumber(string question, int high, int low) 
{
	int number;
	do
	{
		cout << question << " (" << low << " - " << high << "): ";
		cin >> number;
	} while (number > high || number < low);

	return number;
}

char humanPiece()
{
	char go_first = askYesNo("Do you require the first move?");
	if (go_first == 'y')
	{
		cout << "\nПервый ход пользователя.  You will need it.\n";
		return X;
	}
	else
	{
		cout << "\nYour bravery will be your undoing... I will go first.\n";
		return O;
	}
}

char opponent(char piece)
{
	if (piece == X)
	{
		return O;
	}
	else
	{
		return X;
	}
}

void displayBoard(const vector<char>& board)
{
	cout << "\n\t" << board[0] << " | " << board[1] << " | " << board[2];
	cout << "\n\t" << "---------";
	cout << "\n\t" << board[3] << " | " << board[4] << " | " << board[5];
	cout << "\n\t" << "---------";
	cout << "\n\t" << board[6] << " | " << board[7] << " | " << board[8];
	cout << "\n\n";
}

char winner(const vector<char>& board)
{
	// все возможные выйгышные ряды
	const int WINNING_ROWS[8][3] = { { 0, 1, 2 },
	{ 3, 4, 5 },
	{ 6, 7, 8 },
	{ 0, 3, 6 },
	{ 1, 4, 7 },
	{ 2, 5, 8 },
	{ 0, 4, 8 },
	{ 2, 4, 6 } };
	const int TOTAL_ROWS = 8;

	// если в одном из выйгрышных рядов уже присутствуют три одинаковых значения (причем они не равны Empty), то победитель определился.
	for (int row = 0; row < TOTAL_ROWS; ++row)
	{
		if ((board[WINNING_ROWS[row][0]] != EMPTY) &&
			(board[WINNING_ROWS[row][0]] == board[WINNING_ROWS[row][1]]) &&
			(board[WINNING_ROWS[row][1]] == board[WINNING_ROWS[row][2]]))
		{
			return board[WINNING_ROWS[row][0]];
		}
	}

	// поскольку победитель не определился, необходимо проверить, не наступила ли ничья (остались ли на поле пустые клетки).
	if (count(board.begin(), board.end(), EMPTY) == 0)
		return TIE;

	// игра продолжается, поскольку (победителя нет, ничья еще не наступила)
	return NO_ONE;
}

inline bool isLegal(int move, const vector<char>& board)
{
	return (board[move] == EMPTY);
}

int humanMove(const vector<char>& board, char human) // данная функция проверяет ход пользователя и предотвращает повторный выбор поля. Включает в себя функцию isLegal в цикле while +1 перегрузка
{
	int move = askNumber("Where will you move?", (board.size() - 1));
	while (!isLegal(move, board)) //пока ход не соответствует легальному ходу (X или O) на поле.
	{
		cout << "\nThat square is already occupied, foolish human.\n";
		move = askNumber("Where will you move?", (board.size() - 1));
	}
	cout << "Fine...\n";

	return move;
}

int computerMove(vector<char> board, char computer)
{
	unsigned int move = 0;
	bool found = false;

	//если компьютер может выйграть следующим ходом, то он делает этот ход
	while (!found && move < board.size())
	{
		if (isLegal(move, board))
		{
			//try move
			board[move] = computer;
			//test for winner
			found = winner(board) == computer;
			//undo move
			board[move] = EMPTY;
		}

		if (!found)
		{
			++move;
		}
	}

	//otherwise, if opponent can win on next move, that's the move to make
	if (!found)
	{
		move = 0;
		char human = opponent(computer);

		while (!found && move < board.size())
		{
			if (isLegal(move, board))
			{
				//try move
				board[move] = human;
				//test for winner
				found = winner(board) == human;
				//undo move
				board[move] = EMPTY;
			}

			if (!found)
			{
				++move;
			}
		}
	}

	//otherwise, moving to the best open square is the move to make
	if (!found)
	{
		move = 0;
		unsigned int i = 0;

		const int BEST_MOVES[] = { 4, 0, 2, 6, 8, 1, 3, 5, 7 };
		//pick best open square
		while (!found && i <  board.size())
		{
			move = BEST_MOVES[i];
			if (isLegal(move, board))
			{
				found = true;
			}

			++i;
		}
	}

	cout << "I shall take square number " << move << endl;
	return move;
}

void announceWinner(char winner, char computer, char human)
{
	if (winner == computer)
	{
		cout << winner << "'s won!\n";
		cout << "As I predicted, human, I am triumphant once more -- proof\n";
		cout << "that computers are superior to humans in all regards.\n";
	}

	else if (winner == human)
	{
		cout << winner << "'s won!\n";
		cout << "No, no!  It cannot be!  Somehow you tricked me, human.\n";
		cout << "But never again!  I, the computer, so swear it!\n";
	}

	else
	{
		cout << "It's a tie.\n";
		cout << "You were most lucky, human, and somehow managed to tie me.\n";
		cout << "Celebrate... for this is the best you will ever achieve.\n";
	}
}
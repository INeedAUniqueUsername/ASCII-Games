#include<iostream>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<string>
using namespace std;
void print(vector< vector<bool> >);
void print(vector< vector<char> >);
void revealTile(vector< vector<bool> > field, vector< vector<char> > &tiles, int x, int y);
bool isValid(vector< vector<bool> > field, int x, int y);
int countSurroundingMines(vector< vector<bool> > field, int x, int y);
int calcScore(vector< vector<bool> > field, vector< vector<char> > tiles);
bool revealMines(vector< vector<bool> > field, vector< vector<char> > &tiles, int x, int y);
void play();
bool playAgain();
int main() {
	do {
		play();
	} while(playAgain());
}
bool playAgain() {
	char c;
	cout << "Play again? [Y/N]" << endl;
	cin >> c;
	return ((c == 'Y') || (c == 'y'));
}
void play() {
	cout << "Linesweeper!" << endl;
	cout << "Explore the minefield, but don't step on the mines!" << endl;
	cout << "You start at a random position on the field. Type in a number on your Numpad to move in a direction." << endl;
	cout << "MOVEMENT" << endl;
	cout	<< "#	Direction" << endl
			<< "1	Down-Left" << endl
			<< "2	Down" << endl
			<< "3	Down-Right" << endl
			<< "4	Left" << endl
			<< "5	Nowhere" << endl
			<< "6	Right" << endl
			<< "7	Up-Left" << endl
			<< "8	Up" << endl
			<< "9	Up-Right" << endl;
	cout << "LEGEND" << endl;
	cout	<< "Char    Meaning" << endl
			<< "-       Unknown" << endl
			<< "X       Mine" << endl
			<< "[#]     Empty space with number of adjacent mines" << endl
			<< ".       Empty space with no adjacent mines" << endl;
	cout << "Your name: ";
	string name;
	cin >> name;
	cout << endl;
	srand(time(0));
	vector< vector<bool> > field;
	
	int height = 32;
	int width = 32;
	
	vector< vector<char> > tiles(height, vector<char>(width, '-'));
	
	for(int y = 0; y < height; y++) {
		vector<bool> fieldRow(width);
		for(int x = 0; x < width; x++) {
			fieldRow[x] = (rand()%5 == 0);
		}
		field.push_back(fieldRow);
	}
	
	/*
	//Reveal all tiles
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			revealTile(field, tiles, x, y);
		}
	}
	*/
	//Initialize player position
	int xStart = rand()%width;
	int yStart = rand()%height;
	
	int x = xStart;
	int y = yStart;
	//Create a clearing around the player
	for(int y2 = y-3; y2 < y+4; y2++) {
		for(int x2 = x-3; x2 < x+4; x2++) {
			if(isValid(field, x2, y2)) {
				field[y2][x2] = false;
			}
		}
	}
	//Reveal the clearing
	revealMines(field, tiles, x, y);
	tiles[y][x] = '@';
	
	bool active = true;
	system("cls");
	print(tiles);
	
	cout << countSurroundingMines(field, x, y) << " adjacent mines" << endl;
	while(active) {
		revealTile(field, tiles, x, y);
		int n;
		cout << "Your move: ";
		cin >> n;
		
		//Clear the screen
		system("cls");
		
		cout << endl;
		//Figure out where to move
		switch(n) {
			case 1:	//Down-Left
				if(!isValid(field, x-1, y-1))
					break;
				x--;
				y--;
				break;
			case 2:	//Down
				if(!isValid(field, x, y-1))
					break;
				y--;
				break;
			case 3:	//Down-Right
				if(!isValid(field, x+1, y-1))
					break;
				x++;
				y--;
				break;
			case 4:	//Left
				if(!isValid(field, x-1, y))
					break;
				x--;
				break;
			case 5:	//Stay
				break;
			case 6:	//Right
				if(!isValid(field, x+1, y))
					break;
				x++;
				break;
			case 7:	//Up-Left
				if(!isValid(field, x-1, y+1))
					break;
				x--;
				y++;
				break;
			case 8:	//Up
				if(!isValid(field, x, y+1))
					break;
				y++;
				break;
			case 9:	//Up-Right
				if(!isValid(field, x+1, y+1))
					break;
				x++;
				y++;
				break;
			default:
				cout << "Invalid move. Use your Number Pad for input." << endl;
				break;
		}
		//Mark this tile as known
		tiles[y][x] = '@';
		if(!field[y][x]) {
			//Mark this position as known
			tiles[y][x] = '@';
			bool revealed = revealMines(field, tiles, x, y);
			//Place the player at this position
			tiles[y][x] = '@';
			print(tiles);
			if(revealed)
				cout << "Mines located." << endl;
			cout << countSurroundingMines(field, x, y) << " adjacent mines" << endl;
		} else {
			//We stepped on a mine
			//Set the 'X' tile onto the field
			revealTile(field, tiles, x, y);
			print(tiles);
			cout << "You died!" << endl;
			cout << "Your score: " << calcScore(field, tiles) << endl;
			cout << "Continue? [Y/N]" << endl;
			char c;
			cin >> c;
			if(c == 'Y' || c == 'y') {
				//Move the player back to start
				x = xStart;
				y = yStart;
				
				tiles[y][x] = '@';
				system("cls");
				print(tiles);
			} else {
				//Exit the loop
				active = false;
			}
			
		}
	}
}
int calcScore(vector< vector<bool> > field, vector< vector<char> > tiles) {
	int score = 0;
	for(int y = 0; y < tiles.size(); y++) {
		for(int x = 0; x < tiles[y].size(); x++) {
			char c = tiles[y][x];
			if(c != '-') {
				score += 1+countSurroundingMines(field, x, y);
			}
		}
	}
	return score;
}

void revealTile(vector< vector<bool> > field, vector< vector<char> > &tiles, int x, int y) {
	if(field[y][x]) {
		tiles[y][x] = 'X';
		return;
	}
	int count = countSurroundingMines(field, x, y);
	if(count > 0) {
		tiles[y][x] = ('0' + count);
	} else {
		tiles[y][x] = '.';
	}
}
//Automatically reveals unknown spaces for the player if enough information is known about surrounding mines
//Return true if any mines were revealed
bool revealMines(vector< vector<bool> > field, vector< vector<char> > &tiles, int x, int y) {
	int mines = countSurroundingMines(field, x, y);
	int knownMines = 0;
	int unknown = 0;
	for(int x2 = x-1; x2 < x+2; x2++) {
		for(int y2 = y-1; y2 < y+2; y2++) {
			if(isValid(field, x2, y2)) {
				if(tiles[y2][x2] == 'X')
					knownMines++;
				else if(tiles[y2][x2] == '-') {
					unknown++;
				}
			}
		}
	}
	
	//Empty-only inference
	if(mines == 0) {
		for(int x2 = x-1; x2 < x+2; x2++) {
			for(int y2 = y-1; y2 < y+2; y2++) {
				if(isValid(field, x2, y2)) {
					revealTile(field, tiles, x2, y2);
				}
			}
		}
		return false;
	}
	
	/*
	//Smart inference
	int unknownMines = (mines - knownMines);
	if(unknownMines == 0 || unknownMines == unknown) {
		for(int x2 = x-1; x2 < x+2; x2++) {
			for(int y2 = y-1; y2 < y+2; y2++) {
				if(isValid(field, x2, y2)) {
					revealTile(field, tiles, x2, y2);
				}
			}
		}
		return (unknownMines > 0);
	}
	*/
	return false;
}
int countSurroundingMines(vector< vector<bool> > field, int x, int y) {
	int count = 0;
	for(int x2 = x-1; x2 < x+2; x2++) {
		for(int y2 = y-1; y2 < y+2; y2++) {
			if(isValid(field, x2, y2)) {
				if(field[y2][x2]) {
					count++;
				}
			}
		}
	}
	return count;
}
bool isValid(vector< vector<bool> > field, int x, int y) {
	return -1 < y && y < field.size() && -1 < x && x < field[y].size();
}
void print(vector< vector<bool> > field) {
	for(int y = 0; y < field.size(); y++) {
		for(int x = 0; x < field[y].size(); x++) {
			cout << (field[y][x] ? '-' : '.');
		}
		cout << endl;
	}
}
void print(vector< vector<char> > tiles) {
	for(int y = tiles.size()-1; y > -1; y--) {
		for(int x = 0; x < tiles[y].size(); x++) {
			cout << tiles[y][x];
		}
		cout << endl;
	}
}

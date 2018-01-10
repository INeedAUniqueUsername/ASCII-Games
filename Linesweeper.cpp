#include<iostream>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<string>
using namespace std;
void print(const vector< vector<bool> > &field);
void print(const vector< vector<char> > &tiles);
void revealTile(const vector< vector<bool> > &field, vector< vector<char> > &tiles, int x, int y);
bool isValid(const vector< vector<bool> > &field, int x, int y);
int countAdjacentMines(const vector< vector<bool> > &field, int x, int y);
int calcScore(const vector< vector<bool> > &field, const vector< vector<char> > &tiles);
bool revealAdjacentMines(const vector< vector<bool> > &field, vector< vector<char> > &tiles, int x, int y);
void play();
bool playAgain();
int main() {
	//Play the game until we decide to quit
	do {
		play();
	} while(playAgain());
}
//Check if we should play again
bool playAgain() {
	char c;
	cout << "Play again? [Y/N]" << endl;
	cin >> c;
	return ((c == 'Y') || (c == 'y'));
}
//Play the game
void play() {
	cout << "Linesweeper!" << endl;
	cout << "Explore the minefield, but don't step on the mines!" << endl;
	cout << "You start at a random position on the field. Type in a number on your Numpad and then press Enter to move in the specified direction." << endl;
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
	
	int height = 32;
	int width = 32;
	
	//Determines which field spaces contain a mine and which do not contain a mine
	vector< vector<bool> > field(height, vector<bool>(width, false));
	
	//Determines how the field is displayed on the screen
	vector< vector<char> > tiles(height, vector<char>(width, '-'));
	
	//Randomly place mines everywhere
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			field.at(y).at(x) = (rand()%5 == 0);
		}
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
				field.at(y2).at(x2) = false;
			}
		}
	}
	
	//Reveal the player's immediate surroundings
	revealAdjacentMines(field, tiles, x, y);
	tiles.at(y).at(x) = '@';
	
	//Clear the screen, print the field, and start the game
	bool active = true;
	system("cls");
	print(tiles);
	
	cout << countAdjacentMines(field, x, y) << " adjacent mines" << endl;
	while(active) {
		//Reveal what's at the player's position (we're going to move the player immediately after)
		revealTile(field, tiles, x, y);
		
		//Get the move
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
				cout << "Invalid move. Must be within 1-9 inclusive." << endl;
				break;
		}
		if(!field.at(y).at(x)) {
			//Mark this position as known
			tiles.at(y).at(x) = '@';
			//If we have Smart Inference enabled, this will reveal adjacent mines
			bool revealed = revealAdjacentMines(field, tiles, x, y);
			//Display the player at this position
			tiles.at(y).at(x) = '@';
			print(tiles);
			if(revealed)
				cout << "Mines located." << endl;
			cout << countAdjacentMines(field, x, y) << " adjacent mines" << endl;
		} else {
			//We stepped on a mine
			//Reveal the X tile
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
				
				tiles.at(y).at(x) = '@';
				system("cls");
				print(tiles);
			} else {
				//Exit the loop
				active = false;
			}
			
		}
	}
}
//For each known tile, we take number of surrounding mines plus one and add that to our score
int calcScore(const vector< vector<bool> > &field, const vector< vector<char> > &tiles) {
	int score = 0;
	for(int y = 0; y < tiles.size(); y++) {
		for(int x = 0; x < tiles[y].size(); x++) {
			char c = tiles.at(y).at(x);
			if(c == 'X') {
				//We know this is a mine (only applies if Smart Inference is enabled)
				score += 10;
			} else if(c != '-') {
				//We know this tile is not a mine
				score += 1+countAdjacentMines(field, x, y);
			}
		}
	}
	return score;
}
//Reveal what's actually on this tile besides the player
void revealTile(const vector< vector<bool> > &field, vector< vector<char> > &tiles, int x, int y) {
	//Check if this is a mine
	if(field.at(y).at(x)) {
		tiles.at(y).at(x) = 'X';
		return;
	}
	int count = countAdjacentMines(field, x, y);
	//Check if there are mines surrounding this tile
	if(count > 0) {
		tiles.at(y).at(x) = ('0' + count);
	} else {
		tiles.at(y).at(x) = '.';
	}
}
//Automatically reveals unknown spaces around this tile if enough information is known about surrounding mines
//Return true if any mines were revealed
bool revealAdjacentMines(const vector< vector<bool> > &field, vector< vector<char> > &tiles, int x, int y) {
	int mines = countAdjacentMines(field, x, y);
	int knownMines = 0;
	int unknown = 0;
	//Check adjacent tiles for mines
	for(int x2 = x-1; x2 < x+2; x2++) {
		for(int y2 = y-1; y2 < y+2; y2++) {
			if(isValid(field, x2, y2)) {
				if(tiles.at(y2).at(x2) == 'X')
					knownMines++;
				else if(tiles.at(y2).at(x2) == '-') {
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
	//Smart Inference (applies basic Minesweeper strategies automatically)
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
//Count the number of mines surrounding (immediately adjacent to) this tile
int countAdjacentMines(const vector< vector<bool> > &field, int x, int y) {
	int count = 0;
	for(int x2 = x-1; x2 < x+2; x2++) {
		for(int y2 = y-1; y2 < y+2; y2++) {
			if(isValid(field, x2, y2)) {
				if(field.at(y2).at(x2)) {
					count++;
				}
			}
		}
	}
	return count;
}
//Check if this position exists on the field
bool isValid(const vector< vector<bool> > &field, int x, int y) {
	return -1 < y && y < field.size() && -1 < x && x < field[y].size();
}
//Print the field to console
void print(const vector< vector<bool> > &field) {
	for(int y = 0; y < field.size(); y++) {
		for(int x = 0; x < field[y].size(); x++) {
			cout << (field.at(y).at(x) ? 'X' : '.');
		}
		cout << endl;
	}
}
//Print the tiles to console
void print(const vector< vector<char> > &tiles) {
	for(int y = tiles.size()-1; y > -1; y--) {
		for(int x = 0; x < tiles[y].size(); x++) {
			cout << tiles.at(y).at(x);
		}
		cout << endl;
	}
}

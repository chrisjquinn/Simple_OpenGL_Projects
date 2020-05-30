#ifdef __APPLE__
//Supress warnings about OpenGL on apple machines
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif 

//For drawing text instructions
#include "draw_text.h" 


#include <stddef.h>
#include <iostream>


/*										CONSTANTS USED THROUGHOUT										*/
const int ROWS = 20;
const int COLS = 10;

const int BLOCKSIZE = 30;
//Size of the tunnel in which the tetrominos fall
const int TUNWIDTH = COLS * BLOCKSIZE;
const int TUNHEIGHT = ROWS * BLOCKSIZE;
//Size of the window that will include the tunnel and all the other crao
const int VPWIDTH = TUNWIDTH + 100;
const int VPHEIGHT = TUNHEIGHT;

//The 4 orientations of blocks
const int blockDim = 4;
//How many types of blocks allowed in the game
const int numBlocks = 7;



/* 												TETROMINO												*/
/*		Tetris V2 is removing class tetromino and just keeping the fat array of all the possible blocks */
int Tetrominos[numBlocks+1][blockDim][blockDim][blockDim] = 
{ //7 types of block, 4 orientations, each represented by a 4x4 binary array
	{// Empty (we dont want a piece to choose block zero as this is resevred for an empty space)
		{0,0,0,0,
		 0,0,0,0,
		 0,0,0,0,
		 0,0,0,0}, 

		{0,0,0,0,
		 0,0,0,0,
		 0,0,0,0,
		 0,0,0,0},

		{0,0,0,0,
		 0,0,0,0,
		 0,0,0,0,
		 0,0,0,0},

		{0,0,0,0,
		 0,0,0,0,
		 0,0,0,0,
		 0,0,0,0},
	},
	{//Square
		{0,1,1,0,
		 0,1,1,0,
		 0,0,0,0,
		 0,0,0,0}, 

		{0,1,1,0,
		 0,1,1,0,
		 0,0,0,0,
		 0,0,0,0},

		{0,1,1,0,
		 0,1,1,0,
		 0,0,0,0,
		 0,0,0,0},

		{0,1,1,0,
		 0,1,1,0,
		 0,0,0,0,
		 0,0,0,0},
	},
	{//Long I 
		{1,0,0,0,
		 1,0,0,0,
		 1,0,0,0,
		 1,0,0,0},

		{1,1,1,1,
		 0,0,0,0,
		 0,0,0,0,
		 0,0,0,0},

		{1,0,0,0,
		 1,0,0,0,
		 1,0,0,0,
		 1,0,0,0},

		{1,1,1,1,
		 0,0,0,0,
		 0,0,0,0,
		 0,0,0,0},
	},
	{// T 
		{1,1,1,0,
		 0,1,0,0,
		 0,0,0,0,
		 0,0,0,0},

		{0,1,0,0,
		 1,1,0,0,
		 0,1,0,0,
		 0,0,0,0},

		{0,1,0,0,
		 1,1,1,0,
		 0,0,0,0,
		 0,0,0,0},

		{1,0,0,0,
		 1,1,0,0,
		 1,0,0,0,
		 0,0,0,0},

	},
	{// L
		{1,0,0,0,
		 1,0,0,0,
		 1,1,0,0,
		 0,0,0,0},

		{1,1,1,0,
		 1,0,0,0,
		 0,0,0,0,
		 0,0,0,0},

		{1,1,0,0,
		 0,1,0,0,
		 0,1,0,0,
		 0,0,0,0},

		{0,0,1,0,
		 1,1,1,0,
		 0,0,0,0,
		 0,0,0,0},

	},
	{//J (Reverse L)
		{0,1,0,0,
		 0,1,0,0,
		 1,1,0,0,
		 0,0,0,0},

		{1,0,0,0,
		 1,1,1,0,
		 0,0,0,0,
		 0,0,0,0},

		{1,1,0,0,
		 1,0,0,0,
		 1,0,0,0,
		 0,0,0,0},

		{1,1,1,0,
		 0,0,1,0,
		 0,0,0,0,
		 0,0,0,0},

	},
	{// S
		{0,1,0,0,
		 1,1,0,0,
		 1,0,0,0,
		 0,0,0,0},

		{1,1,0,0,
		 0,1,1,0,
		 0,0,0,0,
		 0,0,0,0},

		{0,1,0,0,
		 1,1,0,0,
		 1,0,0,0,
		 0,0,0,0},

		{1,1,0,0,
		 0,1,1,0,
		 0,0,0,0,
		 0,0,0,0},

	},
	{// Z (Reverse S)
		{1,0,0,0,
		 1,1,0,0,
		 0,1,0,0,
		 0,0,0,0},

		{0,1,1,0,
		 1,1,0,0,
		 0,0,0,0,
		 0,0,0,0},

		{1,0,0,0,
		 1,1,0,0,
		 0,1,0,0,
		 0,0,0,0},

		{0,1,1,0,
		 1,1,0,0,
		 0,0,0,0,
		 0,0,0,0},
	}
};

//Above needed for making the new arrays.


/* 											GAME CLASS 										*/


class Game{
	/*	Variables of class game:
	*
	*	grid[ROWS][COLS]		This is a 2D Array that stores all of the blocks in tetris, it does not store the current block being played
	*	currentPiece			This is an integer as the shape corresponding to that integer can be found in the array above
	*	nextPiece				The same as currentPiece, but will be the next piece to be called
	*	orientation 			An integer from 0 to 3 representing which position the block is in
	*	col 					The current column the bottom left of the 4x4 array representing the piece is in
	*	row 					The current row the bottom left of the 4x4 array representing the piece is in
	*	linesCleared 			A counter of how many lines have been cleared by the player
	*	rowToClear 				An integer representing what row needs to be set to empty and then shifted along
	*	
	*	killed					If the player has a game over or not
	*	paused 					If the game is currently paused
	* 	isRowCleared 			For checking if a row has been successfully shifted and the game can continue
	*	dropFaster				When the user wishes to speed up the round of the current piece, dropFaster allows the main logic to determine when to go back to normal speed
	*
	*	timer 					An integer representing the amount of miliseconds to wait before moving the block down one row
	*/

	public:
		int grid[ROWS][COLS];
		int currentPiece;
		int nextPiece;
		int orientation;
		int col;
		int row;
		int linesCleared;
		int rowToClear;

		bool killed;
		bool paused;
		bool isRowCleared;
		bool dropFaster;


		int timer;

	//Game constructor for when it is called later at the end of the class
	Game() {
		currentPiece = rand()%7 + 1; //Between 1 and 8 as 0 is reserved for empty cell
		nextPiece = rand()%7 + 1;
		Restart();
	}

	// Start the game again
	void Restart(){
		ClearGrid(true);
		linesCleared = 0;
		killed = false;
		paused = false;
		dropFaster = false;
	}

	//For when we want to clear the whole grid or just one row
	void ClearGrid(bool wholeGrid){
		if (wholeGrid){
			for(int i = 0; i < ROWS; i++){
				for (int j = 0; j < COLS; j++){
					grid[i][j] = 0; //we set to empty
				}
			}
		}
		else{
			int rowChecker = 0;
			for(int r = 0; r < ROWS; r++){
				for (int c = 0; c < COLS; c++){
					if (grid[r][c] != 0){
						rowChecker++;
					}
				}
				if (rowChecker ==(ROWS)){
					rowToClear = r;
					isRowCleared = true;
				}
			}
		}
	}

	//Shift the rows down when it is filled up
	void ClearRow(){
		linesCleared++;
		for(int c = 0; c < COLS; c++){
			grid[rowToClear][c] = 0;
		}
		int tempGrid[ROWS][COLS];
		for(int r = rowToClear; r < ROWS - 1; r++){
			for(int c = 0; c < COLS; c++){
				// Need to introduce a new temp grid?

				grid[r][c] = grid[r+1][c];
				ClearGrid(false);
			}
		}
	}

	//This will put the piece into the grid array when it has fallen and cannot move any more
	void putPiece(){
		//Have to painstakingly assign the piece from the big array as so
		int piece[4][4];
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				piece[i][j] = Tetrominos[currentPiece][orientation][i][j];
			}
		}
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				if(piece[i][j] == 1){
					//we need not to -1 after adding i and j because of both starting at zero
					grid[row+j][col+i] = currentPiece; // I AND J DIFFERENT HERE
				}
			}
		} 
	}

	//When a piece has fallen we need to switch to the next block.
	void switchBlock(){
		timer = 500;
		putPiece();
		ClearGrid(false);
		checkDeath();
		currentPiece = nextPiece;
		row  = ROWS -1;
		col = COLS / 2;
		orientation = 0;
		dropFaster = false;
		nextPiece = (rand() % 7 + 1); //Get a new next piece
	}

	//The game ends when the blocks stack up and occupy the space where blocks are generated. Thus game over.
	void checkDeath(){
		if(grid[ROWS-1][COLS/2] != 0){
			killed = true;
		}
	}

	//This method allows a block to rotate, provided the move doesnt cause any collisions
	void Move(int dir, int rotate){
		if(dir < 0 && !CheckCollision(false,true,false))
			col--;
		if (dir > 0 && !CheckCollision(false,false,false))
			col++;
		if(rotate > 0 && !CheckCollision(false,false,true))
			orientation = (orientation + 1) % 4;
	}

	/* Big method in checking if a block can fall down one, move to the left, right or rotate when the user wants.
	*  
	*  (rotate)
	*  This first checks if the user wants to rotate and then brings up the piece that is in play, but in the new rotation.
	*  Then, it will make sure the block will not rotate into non-empty cells or out of the grid
	*
	*  (below)
	*  Needed not for when the user wants to move the block, but when time has elapsed and the block needs to move down one row.
	*  This will check if the block cannot move down one row due to the cells beneth being non-empty, or that we have hit the 
	*  bottom of the grid
	*
	*  (left)
	*  like below, checks if the grid is non-empty or if the move will push the block out of the grid. (Right is treated the same
	*  and will take in !left)
	*
	*/

	bool CheckCollision(bool below, bool left, bool rotate){
		if(rotate){
			int piece[4][4];
			for(int i = 0; i < 4; i++){
				for(int j = 0; j < 4; j++){
					piece[i][j] = Tetrominos[currentPiece][(orientation + 1) % 4][i][j];
				}
			}
			for(int i = 0; i < 4; i++){
				for(int j = 0; j < 4; j++){
					if((piece[i][j] == 1) && ((grid[row + i][col + j] != 0) || (col + j > COLS) || (col + j < 0)))
						return true;
				}
			}
		}
		else { //we arent testing for a rotation
			int piece[4][4];
			for(int i = 0; i < 4; i++){
				for(int j = 0; j < 4; j++){
					piece[i][j] = Tetrominos[currentPiece][orientation][i][j];
				}
			}
			for (int i = 0; i < 4; i++){
				for (int j = 0; j < 4; j++){
					if(piece[i][j] == 1){
						if(below){
							if((row == 0) || (grid[row + i -1][col + j] != 0)){ //we are at the bottom of the grid, cant go any further
								return true;
							}
						}
						else if (left){
							if((col + j == 0) || grid[row+i][col+j-1] != 0){
								return true;
							}
						}
						else if (!left){ //want to go right
							if((COLS - 1 - (col + i) <= 0 || grid[row+i][col+j+1] != 0)) { //THIS MAY NEED ADJUSTING
								return true;
							}
						}
					}
				}
			}	
		}
		return false;
	}

	//Prints the grid out onto terminal, used alot in debug
	void printGrid(){
		std::cout << "[" << std::endl;
		for (int i = 0; i < ROWS; i++){
			for(int j = 0; j < COLS; j++){
				std::cout << grid[i][j] << ",";
				if(j == COLS-1){
					std::cout << std::endl;
				}
			}
		}
		std::cout << "]" ;
	}

//End of the game class
};


Game game; //Create an instance of the game for use in the methods below of tetris




/*								MAIN CLASS FOR THE TETRIS 								*/

/*Instead of calling glColor3f all the time and having to guess what it is, I simply made this method that will 
 *Change the colour of the whole pipeline, with 0 reserved for empty cells and 9 for the buffer swap / grid lines
 */
void SetColour(int x){
	switch(x) {
		case 0: 	glColor3f(0.0,0.0,0.0); break; //Black
		case 1:		glColor3f(1.0,1.0,0.0); break; //Yellow
		case 2:		glColor3f(0.0,1.0,1.0); break; //Cyan
		case 3: 	glColor3f(0.5,0.3,0.3); break;
		case 4: 	glColor3f(0.0,0.0,1.0); break;
		case 5:		glColor3f(1.0,0.5,0.0); break;
		case 6:		glColor3f(0.0,1.0,0.0); break;
		case 7: 	glColor3f(1.0,0.0,0.0); break;
		case 8: 	glColor3f(0.5,0.0,0.5); break;
		case 9: 	glColor3f(1.0,1.0,1.0); break; //White
	}
}

//This handles the timing of the block falling depending on what state the game is in. Normally, the game will just 
//Fall into the game.row -= 1 where the block is falling.
void Timer(int id){ 
	if(!game.paused){
		if(game.killed || game.isRowCleared){
			if(game.isRowCleared){
				game.isRowCleared = false;
				game.ClearRow();
			}
			else if (game.killed){
				game.Restart();
			}
			glutTimerFunc(game.timer,Timer,0);
			glutPostRedisplay();
		}
		else{
			if(game.killed){
				glutTimerFunc(100,Timer,0);
			}
			else{
				glutTimerFunc(game.timer,Timer,0);
				if(game.CheckCollision(true,false,false)){
					game.switchBlock();
				}
				else if (!game.CheckCollision(true,false,false)){
					game.row -= 1;
				}
				else if (game.isRowCleared){
					glutTimerFunc(100,Timer,0);
				}
			}
		glutPostRedisplay();
		}
	}
}

//Standard Init() used from the labs, but here I have also put the start of the projection so that I don't
//have to call this a lot of times in the display() function


void init(){
	glLineWidth(2.0);
	glClearColor(255,255,255,0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//Simply pauses the game still, nothing is displayed
void pause(){
	init();
	glutSwapBuffers();
}

/*	Handles all keyboard events, three types. 
*	p - pauses the game
*	q - quits the game
*	space - throws the block in play to the bottom of the grid
*
*/
void keyboard(unsigned char key, int x, int y){
	switch(key){
		case 'q': exit(1); break;
		case 'p': 
			game.paused = !game.paused;
			pause();
			if(!game.paused){
				glutTimerFunc(game.timer,Timer,0);
				glutSwapBuffers();
			}
			break;
		case ' ': game.timer = 0; break;
	}
}

/* 	Handles all arrow key presses from the user, effect the block in play
*	left - moves the block in play left
*	right - moves the block in play right
*	up - rotates the block in play by 90 degrees
*	down - increases the speed in which the block falls
*/

void special(int key, int x, int y){
	if(!game.paused && !game.killed){
		switch(key){
			case GLUT_KEY_LEFT: game.Move(-1,0); break;
			case GLUT_KEY_RIGHT: game.Move(1,0); break;
			case GLUT_KEY_UP: game.Move(0,1); break;
			case GLUT_KEY_DOWN: 
				game.dropFaster = !game.dropFaster;
				if(game.dropFaster){
					game.timer = 100;
				}
				else {
					game.timer = 500; 
				}
				break;
		}
	}
}


/*											DRAWING FUNCTIONS									*/

/*	drawOutline() Takes in co-ordinates and draws an empty rectangle outline in black
*	This is mainly used for the blocks that have fallen and the current piece in play
*/

void drawOutline(int x, int y){
	SetColour(0); //Black
	glBegin(GL_LINE_LOOP);
		glVertex2d(x,y);
		glVertex2d(x,y+1);
		glVertex2d(x+1,y+1);
		glVertex2d(x+1,y);
	glEnd();
}

/*	displayTetromino() Takes in co-ordinates like drawOutline but instead creates 
*	a full colour rectangle, each piece is made up of 4 of these little blocks of colour
*	and can easily be found by referencing the big array of all the shapes possible.
*	
*	The blocks colour can be found by the currentPiece variable for the game, as it represents
*	not only what shape it is but what color too.
*/
void displayTetromino(int x, int y){
	int piece[4][4];
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			piece[i][j] = Tetrominos[game.currentPiece][game.orientation][i][j];
		}
	}
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(piece[i][j] == 1){
				SetColour(game.currentPiece);
				glRectd(x+i, y+j, x+1+i, y+1+j);
				drawOutline(x+i,y+j);
			}
		}
	}
}

/* 	displayNextTetromino() Is exactly the same as displayTetromino
*	but focuses on the next piece only.
*/

void displayNextTetromino(int x, int y){
	int piece[4][4];
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			piece[i][j] = Tetrominos[game.nextPiece][game.orientation][i][j];
		}
	}
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(piece[i][j] == 1){
				SetColour(game.nextPiece);
				glRectd(x+i, y+j, x+1+i, y+1+j);
				drawOutline(x+i,y+j);
			}
		}
	}
}

/*	displaygrid() similar to the two previous drawing functions but draws only the blocks 
*	that have been put into the grid after falling. Their colour is found by just quering the grid
*	as 0 is empty and all other numbers represent colours. The specific shapes do not matter anymore.
*/
void displayGrid() {
	for(int r = 0; r < ROWS; r++){
		for(int c = 0; c < COLS; c++){
			SetColour(game.grid[r][c]);
			glRectd(c,r,c+1,r+1);
			drawOutline(c,r);
		}
	}
	//Now to draw the grid lines
	SetColour(9);
	glBegin(GL_LINES);
		for(int c = 0; c < COLS + 1; c++){
			glVertex2d(c, 0);
			glVertex2d(c, ROWS);
		}
		for(int r = 0; r < ROWS + 1; r++){
			glVertex2d(0,r);
			glVertex2d(COLS,r);
		}
	glEnd();
}

/*	display() -	Main rendering of the visual window the user deals with. 
*	
*	This first initialises and clears the buffer, then it sets up the 2D orthogonal projection (same as 3D with near 1 and far -1)
*	
*	Next, all text is drawn onto the screen, this is taken from lab5 - robot arm. The colour has been changed and a parameter for scale has been added.
*	This creates the difference in size for the "Controls" and its text underneath.
*
*	After writing all text to the window, the grid is rendered then the two inidividual blocks of the current piece in play
*	and the next piece that is going to appear.
*
*	Finally, we swap the buffers and fill the rest in white.
*
*
*/
void display() {
	init();
	glClear(GL_COLOR_BUFFER_BIT);

	//BEGIN PROJECTION - -10 and -3 emperically found as co-ords for the tunnel is (0,0) to (ROWS,COLS)

	gluOrtho2D(-10,COLS,-3,ROWS);

	// WRITE ALL TEXT TO THE SCREEN

	draw_text(30,880,"TETRIS",1.0);
	draw_text(90,700,"Next Piece:",0.5);
	draw_text(130,200,"Controls:", 0.5);
	draw_text(10,170, "Move Left: Left Arrow Key", 0.2);
	draw_text(10,130, "Move Right: Right Arrow Key", 0.2);
	draw_text(10,90, "Rotate: Up Arrow Key", 0.2);
	draw_text(10,50, "Move Down Quicker: Down Arrow Key", 0.2);
	draw_text(10,10, "Throw to bottom: Space Bar", 0.2);
	
	//Show all the blocks that have fallen and stay there.
	displayGrid();

	//Draw the current piece being played
	displayTetromino(game.col,game.row);
	//Draw the next Piece window next to the tunnel
	displayNextTetromino(-6,9);
	//Fill the rest of the buffer with white, done.
	glutSwapBuffers();

}


//Main entry point of tetris and sets up the user input functions to be handled in the pipeline
int main(int argc, char* argv[]) {
	srand(time(NULL)); //start of the pseudo random numbers for whenever rand() is called

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Tetris 1711901");

	glutDisplayFunc(display);
	glutTimerFunc(game.timer, Timer, 0);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);


	glutMainLoop();

	return 0;
}






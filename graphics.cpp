#include "graphics.h"
#include "circle.h"
#include "button.h"
#include "rect.h"
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

GLdouble width, height;
int wd;
bool drawing = true;
bool clicked = false;
bool houseTemp = false;
bool clearScreen = false;
enum screens {first, second};
screens view = first;
enum thickness {thin, medium, thick};
enum penColors {purplePen, bluePen, redPen, greenPen, blackPen};
penColors penColor = blackPen;
thickness pen = medium;
const color skyBlue(77/255.0, 213/255.0, 240/255.0);
const color grassGreen(26/255.0, 176/255.0, 56/255.0);
const color white(1, 1, 1);
const color brickRed(201/255.0, 20/255.0, 20/255.0);
const color darkBlue(1/255.0, 110/255.0, 214/255.0);
const color purple(119/255.0, 11/255.0, 224/255.0);
const color black(0, 0, 0);
const color magenta(1, 0, 1);
const color orange(1, 163/255.0, 22/255.0);
const color cyan (0, 1, 1);

vector<Rect> redRectangles;
Rect user;
Circle leftKnob;
Circle rightKnob;
vector<Rect> whiteBoard;
vector<Rect>house;

Button drawButton({211/255.0, 211/255.0, 211/255.0}, {50, 50}, 40, 50, "DRAW");
Button thickness1({111/255.0, 111/255.0, 111/255.0}, {50, 40}, 165, 50, "THIN");
Button thickness2({111/255.0, 111/255.0, 111/255.0}, {50, 40}, 225, 50, "MEDIUM");
Button thickness3({111/255.0, 111/255.0, 111/255.0}, {50, 40}, 285, 50, "THICK");
Button purpleButton({119/255.0, 11/255.0, 224/255.0}, {25, 25}, 350, 50, "");
Button blueButton({77/255.0, 213/255.0, 240/255.0}, {25, 25}, 385, 50, "");
Button redButton({1, 0, 0}, {25, 25}, 420, 50, "");
Button greenButton({0, 1, 0}, {25, 25}, 455, 50, "");
Button blackButton({0, 0, 0}, {25, 25}, 490, 50, "");
Button shakeButton({211/255.0, 211/255.0, 211/255.0}, {50, 50}, 565, 50, "SHAKE");
Button templateButton({211/255.0, 211/255.0, 211/255.0}, {50, 50}, 105, 50, "GUIDE");
Button exitButton({211/255.0, 211/255.0, 211/255.0}, {50, 50}, 635, 50, "EXIT");

// inits the general outline of the etch-a-sketch
void initLayout() {
    // vertical border
    dimensions rectSize1;
    rectSize1.height = 500;
    rectSize1.width = 75;

    // horizontal border
    dimensions rectSize2;
    rectSize2.height = 75;
    rectSize2.width = 700;

    // build the outer boarder for the etch-a-sketch
    redRectangles.push_back(Rect(brickRed,rectSize1.width/2, height/2+50,rectSize1));
    redRectangles.push_back(Rect(brickRed,width-(rectSize1.width/2), height/2+50,rectSize1));
    redRectangles.push_back(Rect(brickRed,width/2, height-(rectSize2.height/2),rectSize2));
    redRectangles.push_back(Rect(brickRed,width/2, (rectSize2.height/2)+100,rectSize2));

    // create knobs for lower left and right corners
    leftKnob.setCenter(50,655);
    leftKnob.setRadius(30);
    leftKnob.setColor(white);

    rightKnob.setCenter(650,655);
    rightKnob.setRadius(30);
    rightKnob.setColor(white);
}

// init the white board which is made up of small rectangles
void initWhiteBoard(){

    int startSquareY = 620;
    dimensions squareSize;
    bool odd = true;

    // fill in the rectangle inside the borders with small white squares
    while(startSquareY >= 180){
        int startSquareX = 80;
        while (startSquareX <= 620) {
            squareSize.height = 5;
            squareSize.width = 5;
            if (odd == true){
                whiteBoard.push_back(Rect(white,startSquareX,startSquareY,squareSize));
                startSquareX += 2.5;
                odd = false;
            }else{
                whiteBoard.push_back(Rect(white,startSquareX,startSquareY,squareSize));
                startSquareX += 2.5;
                odd = true;
            }

        }
        startSquareY -= 5;
    }
}

void initUser() {
    // set user to be a small black square
    user.setCenter(20,20 );
    user.setSize(10, 10);
    user.setColor(black);
}

// init the template that draws a picture that the user can color in
void initTemplate(){
    // creates the sides of the house
    dimensions sideHouse(10,200);
    house.push_back(Rect(black,150,525,sideHouse));
    house.push_back(Rect(black,350,525,sideHouse));

    // creates the door
    dimensions doorHouse(10,75);
    house.push_back(Rect(black,200,590,doorHouse));
    house.push_back(Rect(black,250,590,doorHouse));

    dimensions topDoorHouse(60,10);
    house.push_back(Rect(black,225,550,topDoorHouse));

    // uses a series of rectangles to draw the roof
    dimensions roofHouse(10,30);
    int startRoofx = 125;
    int startRoofy =450;
    while(startRoofx < 250 && startRoofy >250){
        Rect roof(black,startRoofx,startRoofy,roofHouse);
        startRoofx+= 10;
        startRoofy -= 10;
        house.push_back(roof);
    }

    int startRoofx2 = 375;
    int startRoofy2 =450;
    while(startRoofx2 > 250 && startRoofy2 >250){
        Rect roof(black,startRoofx2,startRoofy2,roofHouse);
        startRoofx2-= 10;
        startRoofy2 -= 10;
        house.push_back(roof);
    }

    // creates the doorknob
    dimensions doorKnobHouse(10,10);
    Rect doorKnob(black,230,590,doorKnobHouse);
    house.push_back(doorKnob);

    // create the top of the house
    dimensions topHouse(200,10);
    house.push_back(Rect(black,250,440,topHouse));

    // creates the tree trunk
    dimensions treeTrunk(10,78);
    house.push_back(Rect(black,475,590,treeTrunk));
    house.push_back(Rect(black,525,590,treeTrunk));

}

// inits the initial layout
void init() {
    width = 700;
    height = 700;
    srand(time(0));
    initLayout();
    initUser();
    initWhiteBoard();
    initTemplate();
}


/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(white.red, white.green, white.blue, 1.0f);
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // Tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height); // DO NOT CHANGE THIS LINE (unless you are running Catalina on Mac)

    // Do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION); // DO NOT CHANGE THIS LINE
    glLoadIdentity(); // DO NOT CHANGE THIS LINE
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f); // DO NOT CHANGE THIS LINE

    // Clear the color buffer with current clearing color
    glClear(GL_COLOR_BUFFER_BIT); // DO NOT CHANGE THIS LINE

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // DO NOT CHANGE THIS LINE

    // if the user has not clicked the template button, draw all the rectangles and buttons
    if(view == first){
        for (Rect &r : redRectangles) {
            r.draw();
        }
        for (Rect &w : whiteBoard) {
            w.draw();
        }
        leftKnob.draw();
        rightKnob.draw();
        user.draw();
        drawButton.draw();
        thickness1.draw();
        thickness2.draw();
        thickness3.draw();
        purpleButton.draw();
        blueButton.draw();
        redButton.draw();
        greenButton.draw();
        blackButton.draw();
        shakeButton.draw();
        exitButton.draw();
        templateButton.draw();

        // if the user presses the draw button
        if (drawing){
            // change the pen color depending on what button the user presses
            switch(penColor){
                case purplePen:
                    for (Rect &w : whiteBoard) {
                        if (w.isOverlapping(user)&& clicked ==true) {
                            w.setColor(purple);
                        }
                    }
                    break;

                case blackPen:
                    for (Rect &w : whiteBoard) {
                        if (w.isOverlapping(user)&& clicked ==true) {
                            w.setColor(black);
                        }
                    }
                    break;

                case bluePen:
                    for (Rect &w : whiteBoard) {
                        if (w.isOverlapping(user)&& clicked ==true) {
                            w.setColor(skyBlue);
                        }
                    }
                    break;

                case redPen:
                    for (Rect &w : whiteBoard) {
                        if (w.isOverlapping(user)&& clicked ==true) {
                            w.setColor(brickRed);
                        }
                    }
                    break;

                case greenPen:
                    for (Rect &w : whiteBoard) {
                        if (w.isOverlapping(user)&& clicked ==true) {
                            w.setColor(grassGreen);
                        }
                    }
                    break;
            }

        // if they are not drawing, whiteboard is clear
        }else{
            for (Rect &w : whiteBoard) {
                w.setColor(white);
            }
        }

        // change the pen size depending on what button the user presses
        if(pen == thin){
            user.setSize(2, 2);
        }
        if(pen == medium){
            user.setSize(10, 10);
        }
        if(pen == thick){
            user.setSize(20, 20);
        }

        // if the template button is pressed, draw the picture of the house and tree
        if(houseTemp){
            for (Rect &h : house) {
                h.draw();
            }

            glBegin(GL_TRIANGLES);
            glColor3f(26/255.0, 176/255.0, 56/255.0);
            glVertex2i(500, 350);
            glVertex2i(450, 400);
            glVertex2i(550, 400);
            glEnd();

            glBegin(GL_TRIANGLES);
            glColor3f(26/255.0, 176/255.0, 56/255.0);
            glVertex2i(500, 375);
            glVertex2i(400, 475);
            glVertex2i(600, 475);
            glEnd();

            glBegin(GL_TRIANGLES);
            glColor3f(26/255.0, 176/255.0, 56/255.0);
            glVertex2i(500, 450);
            glVertex2i(375, 550);
            glVertex2i(625, 550);
            glEnd();

        }

        // if the user wants to clear screen, clear everything and redraw window
        if(clearScreen){
            glClear(GL_COLOR_BUFFER_BIT); // DO NOT CHANGE THIS LINE
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // DO NOT CHANGE THIS LINE

            for (Rect &r : redRectangles) {
                r.draw();
            }
            for (Rect &w : whiteBoard) {
                w.draw();
            }
            leftKnob.draw();
            rightKnob.draw();
            user.draw();
            drawButton.draw();
            thickness1.draw();
            thickness2.draw();
            thickness3.draw();
            purpleButton.draw();
            blueButton.draw();
            redButton.draw();
            greenButton.draw();
            blackButton.draw();
            shakeButton.draw();
            templateButton.draw();
            exitButton.draw();
        }
        glFlush();  // Render now
    }

    // if the user presses the exit button, a new window is displayed and user input is required
    if(view == second){
        glutPostRedisplay();
        string words = "Are you sure you want to exit (Press Y or N)? ";
        glRasterPos2i(160,height/2);
        for (const char &letter : words) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }
        glFlush();
    }

}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y) {
    // escape
    if (key == 27) {
        glutDestroyWindow(wd);
        exit(0);
    }
    // destroy window is user wants to exit
    if (key == 'y') {
        glutDestroyWindow(wd);
        exit(0);
    }
    // if user does not want to exit, go back to first display
    if(key == 'n'){
        view = first;
    }

    glutPostRedisplay();
}

void kbdS(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_DOWN:

            break;
        case GLUT_KEY_LEFT:

            break;
        case GLUT_KEY_RIGHT:

            break;
        case GLUT_KEY_UP:

            break;
    }

    glutPostRedisplay();
}

void cursor(int x, int y) {
    // hover and release buttons if user overlaps cursor with the button
    user.setCenter(x,y);
    if (drawButton.isOverlapping(user)){
        drawButton.hover();
    }
    else{
        drawButton.release();
    }

    if (templateButton.isOverlapping(user)){
        templateButton.hover();
    }
    else{
        templateButton.release();
    }

    if (thickness1.isOverlapping(user)){
        thickness1.hover();
    }
    else{
       thickness1.release();
    }

    if (thickness2.isOverlapping(user)){
        thickness2.hover();
    }
    else{
        thickness2.release();
    }

    if (thickness3.isOverlapping(user)){
        thickness3.hover();
    }
    else{
        thickness3.release();
    }

    if (shakeButton.isOverlapping(user)){
        shakeButton.hover();
    }
    else{
        shakeButton.release();
    }
    if (exitButton.isOverlapping(user)){
        exitButton.hover();
    }
    else{
        exitButton.release();
    }

    glutPostRedisplay();
}


void mouse(int button, int state, int x, int y) {
    // if user is presses draw button, change it's color and set drawing to true
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && drawButton.isOverlapping(user)){
        drawButton.pressDown();
        drawing = true;
    }else {
        drawButton.release();
    }
    // for each rectangle in the whiteboard, if the user is overlappping it, set clicked to true
    for (Rect &w : whiteBoard) {
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && w.isOverlapping(user)) {
           clicked = true;
        }
    }

    //if user presses the thin button, set pen to thin
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && thickness1.isOverlapping(user)) {
        thickness1.pressDown();
        thickness2.release();
        thickness3.release();
        pen = thin;
    }

    //if user presses the medium pen button, set pen to medium
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && thickness2.isOverlapping(user)) {
        thickness2.pressDown();
        thickness1.release();
        thickness3.release();
        pen = medium;
    }

    //if user presses the thick button, set pen to thick
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && thickness3.isOverlapping(user)){
        thickness3.pressDown();
        thickness1.release();
        thickness2.release();
        pen = thick;
    }

    // change pen color based on what button user clicks
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && purpleButton.isOverlapping(user)){
        purpleButton.pressDown();
        blackButton.release();
        blueButton.release();
        greenButton.release();
        redButton.release();
        penColor = purplePen;
    }
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && blueButton.isOverlapping(user)){
        blueButton.pressDown();
        purpleButton.release();
        blackButton.release();
        greenButton.release();
        redButton.release();
        penColor = bluePen;
    }
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && blackButton.isOverlapping(user)){
        blackButton.pressDown();
        purpleButton.release();
        blueButton.release();
        greenButton.release();
        redButton.release();
        penColor = blackPen;
    }
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && greenButton.isOverlapping(user)){
        greenButton.pressDown();
        purpleButton.release();
        blackButton.release();
        blueButton.release();
        redButton.release();
        penColor = greenPen;
    }
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && redButton.isOverlapping(user)){
        redButton.pressDown();
        purpleButton.release();
        blackButton.release();
        blueButton.release();
        greenButton.release();
        penColor = redPen;
    }

    // if user presses the shake button, set drawing to false which will clear the screen
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && shakeButton.isOverlapping(user)){
        drawing= false;
    }

    // if user presses the shake button and the template is displayed,
    // clear the template and the screen
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && shakeButton.isOverlapping(user)&& houseTemp == true){
        houseTemp = false;
        clearScreen = true;
    }

    // if user presses the template button, set houseTemp to true which will draw the pic
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && templateButton.isOverlapping(user)){
       houseTemp = true;
       templateButton.pressDown();
    }

    // if user presses the exit button, set view to second which will prompt for user input
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && exitButton.isOverlapping(user)){
       exitButton.pressDown();
       view = second;
    }

    glutPostRedisplay();
}


/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

    init();

    glutInit(&argc, argv);          // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(100, 200); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Janey's Etch-a-Sketch" /* title */ );

    // Register callback handler for window re-paint event
    glutDisplayFunc(display);

    // Our own OpenGL initialization
    initGL();

    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);

    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);

    // handles mouse movement
    glutPassiveMotionFunc(cursor);

    // handles mouse click
    glutMouseFunc(mouse);

    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}

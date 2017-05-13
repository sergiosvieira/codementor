// lab2: rectangles
// <insert your name here>
// read main.cpp, and follow the instructions at the bottom of main.cpp

#define NOMINMAX // prevent Windows API from conflicting with "min" and "max"

#include <stdio.h>   // C-style output. printf(char*,...), putchar(int)
#include <windows.h> // SetConsoleCursorPosition(HANDLE,COORD)
#include <conio.h>   // _getch()
#include <iostream>
#include <stringstream>
#include <time.h>
#include <stdlib.h>



/**
 * moves the console cursor to the given x/y coordinate
 * 0, 0 is the upper-left hand coordinate. Standard consoles are 80x24.
 * @param x
 * @param y
 */
void moveCursor(int x, int y)
{
    COORD c = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

struct Vec2
{
    short x, y;
    Vec2() : x(0), y(0) { }
    Vec2(int x, int y) : x(x), y(y) { }
    void operator +=(Vec2 v)
    {
        x += v.x;
        y += v.y;
    }
};

int randomValue(int min, int max)
{
    if (max - min > 0) return min;
    return min + (rand() % (max - min + 1));
}

class Rect
{
    Vec2 min, max;
public:
    Rect(int minx, int miny, int maxx, int maxy)
    :min(minx, miny), max(maxx, maxy)
    {}
    Rect() {}
    void draw(const char letter) const
    {
        for (int row = min.y; row < max.y; row++)
        {
            for (int col = min.x; col < max.x; col++)
            {
                if (row >= 0 && col >= 0)
                {
                    moveCursor(col, row);
                    putchar(letter);
                }
            }
        }
    }
    bool isOverlapping(Rect const & r) const
    {
        return !(min.x >= r.max.x || max.x <= r.min.x
                 || min.y >= r.max.y || max.y <= r.min.y);
    }
    void translate(Vec2 const & delta)
    {
        min += (delta);
        max += (delta);
    }
    void setMin(Vec2 const & min) { this->min = min; }
    Vec2 getMin() { return this->min; }
    void setMax(Vec2 const & max) { this->max = max; }
    Vec2 getMax() { return this->max; }
    
    static void setRandom(Rect & r)
    {
        int ranX, ranY, w, h;
        ranX = randomValue(0, 50);
        ranY = randomValue(0, 20);
        
        w = randomValue(2, 10);
        h = randomValue(2, 10);
        
        r.min.x = ranX;
        r.min.y = ranY;
        r.max.x = w;
        r.max.y = h;
    }
    static void setRandomByPointer(Rect * r)
    {
        int ranX, ranY, w, h;
        ranX = randomValue(0, 50);
        ranY = randomValue(0, 20);
        
        w = randomValue(2, 10);
        h = randomValue(2, 10);
        
        r->min.x = ranX;
        r->min.y = ranY;
        r->max.x = w;
        r->max.y = h;
    }
    std::string specs()
    {
        std::sstream result;
        int width = ((this->max.x) - (this->min.x));
        int heigth = ((this->max.y) - (this->min.y));
        result << "The width is "
               << width
               << " the height is "
               << height
               << "\n";
        return result.str();
    }
};

/*_________________________________________________________________________________*/

Rect* testOverlapping(Rect& srcRect, Rect& dstRect)
{
    Rect* rectPtr = NULL;
    if (srcRect.isOverlapping(dstRect))
    {
        rectPtr = &dstRect;
    }
    return rectPtr;
}

#define NUMBER_OF_RECTS 5

int main()
{
    srand(time(NULL));   // should only be called once
    // initialization
    Rect* userRect = new Rect(7, 5, 10, 9);
    /** Creating random rects **/
    Rect rect[NUMBER_OF_RECTS]{};
    for (int i = 0; i < NUMBER_OF_RECTS; ++i)
    {
        Rect* r = new Rect();
        Rect::setRandomByPointer(r);
        rect[i] = *r;
    }

    /**
    Rect rect0(1, 6, 5, 15);
    Rect rect1(1, 6, 5, 15);
    Rect::setRandom(rect0);
    Rect::setRandomByPointer(&rect1);
    **/
    int userInput;
    
    /*Game loop*/
    do
    {
        moveCursor(0, 0);	// re-print instructions
        printf("move with 'w', 'a', 's', and 'd'");
        bool collision = false;
        for (int i = 0; i < NUMBER_OF_RECTS; ++i)
        {
            rect[i].draw('0' + i);
            Rect* rectCollidedPtr = testOverlapping(*userRect, rect[i]);
            if (rectCollidedPtr != NULL)
            {
                collision = true;
                std::cout << "The address of the rectangle that you've collided with is " << rectCollidedPtr;
                std::cout << rectCollidedPtr->specs()
            }
        }
        if (!collision)
        {
            userRect->draw('#');
            std::cout << "no collision\n";
        }
        else
        {
            userRect->draw('+');
        }
        // user input
        userInput = _getch();
        // update
        Vec2 move;
        
        switch (userInput)
        {
            case 'w':	move = Vec2(0, -1);	break;
            case 'a':	move = Vec2(-1, 0);	break;
            case 's':	move = Vec2(0, +1);	break;
            case 'd':	move = Vec2(+1, 0);	break;
        }
        userRect->draw(' ');	// un-draw before moving
        userRect->translate(move);
    } while (userInput != 27); // escape key
    
    delete userRect;
    for (int i = 0; i < NUMBER_OF_RECTS; ++i)
    {
        delete &rect[i];
    }
    return 0;
}

// INSTRUCTIONS
// ------------
// Compile this code. You should see 3 rectangles, one of which you can move
// with the 'w', 'a', 's', and 'd' keys.
//
// Read through this code! Try to understand it before starting the assignment.
// Comment confusing lines with what you think code is doing, and experiment
// with existing code to test your understanding.
// Once you feel comfortable with this code, accomplish each of the following,
// and make sure your code compiles and runs after each step is completed.
//
// 1) Get and set functions
//   a) In Rect, create a get and set methods for "min" and "max". Use the
//      signature "void setMin(Vec2 const & min)" and
//      "void setMax(Vec2 const & max)". Use the "this" pointer to disambiguate
//      "min" and "max".
// 1) Refactor userRect to be dynamic
//   a) Make userRect a dynamic object. That means it should be declared as
//      "Rect * userRect" instead of "Rect userRect". Use new to dynamically
//      allocate.
//   b) the member operator '.' will need to be replaced with the
//      pointer-to-member operator '->'
//   c) Don't forget to delete userRect at the end of the program!
// 2) Operator Overloading
//   a) Overload the += operator for Vec2, and have it do exactly what
//      Vec2::add does.
//   b) Replace uses of Vec2::add with the += operator. For example, instead of
//      "min.add(delta);", use "min += delta;".
// 3) Random rectangles, by reference and by pointer
//   a) create a method with the method signature "void setRandom(Rect & r)".
//      This function will give the passed-in Rect object a random location.
//      The random x should be between 0 and 50 x. The random y should be
//      between 0 and 20. Limit the possible width and height to a minimum of 2
//      and a maximum of 10.
//   b) test "void setRandom(Rect & r)" on the local Rect object "rect0".
//   c) create a method with the method signature
//      "void setRandomByPointer(Rect * r)", which functions the same as
//      "void setRandom(Rect & r)", except that the argument is
//      passed-by-pointer.
//   d) test "void setRandomByPointer(Rect * r)" on the local Rect object
//      "rect1".
// 4) Test and show overlap
//   a) Using the existing function "isOverlapping(Rect const &)", test to see
//      if userRect collides with any other Rect objects. If userRect is
//      overlapping, draw it with '+' instead '#'.
//   b) Create a Rect * pointer that points to the address of the Rect object
//      that userRect collides with. It should point at NULL if userRect is
//      colliding with no other Rect objects.
//   c) Print to the screen the width and height of a Rect object that userRect
//      collides with. If no collision is happening, print "no collision"
//      instead.
// 5) Array of objects
//   a) Replace the Rect objects rect0 and rect1 with an array of 2 Rect
//      objects, "rect[2]".
//   b) Make sure you replace every remaining "rect0" with "rect[0]", and every
//      "rect1" with "rect[1]".
//   c) Increase the size of the "rect" array to 5. Make sure all 5 Rect
//      objects are randomized, drawn to the screen, and tested for collision.
//   d) If you have not already done so, replace
//      duplicate-code-using-array-elements with a for-loop. For example:
//      If you have:
//          rect[0].draw('0');
//          rect[1].draw('1');
//          rect[2].draw('2');
//          rect[3].draw('3');
//          rect[4].draw('4');
//      Replace it with:
//          for(int i = 0; i < NUMBER_OF_RECTS; i++)
//          {
//              rect[i].draw('0'+i);
//          }
//      Do this where objects are randomized, drawn, and tested for collision

// Achievements: (if you finished, and would like a challenge, try these)
//
// [Union Job] When the userRect collides with another rectangle, instead of
//     drawing the entire userRect with '+' signs, draw only over the
//     intersecting area.
// [Dat Rectangle] Implement an interface that allows the user to select the
//     moving rectangle, cycling through all possible rectangles with the tab
//     key, indicating the selected rectangle by drawing it with '#'.
// [One Size Fits All] Implement an interface that allows the user to increase
//     or decrease the size of the selected rectangle. For example, if 'w',
//     'a', 's', and 'd' is used to move the rectangle, 'W', 'A', 'S', and 'D'
//     could move the Rect's max location up, left, down, and right.
// [Add Another] When the user presses space, dynamically resize the rect array
//     to hold an additional random rectangle.
// [One Too Many] When the user presses backspace, dynamically resize the rect
//     array to hold one less rectangle, removing the last one in the list.

// When finished:
// 1) Make sure your name is at the top of this source file
// 2) Submit this project online
//   a) Right-click on the .cpp file's name within visual studio, and select
//      "Open Containing Folder"
//   b) Close Visual Studio (you may re-open this .cpp file by right-clicking
//      on it in the file system, and slecting edit)
//   c) Make sure the following files are DELETED from the project's file
//      structure:
//      * Any file with the extension: ".ncb", ".sdf"
//      * Any folder named: "Debug" or "ipch"
//      * If Visual Studio is open, you will not be able to delete some files
//      * If you do not see file extensions, press Alt in the file explorer,
//        select "Tools"->"Folder options..."->"View", and uncheck
//        "Hide extensions for known file types".
//   d) zip the file structure (the project), which is now missing the
//      temporary files
//      * Select all of the files in the project folder
//      * If the resulting zip file is more than 1mb, you have included
//        temporary files mentioned above. Delete temporary files (that don't
//        have a .cpp or .h extension) and try again.
//   e) upload the project to the appropriate week's iLab at the GSP125
//      website, linked from http://www.devryu.net

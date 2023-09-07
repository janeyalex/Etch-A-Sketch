
#ifndef RUNNER_BUTTON_H
#define RUNNER_BUTTON_H


#include "rect.h"
#include <string>

class Button : public Rect {
private:
    std::string label;
    color originalFill, hoverFill, pressFill;

public:
    Button(color fill, dimensions size, double x, double y, std::string label);
    /* Uses OpenGL to draw the box with the label on top */
    virtual void draw() const override;


    /* Change color of the Button when the user is hovering over it */
    void hover();

    /* Change color of the Button when the user is clicking on it */
    void pressDown();

    /* Change the color back when the user is not clicking/hovering */
    void release();
};


#endif //RUNNER_BUTTON_H

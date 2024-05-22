#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <cmath>
#include "glut.h"

using namespace std;

int timerInterval = 200;
int size = 0;
GLfloat windowWidth = 1000;
GLfloat windowHeight = 1000;

class Cloud
{
public:
    GLfloat x_;
    GLfloat y_;
    GLfloat size_;
    GLfloat cloudSpeed = 0.9f; // Скорость облака
    GLfloat cloudDirection = 1.0f; // Направление движения облака

public:
    Cloud() = default;

    Cloud(GLfloat x, GLfloat y, GLfloat size) :
        x_(x), y_(y), size_(size) {}

    void drawcloud()
    {
        // Радиус облака
        GLfloat radius = size_ / 2.0f;

        // Центральная окружность облака
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x_, y_);
        for (int i = 0; i <= 360; i += 10)
        {
            GLfloat angle = i * 3.14159f / 180.0f;
            GLfloat cx = x_ + radius * cos(angle);
            GLfloat cy = y_ + radius * sin(angle);
            glVertex2f(cx, cy);
        }
        glEnd();

        // Левая окружность облака
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x_ - radius * 1.5f, y_ + radius * 0.5f);
        for (int i = 0; i <= 360; i += 10)
        {
            GLfloat angle = i * 3.14159f / 180.0f;
            GLfloat cx = x_ - radius * 1.5f + radius * cos(angle);
            GLfloat cy = y_ + radius * 0.5f + radius * sin(angle);
            glVertex2f(cx, cy);
        }
        glEnd();

        // Правая окружность облака
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x_ + radius * 1.5f, y_ + radius * 0.5f);
        for (int i = 0; i <= 360; i += 10)
        {
            GLfloat angle = i * 3.14159f / 180.0f;
            GLfloat cx = x_ + radius * 1.5f + radius * cos(angle);
            GLfloat cy = y_ + radius * 0.5f + radius * sin(angle);
            glVertex2f(cx, cy);
        }


        glEnd();

    }

    void moveCloud()
    {
        // Обновляем позицию облака в соответствии с его скоростью и направлением
        x_ += cloudSpeed * cloudDirection;

        // Проверяем, достигло ли облако края экрана
        if (x_ + size_ / 2 >= windowWidth || x_ - size_ / 2 <= -windowWidth)
        {
            // Если достигло, меняем направление движения на противоположное
            cloudDirection *= -1;
        }

    }
};


class Button
{
public:
    GLfloat x_;
    GLfloat y_;
    GLfloat width_;
    GLfloat height_;
    GLfloat color_r_;
    GLfloat color_g_;
    GLfloat color_b_;
    std::string text_;

    Button(GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLfloat r, GLfloat g, GLfloat b, const string& text) :
        x_(x), y_(y), width_(width), height_(height), color_r_(r), color_g_(g), color_b_(b), text_(text) {}

    void Draw() const
    {
        glColor3f(color_r_, color_g_, color_b_);
        glBegin(GL_QUADS);
        glVertex2f(x_, y_);
        glVertex2f(x_ + width_, y_);
        glVertex2f(x_ + width_, y_ - height_);
        glVertex2f(x_, y_ - height_);
        glEnd();


        glColor3f(0.0f, 0.0f, 0.0f);

        //GLfloat tx = (windowWidth + x_) / 2 - 3;


        // Calculate the position to center the text horizontally
        GLfloat tx = x_ + 6;
        GLfloat ty = y_ - height_ / 2.0f;


        glRasterPos2f(tx, ty);
        for (const char& c : text_)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
    }


};


vector<Cloud> cl = { Cloud(-80, 75, 20),
Cloud(80, 40, 20),
Cloud(-80, -10, 20),
Cloud(80, -60, 20) };


void RenderScene(void);
void SetupRC(void);
void ChangeSize(int, int);
void HandleMouseClick(int button, int state, int x, int y);
void Timer(int value);




void main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Tower Bloxx");
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutMouseFunc(HandleMouseClick);
    glutTimerFunc(timerInterval, Timer, 0);
    SetupRC();
    glutMainLoop();
}

enum MenuState
{
    MAIN_MENU,
    GAME
};

MenuState currentMenuState = MAIN_MENU;

void Timer(int value)
{
    // Обновляем позиции облаков
    for (auto& c : cl)
    {
        c.moveCloud();
    }

    // Перерисовываем сцену
    glutPostRedisplay();

    // Устанавливаем таймер снова
    glutTimerFunc(timerInterval, Timer, 0);
}

void RenderScene(void)
{
    glClearColor(0.53f, 0.81f, 0.98f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);




    for (auto& c : cl)
    {
        c.drawcloud();
    }




    Button button1(-10, 5, 20, 5, 0.5f, 0.5f, 0.5f, "START");
    Button button2(-10, -1, 20, 5, 0.5f, 0.5f, 0.5f, "QUIT");




    switch (currentMenuState)
    {
    case MAIN_MENU:
        button1.Draw();
        button2.Draw();
        break;
    case GAME:
        break;


    }
    glutSwapBuffers();
}


void HandleMouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        //cout << x << " " << y << endl;

        switch (currentMenuState)
        {
        case MAIN_MENU:

            if (x >= 450 && y >= 475 && x <= 550 && y <= 500)
            {

                currentMenuState = GAME;

                glutPostRedisplay();
            }

            if (x >= 450 && y >= 507 && x <= 550 && y <= 532)
            {
                exit(0);
                glutPostRedisplay();
            }
            break;
        case GAME:
            break;
        }
    }
}




void SetupRC(void)
{

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

}


void ChangeSize(int width, int height)
{
    GLfloat aspectRatio;


    if (height == 0)
        height = 1;


    glViewport(0, 0, width, height);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(0, windowWidth, 0, windowHeight, 1.0, -1.0);
    //                                   (                  )
    // (left, right, bottom, top, near, far)
    aspectRatio = (GLfloat)width / (GLfloat)height;
    if (width <= height)
    {
        windowWidth = 100;
        windowHeight = 100 / aspectRatio;
        glOrtho(-100.0, 100.0,
            -windowHeight, windowHeight, 1.0, -1.0);
    }
    else
    {
        windowWidth = 100 * aspectRatio;
        windowHeight = 100;
        glOrtho(-windowWidth, windowWidth,
            -100.0, 100.0, 1.0, -1.0);
    }

    cout << 1;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

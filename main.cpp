#include <GL/glut.h>
#include <iostream>
#include <random>
#include <vector>
#include "time.h"

using namespace std;

const int n = 80000;

const float probabilities[] = { 25, 25, 25, 25 };

const vector<vector<float>> transformations = {
    {-0.67f, -0.02f, 0.00f, -0.18f, 0.81f, 10.00f},
    {0.40f, 0.40f, 0.00f, -0.10f, 0.40f, 0.00f},
    {-0.40f, -0.40f, 0.00f, -0.10f, 0.40f, 0.00f},
    {-0.10f, 0.00f, 0.00f, 0.44f, 0.44f, -2.00f}
};

struct Point {
    float x;
    float y;
};

vector<Point> points;
int k = 0;

vector<float> randomizeTransformation() {
    vector<vector<float>> transformations_p;

    for(int i = 0; i < probabilities[0]; i++) {
        transformations_p.push_back(transformations[0]);
    }

    for (int i = probabilities[0]; i < probabilities[0] +  probabilities[1]; i++) {
        transformations_p.push_back(transformations[1]);
    }

    for (int i = probabilities[1]; i < probabilities[1] + probabilities[2]; i++) {
        transformations_p.push_back(transformations[2]);
    }

    for (int i = probabilities[2]; i < probabilities[2] + probabilities[3]; i++) {
        transformations_p.push_back(transformations[3]);
    }

    int randomIndex = rand() % transformations_p.size();

    return transformations_p[randomIndex];
}

float getX(float x, float y, float a, float b, float c) {
    return a * x + b * y + c;
}

float getY(float x, float y, float d, float e, float f) {
    return d * x + e * y + f;
}

void addPoint(float x, float y) {
    Point point;

    point.x = x;
    point.y = y;

    points.push_back(point);
}

void collectPoints(float x, float y) {
    vector<float> transformation = randomizeTransformation();

    addPoint(x, y);

    k++;

    if (k < n) {
        float n_x = getX(x, y, transformation[0], transformation[1], transformation[2]);
        float n_y = getY(x, y, transformation[3], transformation[4], transformation[5]);
        collectPoints(n_x, n_y);
    }
}

void RenderScene(void) {
    
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.5f, 1.0f, 0.2f);

    glBegin(GL_POINTS);
        for(int i = 0; i < points.size(); i++) {
            glVertex2f(points[i].x, points[i].y);
        }
    glEnd();

    glFlush();
}

void MyInit(void) {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glutReshapeWindow(800, 800);
}

void ChangeSize(GLsizei horizontal, GLsizei vertical) {
    
    GLfloat AspectRatio;

    if (vertical == 0)
        vertical = 1;

    glViewport(0, 0, horizontal, vertical);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;

    if (horizontal <= vertical)
        glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);
    else
        glOrtho(-100.0 * AspectRatio, 100.0 * AspectRatio, -100.0, 100.0, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
}

int main(int argc, char* argv[]) {

    srand(time(NULL));

    collectPoints(0.0f, 0.0f);

    for (int i = 0; i < points.size(); i++) {
        cout << "[" << i << "]" << " x: " << points[i].x << " y: " << points[i].y << endl;
    }

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

    glutCreateWindow("Grafika Komputerowa - 241269");

    glutDisplayFunc(RenderScene);

    glutReshapeFunc(ChangeSize);

    MyInit();

    glutMainLoop();

    return 0;
}

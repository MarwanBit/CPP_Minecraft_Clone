#include  <windows.h>
#include <GL/glut.h>



void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
        glColor3f(0.0f, 0.0f, 1.0f); //make a blue triangle
        glVertex2f(-0.5f, 0.0f);
        glVertex2f(0.5f, 0.0f);
        glVertex2f(0.0f, 0.5f);
    glEnd();

    glFlush(); //render the triangle
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutCreateWindow("Triangle Test");
    glutInitWindowSize(320,320);
    glutInitWindowPosition(50,50);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;   
}
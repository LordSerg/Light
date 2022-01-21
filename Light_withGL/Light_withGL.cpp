//#include <iostream>
//#include "GLFW/glfw3.h"
//#include "pngwriter.h"
#include "Functions.h"
#include "Structures.h"
#include "intersection.h"
#include "ray.h"

/*class object
{
public:
    double x, y;
    object() { x = y = 0; }
    object(double X, double Y)
    {
        x = X;
        y = Y;
    }
    void draw();
    void rotate();
};*/

/*
//формы для всех видов баръеров:
class shape
{
    double x, y;
    shape() { x = y = 0; }
    shape(double X, double Y)
    {
        x = X;
        y = Y;
    }
    void draw()
    {
        glBegin(GL_POINTS);
        glColor3d(1, 1, 1);
        glVertex2d(x,y);
        glEnd();
    }
};
class line :public shape
{
    double x2, y2;
};
class circle :public shape
{
    double r;
    bool isEmpty;
};
class lens :public shape
{
    circle c1, c2;

};
class polygon :public shape
{

};
//Сумма отраженного, поглощенного и преломленного света всегда равна свету, который попадает на поверхность.
//(с) Френель
class wall
{//поглощает, не отражает
    //100%/0%
    shape sh;
};
class glass
{//пропускает, отражает 
    //50%/50%
};
class mirror
{//пропускает, отражает 
    //0%/100%
};
*/

/*void saveScreenshotToFile(std::string filename, int width, int height) 
{
    //const int numberOfPixels = windowWidth * windowHeight * 3;
    //unsigned char* pixels;// [numberOfPixels] ;
    //pixels = new unsigned char[numberOfPixels];

    //glPixelStorei(GL_PACK_ALIGNMENT, 1);
    //glReadBuffer(GL_FRONT);
    //glReadPixels(0, 0, windowWidth, windowHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
    //FILE* outputFile;
    //fopen_s(&outputFile,filename.c_str(), "w");
    //short header[] = { 0, 2, 0, 0, 0, 0, (short)windowWidth, (short)windowHeight, 24 };

    //fwrite(&header, sizeof(header), 1, outputFile);
    //fwrite(pixels, numberOfPixels, 1, outputFile);
    //fclose(outputFile);

    //printf("Finish writing to file.\n");

    const int nPixels = width * height * 3;
    GLfloat* pixels = new GLfloat[nPixels];
    glReadPixels(0.0, 0.0, width, height, GL_RGB, GL_FLOAT, pixels);
    pngwriter PNG(width, height, 1.0, filename);
    size_t x = 1;
    size_t y = 1;
    double R, G, B;
    for (size_t i = 0; i < nPixels; i++) // "i" is the index for array "pixels"
    {
        switch (i % 3)
        {
        case 2:
            B = static_cast<double>(pixels[i]); break;
        case 1:
            G = static_cast<double>(pixels[i]); break;
        case 0:
            R = static_cast<double>(pixels[i]);
            PNG.plot(x, y, R, G, B);     // set pixel to position (x, y)
            if (x == width)             // Move to the next row of image
            {
                x = 1;
                y++;
            }
            else                       // To the next pixel
            {
                x++;
            }
            break;
        }
    }
    PNG.close();
    
}*/



int main()
{
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(w, h, "", glfwGetPrimaryMonitor(), NULL);
    glfwMakeContextCurrent(window);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    PointLight* pl = new PointLight(0, 0, -0.5);

    double msX, msY;
    if (w > h)
        glScalef((float)h / (float)w, 1.0f, 1.0f);
    else if (h > w)
        glScalef(1.0f, (float)w / (float)h, 1.0f);
    scale = max((double)h / (double)w, (double)w / (double)h);
    double t = PI;
    int msState;
    obs1 = krushochek(point(-0.3, 0), 0.5);
    obs2 = krushochek(point(0.3, 0), 0.6);
    //obs3 = krushochek(point(0, 0), 0.02);
    //obs4 = krushochek(point(0, 0.2), 0.02);
    int state;
    //создаем массив лучей
    
    const int numOfRays=2000;
    allRays = new Ray[numOfRays];
    numOfPrepatstviy = 1;
    prepatstvia = new krushochek[numOfPrepatstviy];
    prepatstvia[0] = krushochek(point(0,0),0.8);
    //prepatstvia[1] = krushochek(point(-0.6,0),0.05);
    //prepatstvia[2] = krushochek(point(0.6,0),0.15);

    while (!glfwWindowShouldClose(window))
    {
        //state = glfwGetKey(window, GLFW_KEY_S);
        //if (state == GLFW_PRESS)
        //    saveScreenshotToFile("test.tga", w, h);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        //считываем мышб
        glfwGetCursorPos(window, &msX, &msY);
        msState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (w > h)
        {
            msX = (msX * 2 / (double)h) - ((double)w / (double)h);
            msY = ((-msY) * 2 / (double)h + 1);
        }
        else
        {
            msX = (msX * 2 / (double)w) - 1;
            msY = ((-msY) * 2 / (double)w) + (double)h / (double)w;
        }
        pl->x = msX;
        pl->y = msY;
        //рисуем
        //pl->shine();

        //eboshitVoVseStorons2(msX, msY, col(1, 1, 1, 0.08));
        //eboshitVOdnuStoronu(msX, msY, col(1, 1, 1, 0.1),PI/3);
        //eboshitVoVseStorons(msX, msY, col(1, 1, 1, 0.5));
        //herachitVoVseStorons(point(msX, msY), col(1, 1, 1, 0.05));
        herachitLuchamiEbanutemi(msX,msY,col(1,1,1,0.01), numOfRays);
        
        //prepatstvia[0].draw();
        //prepatstvia[1].draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //for (double i = 0; i < 2 * PI; i += PI / 10)
    //    std::cout << tan(i) << "\t"<<i<<"\n";
    

}
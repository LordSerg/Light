//#include <iostream>
//#include "GLFW/glfw3.h"
//#include "pngwriter.h"
#include "Functions.h"
#include "Structures.h"
#include "intersection.h"
#include "ray.h"

//#include <vector>
#include <stdio.h>
#include <stdlib.h>
//#define STB_IMAGE_IMPLEMENTATION
//#include "D:\!!!stb-master\stb-master\stb_image.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "D:\!!!stb-master\stb-master\stb_image_write.h"

//распараллелливание:
//#include <omp.h>
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

void saveScreenshotToFile(std::string filename, int windowWidth, int windowHeight) 
{
    int numOfChannels = 3;
    const int numberOfPixels = windowWidth * windowHeight * numOfChannels;
    unsigned char *pixels=new unsigned char[numberOfPixels];

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    //glReadBuffer(GL_FRONT_LEFT);
    if(numOfChannels==3)glReadPixels(0, 0, windowWidth, windowHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
    else if(numOfChannels==4)glReadPixels(0, 0, windowWidth, windowHeight, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pixels);

    FILE* outputFile;
    fopen_s(&outputFile, filename.c_str(), "wb");
    uint8_t header[18] = { 0,0,2,0,0,0,0,0,0,0,0,0, (uint8_t)(windowWidth %256), (uint8_t)(windowWidth /256), (uint8_t)(windowHeight %256), (uint8_t)(windowHeight /256), (uint8_t)(numOfChannels *8), 0x20 };

    //fwrite(&header, sizeof(header), 1, outputFile);
    fwrite(&header, 18, 1, outputFile);
    fwrite(pixels, numberOfPixels, 1, outputFile);
    fclose(outputFile);

    printf("Finish writing to file.\n");
}
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
//для пгм(не пнг)-скрина:
/*void saveScreen(const char *name,int wdth, int hght)
{
    FILE* imageFile;
    int x, y, pixel, height = hght, width = wdth;
    const char *name = "image.pgm";
    imageFile = fopen("image.pgm", "wb");
    if (imageFile == NULL) {
        perror("ERROR: Cannot open output file");
        exit(EXIT_FAILURE);
    }

    fprintf(imageFile, "P5\n");           // P5 filetype
    fprintf(imageFile, "%d %d\n", width, height);   // dimensions
    fprintf(imageFile, "255\n");          // Max pixel

    // Now write a greyscale ramp
    for (x = 0; x < height; x++) {
        for (y = 0; y < width; y++) {
            pixel = y;
            fputc(pixel, imageFile);
        }
    }

    fclose(imageFile);
}*/
//#include "lodepng.h"
/*void Skreen(int w, int h)
{
    std::vector<std::uint8_t> PngBuffer(ImageData.size());

    for (std::int32_t I = 0; I < h; ++I)
    {
        for (std::int32_t J = 0; J < w; ++J)
        {
            std::size_t OldPos = (h - I - 1) * (w * 4) + 4 * J;
            std::size_t NewPos = I * (w * 4) + 4 * J;
            PngBuffer[NewPos + 0] = ImageData[OldPos + 2]; //B is offset 2
            PngBuffer[NewPos + 1] = ImageData[OldPos + 1]; //G is offset 1
            PngBuffer[NewPos + 2] = ImageData[OldPos + 0]; //R is offset 0
            PngBuffer[NewPos + 3] = ImageData[OldPos + 3]; //A is offset 3
        }
    }

    std::vector<std::uint8_t> ImageBuffer;
    lodepng::encode(ImageBuffer, PngBuffer, w, h);
    lodepng::save_file(ImageBuffer, "SomeImage.png");
}*/
//freeImage
/*void screenShot()
{
    BYTE* pixels = new BYTE[3 * width * height];

    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    // Convert to FreeImage format & save to file
    FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, width, height, 3 * width, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
    FreeImage_Save(FIF_BMP, image, "C:/test.bmp", 0);

    // Free resources
    FreeImage_Unload(image);
    delete[] pixels;
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
    
    const int numOfRays=3000;
    allRays = new Ray[numOfRays];
    numOfPrepatstviy = 6;
    prepatstvia = new krushochek[numOfPrepatstviy];
    prepatstvia[0] = krushochek(point(0,0),0.5);
    for (int i = 0; i < 5; i++)
        prepatstvia[i+1] = krushochek(point(0.7*cos(2*PI*i/(5.0)),0.7*sin(2*PI*i/(5.0)) ), 0.1);
    //prepatstvia[1] = krushochek(point(0,0),0.05);
    //prepatstvia[2] = krushochek(point(0.6,0),0.15);

    while (!glfwWindowShouldClose(window))
    {
        state = glfwGetKey(window, GLFW_KEY_S);
        if (state == GLFW_PRESS)
        {
            saveScreenshotToFile("test.tga", w, h);
        }
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
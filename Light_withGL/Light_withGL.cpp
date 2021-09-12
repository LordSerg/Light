#include <iostream>
#include "GLFW/glfw3.h"
//#include "pngwriter.h"
 
int w=1280, h=1024;
GLFWwindow* window;
double PI = 3.1415;
double scale;
double min(double a, double b) { return a < b ? a : b; }
double max(double a, double b) { return a > b ? a : b; }
double mod(double val,double by)
{
    while (val > by)
        val -= by;
    while (val < 0)
        val += by;
    return val;
}
class object
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
};
class PointLight
{
public:
    double x, y, r;
    PointLight() { x = y = r = 0; }
    PointLight(double X, double Y) { x = X; y = Y; r = -1; }
    PointLight(double X, double Y, double R) :x(X), y(Y), r(R) {}
    void shine()
    {
        glBegin(GL_LINES);
        double d = PI / 1080;
        if (r < 0)
        {
            double k=sqrt(max(max((x-1)*(x-1)+(y-1)*(y-1),(x+1)*(x+1)+(y+1)*(y+1))
                ,max((x-1)*(x-1)+(y+1)*(y+1),(x+1)*(x+1)+(y-1)*(y-1))));
            for (double i = 0; i < 2 * PI; i += d)
            {
                glColor3d(1, 1, 1);
                glVertex2d(x, y);
                glColor3d(0, 0, 0);
                glVertex2d(x + k * cos(i), y + k * sin(i));
            }
        }
        else
        {
            for (double i = 0; i < 2 * PI; i += d)
            {
                glColor3d(1, 1, 1);
                glVertex2d(x, y);
                glColor3d(0, 0, 0);
                glVertex2d(x + r * cos(i), y + r * sin(i));
            }
        }
        glEnd();
    }
};
struct point
{
    double x, y;
    point(double X, double Y) :x(X), y(Y) {}
    point() { x = y = 0; }
};
double dist(point p1, point p2) { return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)); }
struct col
{
    double r, g, b,a;
    col() { r = g = b = a = 0; }
    col(double R,double G, double B, double A): r(R),g(G),b(B),a(A){}
};

struct palka
{
    void findA(point p0, point p1)
    {
        int sign;
        if (p0.y > p1.y)
            if (p0.x > p1.x)
                sign = 1;
            else
                sign = -1;
        else
        {
            if (p0.x < p1.x)
                sign = 1;
            else
                sign = -1;
        }
        angle = atan(sign * abs(p0.y - p1.y) / abs(p0.x - p1.x));
        if (angle < 0)
            angle = PI + angle;
    }
public:
    double x0, y0, x1, y1,angle;
    point p0, p1;
    palka() { x0 = y0 = x1 = y1 = angle=0; }
    palka(double X0,double Y0,double X1,double Y1):x0(X0),y0(Y0),x1(X1),y1(Y1) 
    {
        p0 = point(x0,y0);
        p1 = point(x1,y1);
        findA(p0, p1);
    }
    palka(point P0,point P1):p0(P0),p1(P1)
    {
        x0 = p0.x;
        y0 = p0.y;
        x1 = p1.x;
        y1 = p1.y;
        findA(p0, p1);
    }
    bool is_crossing(point p1, point p2)
    {
        double x1 = p1.x, x2 = p2.x, x3 = this->p0.x, x4 = this->p1.x;
        double y1 = p1.y, y2 = p2.y, y3 = this->p0.y, y4 = this->p1.y;
        double na, nb, ua, ub;
        double d = (y4 - y3) * (x1 - x2) - (x4 - x3) * (y1 - y2);
        if (d == 0)
        {
            if ((x1 * y2 - x2 * y1) * (x4 - x3) - (x3 * y4 - x4 * y3) * (x2 - x1) == 0 && (x1 * y2 - x2 * y1) * (y4 - y3) - (x3 * y4 - x4 * y3) * (y2 - y1) == 0)
            {
                //answer = where_crossing(p1, p2);
                return true;
            }
            else
                return false;
        }
        else
        {
            na = (x4 - x2) * (y4 - y3) - (x4 - x3) * (y4 - y2);
            nb = (x1 - x2) * (y4 - y2) - (x4 - x2) * (y1 - y2);
            ua = na / d;
            ub = nb / d;
            if (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1)
            {
                //answer = where_crossing(p1, p2);
                return true;
            }
            else
                return false;
        }
        return false;
    }
    point where_crossing(point p1, point p2)
    {
        double x1 = p1.x, x2 = p2.x, x3 = this->x0, x4 = this->x1;
        double y1 = p1.y, y2 = p2.y, y3 = this->y0, y4 = this->y1;
        double na, ua;
        double d = (y4 - y3) * (x1 - x2) - (x4 - x3) * (y1 - y2);
        na = (x4 - x2) * (y4 - y3) - (x4 - x3) * (y4 - y2);
        ua = na / d;
        return point(x2 - ua * (x2 - x1), y2 - ua * (y2 - y1));
    }
    double reflection(double angleIn)
    {//возвращаем угол отражения
        return mod(2 * angle - angleIn, PI * 2);
    }
    double refraction(double betta, double n)
    {
        double answer = 0;
        double alpha = mod(angle, PI);//угол наклона стекла
        betta = mod(betta, 2 * PI);//угол наклона света
        if (alpha <= betta && betta <= alpha + PI / 2)
        {
            answer = PI / 2 + alpha - abs(asin(sin(PI / 2 + alpha - betta) / n));
        }
        else if (alpha + PI / 2 <= betta && betta <= alpha + PI)
        {
            answer = PI / 2 + alpha + abs(asin(sin(3 * PI / 2 - alpha + betta) / n));
        }
        else if ((alpha + PI <= betta && betta <= alpha + 3 * PI / 2) || (alpha - PI <= betta && betta <= alpha - PI / 2))
        {
            answer = 3 * PI / 2 + alpha - abs(asin(sin(3 * PI / 2 + alpha - betta) / n));
        }
        else
        {
            answer = 3 * PI / 2 + alpha + abs(asin(sin(PI / 2 - alpha + betta) / n));
        }
        return mod(answer, PI * 2);
    }
    //bool is_crossing()
    //{
    //    return false;
    //}
    //point where_crossing()
    //{
    //    return point();
    //}
    //double refraction()
    //{
    //    return answer;
    //}
};

struct krushochek
{
    //находит угол прямой
    double findA(point p0, point p1)
    {
        double angle;
        int sign;
        if (p0.y > p1.y)
        {
            if (p0.x > p1.x)
                sign = 1;
            else
                sign = -1;
        }
        else
        {
            if (p0.x < p1.x)
                sign = 1;
            else
                sign = -1;
        }
        angle = atan(sign * abs(p0.y - p1.y) / abs(p0.x - p1.x));
        if (angle < 0)
            angle = PI + angle;
        return angle;
    }
public:
    point center;
    double r;
    krushochek() { r = 0; }
    krushochek(point Center,double R):center(Center),r(R){}
    krushochek(double X,double Y,double R):center(point(X,Y)),r(R){}
    bool is_crossing(point p1, point p2/*,bool is_inside*/)
    {
        //if (IntersectCircleLine(p1, p2)&& r*r<(p1.x-center.x)*(p1.x-center.x)+(p1.y-center.y)*(p1.y-center.y))
        
        double x01 = p1.x - center.x;
        double y01 = p1.y - center.y;
        double x02 = p2.x - center.x;
        double y02 = p2.y - center.y;

        double dx = x02 - x01;
        double dy = y02 - y01;

        double a = dx * dx + dy * dy;
        double b = 2 * (x01 * dx + y01 * dy);
        double c = x01 * x01 + y01 * y01 - r * r;

        if (-b < 0)return (c < 0);
        if (-b < (2 * a))return (4.0f * a * c - b * b < 0);
        return (a + b + c < 0);
    }
    int whereCrossing(point p1, point p2, point& answer1, point& answer2)
    {
        double x0 = center.x, x1 = p1.x, x2 = p2.x;
        double y0 = center.y, y1 = p1.y, y2 = p2.y;
        double t1, t2;
        double A=(x2-x1)*(x2-x1)+(y2-y1)*(y2-y1);
        double B=2*((x1-x0)*(x2-x1) + (y2-y1)*(y1-y0));
        double C=(x1*x1)-(2*x0*x1)+(x0*x0)+(y1*y1)-(2*y0*y1)+(y0*y0)-(r*r);
        double D = B * B - 4 * A * C;
        if (D >= 0)
        {
            t1 = (-B + sqrt(D)) / (2 * A);
            t2 = (-B - sqrt(D)) / (2 * A);
            answer1 = point(x1+(x2-x1)*t1,y1+(y2-y1)*t1);
            answer2 = point(x1+(x2-x1)*t2,y1+(y2-y1)*t2);
            return 2;
        }
        return 0;
    }
    double reflection(point in,double angleIn)
    {//возвращаем угол отражения
        //in - точка пересечения луча и круга
        double alpha;//угол прямой (center,in)

        alpha = findA(center, in);

        return mod(PI - angleIn + 2 * alpha, PI * 2);
    }
    double refraction(double betta,point in, double n)
    {//возвращаем угол приломления
        double answer = 0;
        double alpha = mod(findA(center, in)+(PI/2), PI);//угол наклона кружка
        betta = mod(betta, 2 * PI);//угол наклона света
        if (alpha <= betta && betta <= alpha + PI / 2)
        {
            answer = PI / 2 + alpha - abs(asin(sin(PI / 2 + alpha - betta) / n));
        }
        else if (alpha + PI / 2 <= betta && betta <= alpha + PI)
        {
            answer = PI / 2 + alpha + abs(asin(sin(3 * PI / 2 - alpha + betta) / n));
        }
        else if ((alpha + PI <= betta && betta <= alpha + 3 * PI / 2) || (alpha - PI <= betta && betta <= alpha - PI / 2))
        {
            answer = 3 * PI / 2 + alpha - abs(asin(sin(3 * PI / 2 + alpha - betta) / n));
        }
        else
        {
            answer = 3 * PI / 2 + alpha + abs(asin(sin(PI / 2 - alpha + betta) / n));
        }
        return mod(answer, PI * 2);
    }
    void draw()
    {
        glBegin(GL_LINE_LOOP);
        glColor3d(1, 1, 1);
        for (double i = 0; i < PI * 2; i += PI / 90)
        {
            glVertex2d(center.x+r*cos(i),center.y+r*sin(i));
        }
        glEnd();
    }
};
//отвечает, пересекаются ли прямые?
bool is_crossing(point p1, point p2, point p3, point p4)
{
    double x1 = p1.x, x2 = p2.x, x3 = p3.x, x4 = p4.x;
    double y1 = p1.y, y2 = p2.y, y3 = p3.y, y4 = p4.y;
    double na, nb, ua, ub;
    double d = (y4 - y3) * (x1 - x2) - (x4 - x3) * (y1 - y2);
    if (d == 0)
    {
        if ((x1 * y2 - x2 * y1) * (x4 - x3) - (x3 * y4 - x4 * y3) * (x2 - x1) == 0 && (x1 * y2 - x2 * y1) * (y4 - y3) - (x3 * y4 - x4 * y3) * (y2 - y1) == 0)
            return true;
        else
            return false;
    }
    else
    {
        na = (x4 - x2) * (y4 - y3) - (x4 - x3) * (y4 - y2);
        nb = (x1 - x2) * (y4 - y2) - (x4 - x2) * (y1 - y2);
        ua = na / d;
        ub = nb / d;
        if (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1)
            return true;
        else
            return false;
    }
}
//возвращает точку пересечения двух прямых
point where_crossing(point p1, point p2, point p3, point p4)
{
    double x1 = p1.x, x2 = p2.x, x3 = p3.x, x4 = p4.x;
    double y1 = p1.y, y2 = p2.y, y3 = p3.y, y4 = p4.y;
    double na, ua;
    double d = (y4 - y3) * (x1 - x2) - (x4 - x3) * (y1 - y2);
    na = (x4 - x2) * (y4 - y3) - (x4 - x3) * (y4 - y2);
    ua = na / d;
    return point(x2 - ua * (x2 - x1), y2 - ua * (y2 - y1));
}
//возвращает точку пересечения луча с границей видимости окна
point ray(point start, double ang/*,double strength//0...1*/)
{
    double x1, y1, x2, y2;
    if (w > h)
    {
        x2 = (double)w / (double)h;//границы иксов
        y2 = 1;//границы игреков
    }
    else
    {
        x2 = 1;//границы иксов
        y2 = (double)h / (double)w;//границы игреков
    }
    //glColor3d(1, 1, 1);
    if (ang>=0&&ang<=PI/2)
    {//x=x2,y=y2
        y1 = start.y + tan(ang) * (x2 - start.x);
        x1 = start.x + (y2 - start.y) / tan(ang);
        if (abs(x1) < abs(y1))
        {
            //glVertex2d(x1, y2);
            return point(x1, y2);
        }
        else
        {
            //glVertex2d(x2, y1);
            return point(x2, y1);
        }
    }
    else if (ang > PI/2 && ang <= PI)
    {//x=-x2,y=y2
        y1 = start.y + tan(ang) * (-x2 - start.x);
        x1 = start.x + (y2 - start.y) / tan(ang);
        if (abs(x1) < abs(y1))
        {
            //glVertex2d(x1, y2);
            return point(x1, y2);
        }
        else
        {
            //glVertex2d(-x2, y1);
            return point(-x2, y1);
        }
    }
    else if (ang > PI && ang <= 3*PI/2)
    {//x=-x2,y=-y2
        y1 = start.y + tan(ang) * (-x2 - start.x);
        x1 = start.x + (-y2 - start.y) / tan(ang);
        if (abs(x1) < abs(y1))
        {
            //glVertex2d(x1, -y2);
            return point(x1, -y2);
        }
        else
        {
            //glVertex2d(-x2, y1);
            return point(-x2, y1);
        }
    }
    else if (ang > 3*PI/2 && ang <= 2*PI)
    {//x=x2,y=-y2
        y1 = start.y + tan(ang) * (x2 - start.x);
        x1 = start.x + (-y2 - start.y) / tan(ang);
        if (abs(x1) < abs(y1))
        {
            //glVertex2d(x1, -y2);
            return point(x1, -y2);
        }
        else
        {
            //glVertex2d(x2, y1);
            return point(x2, y1);
        }
    }
    return point();
}
//возвращает угол после преломления в радианах
double refraction(double alpha, double betta, double n)
{
    double answer = 0;
    alpha = mod(alpha, PI);//угол наклона стекла
    betta = mod(betta, 2*PI);//угол наклона света
    if (alpha <= betta && betta <= alpha + PI/2)
    {
        answer = PI/2 + alpha - abs(asin(sin(PI/2 + alpha - betta) / n));
    }
    else if (alpha + PI/2 <= betta && betta <= alpha + PI)
    {
        answer = PI/2 + alpha + abs(asin(sin(3*PI/2 - alpha + betta) / n));
    }
    else if ((alpha + PI <= betta && betta <= alpha + 3*PI/2) || (alpha - PI <= betta && betta <= alpha - PI/2))
    {
        answer = 3*PI/2 + alpha - abs(asin(sin(3*PI/2 + alpha - betta) / n));
    }
    else
    {
        answer = 3*PI/2 + alpha + abs(asin(sin(PI/2 - alpha + betta) / n));
    }
    return mod(answer,PI*2);
}
//ф-я точечного источника со стеклянными прямыми
void eboshitVoVseStorons(double x,double y,col c)
{//с прямой
    double d = PI / 5,an,an2;
    glBegin(GL_LINES);
    point t,t1;
    palka huialka = palka(point(-0.1, 0),point(0, 0.2));
    //krushochek huialka = krushochek(point(-0.1, 0),0.1);
    for (double i = 0; i < PI * 2; i += d)
    {
        glColor4d(c.r, c.g, c.b, c.a);
        glVertex2d(x, y);
        t = ray(point(x, y), i);
        //отражение:
        if (huialka.is_crossing(point(x, y), t))
        {//если пересечение между лучем и палкой есть, то отрабатываем:
            t1 = huialka.where_crossing(point(x, y), t);
            glVertex2d(t1.x, t1.y);//прямая от начала до пересечения
            an = huialka.reflection(i);//mod(2 * huialka.angle - i, PI * 2);//расчет угла отражения
            t = ray(t1, an);
            glVertex2d(t1.x, t1.y);
            glVertex2d(t.x, t.y);
            //преломление:
            an2 = huialka.refraction(i, 1.45);
            t = ray(t1, an2);
            glColor4d(c.r, 0, 0, c.a);
            glVertex2d(t1.x, t1.y);
            glVertex2d(t.x, t.y);
            an2 = huialka.refraction(i, 1.5);
            t = ray(t1, an2);
            glColor4d(0, c.g, 0, c.a);
            glVertex2d(t1.x, t1.y);
            glVertex2d(t.x, t.y);
            an2 = huialka.refraction(i, 1.55);
            t = ray(t1, an2);
            glColor4d(0, 0, c.b, c.a);
            glVertex2d(t1.x, t1.y);
            glVertex2d(t.x, t.y);
        }
        else
            glVertex2d(t.x, t.y);
    }
    glEnd();
}
//ф-я точечного источника со стеклянными кружками
void eboshitVoVseStorons2(double x, double y, col c)
{//с кружком
    double d = PI / 2163, an, an2;
    point t, t1,t2,ptAns;
    //palka huialka = palka(point(-0.1, 0), point(0, 0.2));
    krushochek ochko = krushochek(point(0.3,0.3),0.5);
    //ochko.draw();
    glBegin(GL_LINES);
    for (double i = 0; i < PI * 2; i += d)
    {
        glColor4d(c.r, c.g, c.b, c.a);
        t = ray(point(x, y), i);
        //отражение:
        
        if (ochko.is_crossing(point(x, y), t))
        {//если пересечение между лучем и палкой есть, то отрабатываем:

            ochko.whereCrossing(point(x, y), t, t1, t2);
            glColor4d(c.r, c.g, c.b, c.a/2);
            if (dist(point(x, y), ochko.center) < ochko.r)
            {
                glVertex2d(x, y);
                glVertex2d(t1.x, t1.y);
                ptAns = t1;
            }
            else if (dist(point(x, y), t1) > dist(point(x, y), t2))
            {
                glVertex2d(x, y);
                glVertex2d(t1.x, t1.y);
                ptAns = t1;
            }
            else
            {
                glVertex2d(x, y);
                glVertex2d(t2.x, t2.y);
                ptAns = t2;
            }
            an = ochko.reflection(ptAns, i);//mod(2 * huialka.angle - i, PI * 2);//расчет угла отражения
            t = ray(ptAns, an);
            glVertex2d(ptAns.x, ptAns.y);
            glVertex2d(t.x, t.y);

            an2 = ochko.refraction(i, ptAns, 1.45);
            t = ray(ptAns, an2);
            glColor4d(c.r, 0, 0, c.a);
            glVertex2d(ptAns.x, ptAns.y);
            glVertex2d(t.x, t.y);
            an2 = ochko.refraction(i, ptAns, 1.5);
            t = ray(ptAns, an2);
            glColor4d(0, c.g, 0, c.a);
            glVertex2d(ptAns.x, ptAns.y);
            glVertex2d(t.x, t.y);
            an2 = ochko.refraction(i, ptAns, 1.55);
            t = ray(ptAns, an2);
            glColor4d(0, 0, c.b, c.a);
            glVertex2d(ptAns.x, ptAns.y);
            glVertex2d(t.x, t.y);


            glColor4d(c.r, c.g, c.b, c.a/2);
            if (dist(point(x, y), ochko.center) < ochko.r)
            {
                //glVertex2d(x, y);
                //glVertex2d(t1.x, t1.y);
                ptAns = t1;
            }
            else if (dist(point(x, y), t1) < dist(point(x, y), t2))
            {
                glVertex2d(x, y);
                glVertex2d(t1.x, t1.y);
                ptAns = t1;
            }
            else
            {
                glVertex2d(x, y);
                glVertex2d(t2.x, t2.y);
                ptAns = t2;
            }

            if (dist(point(x, y), ochko.center) >= ochko.r)
            {
                an = ochko.reflection(ptAns, i);//mod(2 * huialka.angle - i, PI * 2);//расчет угла отражения
                t = ray(ptAns, an);
                glVertex2d(ptAns.x, ptAns.y);
                glVertex2d(t.x, t.y);
                //t2:
                //glVertex2d(x, y);
                //glVertex2d(t2.x, t2.y);//прямая от начала до пересечения
                //an = ochko.reflection(t2, i);//mod(2 * huialka.angle - i, PI * 2);//расчет угла отражения
                //t = ray(t2, an);
                //glVertex2d(t2.x, t2.y);
                //glVertex2d(t.x, t.y);

                //преломление:
                an2 = ochko.refraction(i, ptAns, 1.45);
                t = ray(ptAns, an2);
                glColor4d(c.r, 0, 0, c.a);
                glVertex2d(ptAns.x, ptAns.y);
                glVertex2d(t.x, t.y);
                an2 = ochko.refraction(i, ptAns, 1.5);
                t = ray(ptAns, an2);
                glColor4d(0, c.g, 0, c.a);
                glVertex2d(ptAns.x, ptAns.y);
                glVertex2d(t.x, t.y);
                an2 = ochko.refraction(i, ptAns, 1.55);
                t = ray(ptAns, an2);
                glColor4d(0, 0, c.b, c.a);
                glVertex2d(ptAns.x, ptAns.y);
                glVertex2d(t.x, t.y);
            }
        }
        else
        {
            glVertex2d(x, y);
            glVertex2d(t.x, t.y);
        }
    }
    glEnd();
}
krushochek obs1;
krushochek obs2;
krushochek obs3;
krushochek obs4;
const int num_of_obj = 2;
struct intersection
{
public:
    int id;
    point p,ptwo;
    bool isCrossin,isInside;
    double reflA, refrR, refrG, refrB;
    double reflA2, refrR2, refrG2, refrB2;
    intersection()
    {
        p = ptwo = point();
        reflA = refrR = refrG = refrB = reflA2 = refrR2 = refrG2 = refrB2 = 0;
        isCrossin = false;
        //id = -1;
    }
    //intersection(point IntrPt,double reflAng,double refrAng):p(IntrPt),reflA(reflAng),refrR(refrAng){}
    intersection(point p1, point p2,double AngleIn, palka palka)
    {
        //id = 1;
        isCrossin = palka.is_crossing(p1, p2);
        if (isCrossin)
        {
            p = palka.where_crossing(p1, p2);
            reflA = palka.reflection(AngleIn);
            refrR = palka.refraction(AngleIn, 1.45);
            refrG = palka.refraction(AngleIn, 1.5);
            refrB = palka.refraction(AngleIn,1.55);
        }
    }
    intersection(point p1, point p2, double AngleIn, krushochek krug/*,col c*/)
    {
        id = 2;
        isCrossin = krug.is_crossing(p1, p2);
        /*if (isCrossin)
        {
            point t1, t2;
            krug.whereCrossing(p1, p2, t1, t2);
            if (dist(p1, krug.center) < krug.r)
            {
                p = t1;
                reflA = krug.reflection(p, AngleIn);
            }
            else if (dist(p1, t1) > dist(p1, t2))
            {
                p = t1;
                reflA = krug.reflection(t1, AngleIn);
            }
            else
            {
                p = t2;
                reflA = krug.reflection(t2, AngleIn);
            }
            refrR = krug.refraction(AngleIn, p, 1.45);
            refrG = krug.refraction(AngleIn, p, 1.5);
            refrB = krug.refraction(AngleIn, p, 1.55);
        }*/
        point t,t1, t2;
        //double an, an2;
        if (isCrossin)
        {//если пересечение между лучем и палкой есть, то отрабатываем:

            krug.whereCrossing(p1, p2, t1, t2);
            //glColor4d(c.r, c.g, c.b, c.a / 2);
            isInside = false;
            if (dist(point(p1.x, p1.y), krug.center) < krug.r)
            {
                //glVertex2d(p1.x, p1.y);
                //glVertex2d(t1.x, t1.y);
                isInside = true;
                p = t1;
            }
            else if (dist(point(p1.x, p1.y), t1) > dist(point(p1.x, p1.y), t2))
            {
                //glVertex2d(p1.x, p1.y);
                //glVertex2d(t1.x, t1.y);
                p = t1;
            }
            else
            {
                //glVertex2d(p1.x, p1.y);
                //glVertex2d(t2.x, t2.y);
                p = t2;
            }
            reflA = krug.reflection(p, AngleIn);//mod(2 * huialka.angle - i, PI * 2);//расчет угла отражения
            //t = ray(p, reflA);
            //glVertex2d(p.x, p.y);
            //glVertex2d(t.x, t.y);

            refrR = krug.refraction(AngleIn, p, 1.45);
            //t = ray(p, refrR);
            //glColor4d(c.r, 0, 0, c.a);
            //glVertex2d(p.x, p.y);
            //glVertex2d(t.x, t.y);
            refrG = krug.refraction(AngleIn, p, 1.5);
            //t = ray(p, refrG);
            //glColor4d(0, c.g, 0, c.a);
            //glVertex2d(p.x, p.y);
            //glVertex2d(t.x, t.y);
            refrB = krug.refraction(AngleIn, p, 1.55);
            //t = ray(p, refrB);
            //glColor4d(0, 0, c.b, c.a);
            //glVertex2d(p.x, p.y);
            //glVertex2d(t.x, t.y);


            //glColor4d(c.r, c.g, c.b, c.a / 2);
            if (isInside)
            {
                ////glVertex2d(x, y);
                ////glVertex2d(t1.x, t1.y);
                ptwo = t1;
            }
            else if (dist(point(p1.x, p1.y), t1) < dist(point(p1.x, p1.y), t2))
            {
                //glVertex2d(p1.x, p1.y);
                //glVertex2d(t1.x, t1.y);
                ptwo = t1;
            }
            else
            {
                //glVertex2d(p1.x, p1.y);
                //glVertex2d(t2.x, t2.y);
                ptwo = t2;
            }

            if (!isInside)
            {
                reflA2 = krug.reflection(ptwo, AngleIn);//mod(2 * huialka.angle - i, PI * 2);//расчет угла отражения
                //t = ray(p, reflA);
                //glVertex2d(p.x, p.y);
                //glVertex2d(t.x, t.y);
                //преломление:
                refrR2 = krug.refraction(AngleIn, ptwo, 1.45);
                //t = ray(p, refrR);
                //glColor4d(c.r, 0, 0, c.a);
                //glVertex2d(p.x, p.y);
                //glVertex2d(t.x, t.y);
                refrG2 = krug.refraction(AngleIn, ptwo, 1.5);
                //t = ray(p, refrG);
                //glColor4d(0, c.g, 0, c.a);
                //glVertex2d(p.x, p.y);
                //glVertex2d(t.x, t.y);
                refrB2 = krug.refraction(AngleIn, ptwo, 1.55);
                //t = ray(p, refrB);
                //glColor4d(0, 0, c.b, c.a);
                //glVertex2d(p.x, p.y);
                //glVertex2d(t.x, t.y);
            }
        }

    }
};
void SendRay(point p,double angle,col c,int n)
{//n - лимит рекурсии
    if (n < 6)
    {
        point pt = ray(p,angle),pone,ptwo;
        //point t1[num_of_obj];
        intersection *t1;
        t1 = new intersection[4];
        //point tt1,tt2;
        int num=0;
        //bool isCrossin = obs1.is_crossing(p, pt);
        
        /*point t, t1, t2;
        //double an, an2;
        if (isCrossin)
        {//если пересечение между лучем и палкой есть, то отрабатываем:

            obs1.whereCrossing(p, pt, t1, t2);
            //glColor4d(c.r, c.g, c.b, c.a / 2);
            bool isInside = false;
            if (dist(p, obs1.center) < obs1.r)
            {
                glVertex2d(p.x, p.y);
                glVertex2d(t1.x, t1.y);
                isInside = true;
                pone = t1;
            }
            else if (dist(p, t1) > dist(p, t2))
            {
                glVertex2d(p.x, p.y);
                glVertex2d(t1.x, t1.y);
                pone = t1;
            }
            else
            {
                glVertex2d(p.x, p.y);
                glVertex2d(t2.x, t2.y);
                pone = t2;
            }
            double reflA = obs1.reflection(p, angle);//mod(2 * huialka.angle - i, PI * 2);//расчет угла отражения
            glColor4d(c.r, c.g, c.b, c.a);
            SendRay(pone, reflA, col(c.r, c.g, c.b, c.a), n + 1);
            //t = ray(p, reflA);
            //glVertex2d(p.x, p.y);
            //glVertex2d(t.x, t.y);

            double refrR = obs1.refraction(angle, p, 1.45);
            glColor4d(c.r, 0, 0, c.a);
            SendRay(pone, refrR, col(c.r, 0, 0, c.a), n + 1);
            //t = ray(p, refrR);
            //glColor4d(c.r, 0, 0, c.a);
            //glVertex2d(p.x, p.y);
            //glVertex2d(t.x, t.y);
            double refrG = obs1.refraction(angle, p, 1.5);
            glColor4d(0, c.g, 0, c.a);
            SendRay(pone, refrG, col(0, c.g, 0, c.a), n + 1);
            //t = ray(p, refrG);
            //glColor4d(0, c.g, 0, c.a);
            //glVertex2d(p.x, p.y);
            //glVertex2d(t.x, t.y);
            double refrB = obs1.refraction(angle, p, 1.55);
            glColor4d(0, 0, c.b, c.a);
            SendRay(pone, refrB, col(0, 0, c.b, c.a), n + 1);
            //t = ray(p, refrB);
            //glColor4d(0, 0, c.b, c.a);
            //glVertex2d(p.x, p.y);
            //glVertex2d(t.x, t.y);



            
            glColor4d(c.r, c.g, c.b, c.a / 2);
            if (isInside)
            {
                //glVertex2d(x, y);
                //glVertex2d(t1.x, t1.y);
                ptwo = t1;
            }
            else if (dist(p, t1) < dist(p, t2))
            {
                glVertex2d(p.x, p.y);
                glVertex2d(t1.x, t1.y);
                ptwo = t1;
            }
            else
            {
                glVertex2d(p.x, p.y);
                glVertex2d(t2.x, t2.y);
                ptwo = t2;
            }

            if (!isInside)
            {
                double reflA2 = obs1.reflection(ptwo, angle);//mod(2 * huialka.angle - i, PI * 2);//расчет угла отражения
                glColor4d(c.r, c.g, c.b, c.a);
                SendRay(ptwo, reflA2, col(c.r, c.g, c.b, c.a), n + 1);
                //t = ray(p, reflA);
                //glVertex2d(p.x, p.y);
                //glVertex2d(t.x, t.y);
                //преломление:
                double refrR2 = obs1.refraction(angle, ptwo, 1.45);
                glColor4d(c.r, 0, 0, c.a);
                SendRay(ptwo, refrR2, col(c.r, 0, 0, c.a), n + 1);
                //t = ray(p, refrR);
                //glColor4d(c.r, 0, 0, c.a);
                //glVertex2d(p.x, p.y);
                //glVertex2d(t.x, t.y);
                double refrG2 = obs1.refraction(angle, ptwo, 1.5);
                glColor4d(0, c.g, 0, c.a);
                SendRay(ptwo, refrG2, col(0, c.g, 0, c.a), n + 1);
                //t = ray(p, refrG);
                //glColor4d(0, c.g, 0, c.a);
                //glVertex2d(p.x, p.y);
                //glVertex2d(t.x, t.y);
                double refrB2 = obs1.refraction(angle, ptwo, 1.55);
                glColor4d(0, 0, c.b, c.a);
                SendRay(ptwo, refrB2, col(0, 0, c.b, c.a), n + 1);
                //t = ray(p, refrB);
                //glColor4d(0, 0, c.b, c.a);
                //glVertex2d(p.x, p.y);
                //glVertex2d(t.x, t.y);



            }
        }
        else
        {
            glVertex2d(p.x, p.y);
            glVertex2d(pt.x, pt.y);
        }*/
        //while (obs1)
        {//просматриваем все объекты "палки"
            //t1/*[num]*/ = intersection(p, pt, angle, obs1, c);
            //t2/*[num]*/ = intersection(p, pt, angle, obs2, c);
            //t3/*[num]*/ = intersection(p, pt, angle, obs3, c);
            //intersection othr;
            //othr.p = ...;
            //krushochek kother;
            //if(t1.isCrossin)

            //if (t1/*[num]*/.isCrossin)
            //{
            //    if (t1/*[num]*/.id == 2)
            //    {
            //        glVertex2d(p.x, p.y);
            //        glVertex2d(t1/*[num]*/.p.x, t1/*[num]*/.p.y);
            //        glColor4d(c.r, c.g, c.b, c.a);
            //        SendRay(t1/*[num]*/.p, t1/*[num]*/.reflA, col(c.r, c.g, c.b, c.a), n + 1);
            //        glColor4d(c.r, 0, 0, c.a);
            //        SendRay(t1/*[num]*/.p, t1/*[num]*/.refrR, col(c.r, 0, 0, c.a), n + 1);
            //        glColor4d(0, c.g, 0, c.a);
            //        SendRay(t1/*[num]*/.p, t1/*[num]*/.refrG, col(0, c.g, 0, c.a), n + 1);
            //        glColor4d(0, 0, c.b, c.a);
            //        SendRay(t1/*[num]*/.p, t1/*[num]*/.refrB, col(0, 0, c.b, c.a), n + 1);
            //        if (!t1/*[num]*/.isInside)
            //        {
            //            glColor4d(c.r, c.g, c.b, c.a);
            //            if (n != 0)
            //            {
            //                glVertex2d(p.x, p.y);
            //                glVertex2d(t1/*[num]*/.ptwo.x, t1/*[num]*/.ptwo.y);
            //            }
            //            glColor4d(c.r, c.g, c.b, c.a);
            //            SendRay(t1/*[num]*/.ptwo, t1/*[num]*/.reflA2, col(c.r, c.g, c.b, c.a), n + 1);
            //            glColor4d(c.r, 0, 0, c.a);
            //            SendRay(t1/*[num]*/.ptwo, t1/*[num]*/.refrR2, col(c.r, 0, 0, c.a), n + 1);
            //            glColor4d(0, c.g, 0, c.a);
            //            SendRay(t1/*[num]*/.ptwo, t1/*[num]*/.refrG2, col(0, c.g, 0, c.a), n + 1);
            //            glColor4d(0, 0, c.b, c.a);
            //            SendRay(t1/*[num]*/.ptwo, t1/*[num]*/.refrB2, col(0, 0, c.b, c.a), n + 1);
            //        }
            //    }
            //    num++;
            //}
            //else
            //{
            //    glVertex2d(p.x, p.y);
            //    glVertex2d(pt.x, pt.y);
            //}

            //obs1++;
        }

        //while (obs2)
        {//просматриваем все объекты "кружочки"
            t1[num] = intersection(p, pt, angle, obs1);
            if (t1[num].isCrossin)num++;
            t1[num] = intersection(p, pt, angle, obs2);
            if (t1[num].isCrossin)num++;
            //obs2++;
        }

        //t1[num] = intersection(p, pt, angle, obs3);
        //if (t1[num].isCrossin)num++;

        //t1[num] = intersection(p, pt, angle, obs4);
        //if (t1[num].isCrossin)num++;
        
        
        intersection resT;
        resT.p = pt;
        bool flag = false;
        for (int i = 0; i < num; i++)
        {//находим минимальное расстояние до точек пересечения:
            if (dist(p,t1[i].p)<dist(resT.p,p))
                //(p.x - t1[i].p.x) * (p.x - t1[i].p.x) + (p.y - t1[i].p.y) * (p.y - t1[i].p.y) <
                //(resT.x - p.x) * (resT.x - p.x) + (resT.y - p.y) * (resT.y - p.y))
            {
                resT = t1[i];
                flag = true;
            }
        }
        delete[]t1;
        //if (c.r == 0 || c.g == 0 || c.b == 0)
        {
            glVertex2d(p.x, p.y);
            glVertex2d(resT.p.x, resT.p.y);
        }
        if (flag)
        {
            //c.a = c.a / 2;
            glColor4d(c.r, c.g, c.b, c.a);
            SendRay(resT.p, resT.reflA, c, n + 1);
            glColor4d(c.r, 0, 0, c.a);
            SendRay(resT.p, resT.refrR, col(c.r, 0, 0, c.a), n + 1);
            glColor4d(0, c.g, 0, c.a);
            SendRay(resT.p, resT.refrG, col(0, c.g, 0, c.a), n + 1);
            glColor4d(0, 0, c.b, c.a);
            SendRay(resT.p, resT.refrB, col(0, 0, c.b, c.a), n + 1);
            if (resT.id==2&&!resT.isInside)
            {
                glColor4d(c.r, c.g, c.b, c.a);
                SendRay(resT.ptwo, resT.reflA2, c, n + 1);
                glColor4d(c.r, 0, 0, c.a);
                SendRay(resT.ptwo, resT.refrR2, col(c.r, 0, 0, c.a), n + 1);
                glColor4d(0, c.g, 0, c.a);
                SendRay(resT.ptwo, resT.refrG2, col(0, c.g, 0, c.a), n + 1);
                glColor4d(0, 0, c.b, c.a);
                SendRay(resT.ptwo, resT.refrB2, col(0, 0, c.b, c.a), n + 1);
            }
        }
    }
}
void SendRay2(point PtIn,double AngleIn,col c)
{
    int n = 3;//максимальная глубина отражений
    point t,t1,t2,ptAns;
    double an, an2;
    bool flag = false;
    double minDist;
    point w1, w2;//для точек пересечения
    krushochek kIntrs;
    for (int i = 0; i < n; i++)
    {
        glColor4d(c.r, c.g, c.b, c.a);
        t = ray(PtIn, AngleIn);
        //TODO:накинуть цикл по всем объектам
        minDist = dist(PtIn, t);
        if (obs1.is_crossing(PtIn, t))
        {
            obs1.whereCrossing(PtIn, t,w1,w2);
            if (minDist > dist(PtIn, w1))
            {
                minDist = dist(PtIn, w1);
                kIntrs = obs1;
                ptAns = w1;
            }
            if (minDist > dist(PtIn, w2))
            {
                minDist = dist(PtIn, w2);
                kIntrs = obs1;
                ptAns = w2;
            }
            flag = true;
        }
        if (obs2.is_crossing(PtIn, t))
        {
            obs2.whereCrossing(PtIn, t, w1, w2);
            if (minDist > dist(PtIn, w1))
            {
                minDist = dist(PtIn, w1);
                kIntrs = obs2;
                ptAns = w1;
            }
            if (minDist > dist(PtIn, w2))
            {
                minDist = dist(PtIn, w2);
                kIntrs = obs2;
                ptAns = w2;
            }
            flag = true;
        }
        if (flag)
        {//если пересечение есть, то отрабатываем:
            glColor4d(c.r, c.g, c.b, c.a / 2);
            glVertex2d(PtIn.x, PtIn.y);
            glVertex2d(ptAns.x, ptAns.y);
            
            an = obs1.reflection(ptAns, AngleIn);//mod(2 * huialka.angle - AngleIn, PI * 2);//расчет угла отражения
            //---------------
            t = ray(ptAns, an);
            glVertex2d(ptAns.x, ptAns.y);
            glVertex2d(t.x, t.y);
            //---------------
            an2 = obs1.refraction(AngleIn, ptAns, 1.45);
            //---------------
            t = ray(ptAns, an2);
            glColor4d(c.r, 0, 0, c.a);
            glVertex2d(ptAns.x, ptAns.y);
            glVertex2d(t.x, t.y);
            //---------------
            an2 = obs1.refraction(AngleIn, ptAns, 1.5);
            //---------------
            t = ray(ptAns, an2);
            glColor4d(0, c.g, 0, c.a);
            glVertex2d(ptAns.x, ptAns.y);
            glVertex2d(t.x, t.y);
            //---------------
            an2 = obs1.refraction(AngleIn, ptAns, 1.55);
            //---------------
            t = ray(ptAns, an2);
            glColor4d(0, 0, c.b, c.a);
            glVertex2d(ptAns.x, ptAns.y);
            glVertex2d(t.x, t.y);
            //---------------

            glColor4d(c.r, c.g, c.b, c.a / 2);
            if (dist(PtIn, obs1.center) < obs1.r)
            {
                //glVertex2d(x, y);
                //glVertex2d(t1.x, t1.y);
                ptAns = t1;
            }
            else if (dist(PtIn, t1) < dist(PtIn, t2))
            {
                glVertex2d(PtIn.x, PtIn.y);
                glVertex2d(t1.x, t1.y);
                ptAns = t1;
            }
            else
            {
                glVertex2d(PtIn.x, PtIn.y);
                glVertex2d(t2.x, t2.y);
                ptAns = t2;
            }

            if (dist(PtIn, obs1.center) >= obs1.r)
            {
                an = obs1.reflection(ptAns, AngleIn);//mod(2 * huialka.angle - AngleIn, PI * 2);//расчет угла отражения
                t = ray(ptAns, an);
                glVertex2d(ptAns.x, ptAns.y);
                glVertex2d(t.x, t.y);
                //t2:
                //glVertex2d(x, y);
                //glVertex2d(t2.x, t2.y);//прямая от начала до пересечения
                //an = ochko.reflection(t2, AngleIn);//mod(2 * huialka.angle - AngleIn, PI * 2);//расчет угла отражения
                //t = ray(t2, an);
                //glVertex2d(t2.x, t2.y);
                //glVertex2d(t.x, t.y);

                //преломление:
                an2 = obs1.refraction(AngleIn, ptAns, 1.45);
                t = ray(ptAns, an2);
                glColor4d(c.r, 0, 0, c.a);
                glVertex2d(ptAns.x, ptAns.y);
                glVertex2d(t.x, t.y);
                an2 = obs1.refraction(AngleIn, ptAns, 1.5);
                t = ray(ptAns, an2);
                glColor4d(0, c.g, 0, c.a);
                glVertex2d(ptAns.x, ptAns.y);
                glVertex2d(t.x, t.y);
                an2 = obs1.refraction(AngleIn, ptAns, 1.55);
                t = ray(ptAns, an2);
                glColor4d(0, 0, c.b, c.a);
                glVertex2d(ptAns.x, ptAns.y);
                glVertex2d(t.x, t.y);
            }
        }
        else
        {
            glVertex2d(PtIn.x, PtIn.y);
            glVertex2d(t.x, t.y);
        }
    }
}
void herachitVoVseStorons(point p,col c)
{
    double d = PI / 180;
    glBegin(GL_LINES);
    for (double i = 0; i < PI * 2; i += d)
    {
        glColor4d(c.r, c.g, c.b, c.a);
        SendRay2(p, i, c);
    }
    glEnd();
}
void eboshitVOdnuStoronu(double x, double y, col c,double ang)
{
    double d = 0.1, an, an2,x1,y1;
    glBegin(GL_LINES);
    point t, t1;
    palka huialka = palka(point(0, 0.2), point(0, 0));
    for (double i = 0; i < d; i += d/200)
    {
        glColor4d(c.r, c.g, c.b, c.a);
        x1 = x + i * cos(ang + PI / 2);
        y1 = y + i*sin(ang + PI / 2);//начальная точка луча
        glVertex2d(x1, y1);
        t = ray(point(x1, y1), ang);//точка пересечения луча и края карты
        //отражение:
        if (huialka.is_crossing(point(x1, y1), t))
        {//если пересечение между лучем и палкой есть, то отрабатываем:
            t1 = huialka.where_crossing(point(x1, y1), t);
            glVertex2d(t1.x, t1.y);//прямая от начала до пересечения
            an = mod(2 * huialka.angle - ang, PI * 2);//расчет угла отражения
            t = ray(t1, an);
            glVertex2d(t1.x, t1.y);
            glVertex2d(t.x, t.y);
            //преломление:
            an2 = huialka.refraction(ang, 1.45);
            t = ray(t1, an2);
            glColor4d(c.r, 0, 0, c.a);
            glVertex2d(t1.x, t1.y);
            glVertex2d(t.x, t.y);
            an2 = huialka.refraction(ang, 1.5);
            t = ray(t1, an2);
            glColor4d(0, c.g, 0, c.a);
            glVertex2d(t1.x, t1.y);
            glVertex2d(t.x, t.y);
            an2 = huialka.refraction(ang, 1.55);
            t = ray(t1, an2);
            glColor4d(0, 0, c.b, c.a);
            glVertex2d(t1.x, t1.y);
            glVertex2d(t.x, t.y);
        }
        else
            glVertex2d(t.x, t.y);
    }
    glEnd();
}
//
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

        eboshitVoVseStorons2(msX, msY, col(1, 1, 1, 0.08));
        //eboshitVOdnuStoronu(msX, msY, col(1, 1, 1, 0.1),PI/3);
        //eboshitVoVseStorons(msX, msY, col(1, 1, 1, 0.5));
        //herachitVoVseStorons(point(msX, msY), col(1, 1, 1, 0.05));

        glColor3d(1, 1, 1);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //for (double i = 0; i < 2 * PI; i += PI / 10)
    //    std::cout << tan(i) << "\t"<<i<<"\n";
    

}
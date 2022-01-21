#pragma once
#include <iostream>
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
        double t1, t2;                                 //p1=center => 
        double A=((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1));  //...
        double B=2*((x2-x1)*(x1-x0) + (y2-y1)*(y1-y0));//0
        double C=(x1-x0)*(x1-x0)+(y1-y0)*(y1-y0)-r*r;  //0 => D=0!!!
        //if (B <= 0 && C <= 0) { B = C = 0; }
        double D = (B * B) - (4 * A * C);
        t1 = (-B + sqrt(D)) / (2 * A);
        t2 = (-B - sqrt(D)) / (2 * A);
        if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1)
        {
            answer1 = point(x1 + (x2 - x1) * t1, y1 + (y2 - y1) * t1);
            answer2 = point(x1 + (x2 - x1) * t2, y1 + (y2 - y1) * t2);
            return 2;
        }
        else if (t1 >= 0 && t1 <= 1)
        {
            answer1 = point(x1 + (x2 - x1) * t1, y1 + (y2 - y1) * t1);
            answer2 = point(0, 0);
            return 1;
        }
        else if (t2 >= 0 && t2 <= 1)
        {
            answer1 = point(x1 + (x2 - x1) * t2, y1 + (y2 - y1) * t2);
            answer2 = point(0, 0);
            return 1;
        }
        else
            return 0;
        //std::cout << D<<"               \r";
        //if (D == 0)
        //{
        //    t1 = (-B + sqrt(D)) / (2 * A);
        //    //t2 = (-B - sqrt(D)) / (2 * A);
        //    answer1 = point(x1 + (x2 - x1) * t1, y1 + (y2 - y1) * t1);
        //    //answer2 = point(x1 + (x2 - x1) * t2, y1 + (y2 - y1) * t2);
        //    return 1;
        //}
        //else if (D > 0)
        //{
        //    t1 = (-B + sqrt(D)) / (2 * A);
        //    t2 = (-B - sqrt(D)) / (2 * A);
        //    answer1 = point(x1 + (x2 - x1) * t1, y1 + (y2 - y1) * t1);
        //    answer2 = point(x1 + (x2 - x1) * t2, y1 + (y2 - y1) * t2);
        //    return 2;
        //}
        //else
        //    return 0;
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
        {//если пересечение между лучем и кружком есть, то отрабатываем:

            ochko.whereCrossing(point(x, y), t, t1, t2);
            glColor4d(c.r, c.g, c.b, c.a/2);
            if (dist(point(x, y), ochko.center) < ochko.r)
            {//если (х у) внутри круга
                glVertex2d(x, y);
                glVertex2d(t1.x, t1.y);
                ptAns = t1;
            }
            else if (dist(point(x, y), t1) > dist(point(x, y), t2))
            {//если к t1 ближе 
                glVertex2d(x, y);
                glVertex2d(t1.x, t1.y);
                ptAns = t1;
            }
            else
            {//если к t2 ближе
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
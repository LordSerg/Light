#pragma once
//класс Луча, каждый объект которого хранит цвет, т. начала и 5 других лучей
krushochek* prepatstvia;
int numOfPrepatstviy;

class Ray
{
    static int const maxDepth = 1;//----------------------------------------важная константа
public:
    int id; //0 - для отражения
            //1 - для приломления
    col c;//цвет луча
    point beg;//начальная точка
    point end;//конечная точка
    double angle;//направление луча
    int depth;//текущая глубина рекурсии луча
    Ray *rays;//отражение, приломление (3 шт), прохождение дальше
    Ray()
    {
        c = col();
        beg = point();
        end = point();
        //rays = new Ray[5];
        angle = 0;
        depth = 0;
        id = 0;
        //for (int i = 0; i < 5; i++)rays[i] = Ray();
    }
    //резервная копия на всякий случай:
    /*
    Ray(point Begin, col color, double Angle, int Depth, int ID)
    {//надо как-то передать все объекты, которые надо преодолеть лучу
    depth = Depth;
    id = ID;
    if (depth < maxDepth)
    {//ставим ограничение на глубину рекурсии
        c = color;
        beg = Begin;
        angle = Angle;
        point peresechenie = point();
        //находим точку пересечения с объектами
        peresechenie = ray(beg, angle);//нашли точку пересечения луча с границей экрана
        end = peresechenie;
        bool flag = false;
        int j = 0;
        point t1, t2, ptAnsLocal, ptAnsMin;
        krushochek govno;//объект, с которым в итоге взаимодействуем
        int zz = 0;
        for (int j = 0; j < numOfPrepatstviy; j++)
            if (prepatstvia[j].is_crossing(beg, peresechenie))
            {//находим первое пересечение с каким-либо препятствием
                zz = prepatstvia[j].whereCrossing(beg, peresechenie, t1, t2);
                govno = prepatstvia[j];
                flag = true;
                if (id == 0)
                {//случай отражения
                    if (zz == 2)
                    {
                        if (dist(beg, prepatstvia[j].center) <= prepatstvia[j].r + epsilon)
                        {//если beg внутри круга
                            ptAnsMin = t1;
                        }
                        else if (dist(beg, t1) < dist(beg, t2) && dist(beg, t1) > epsilon)
                        {//если к t1 ближе 
                            ptAnsMin = t2;
                        }
                        else if (abs(dist(beg, prepatstvia[j].center) - prepatstvia[j].r) > epsilon)
                        {//если к t2 ближе
                            ptAnsMin = t2;
                        }
                        else
                            ptAnsMin = t2;
                    }
                    else if (zz == 1)
                    {
                        if (dist(beg, t1) > epsilon)
                            ptAnsMin = t1;
                        else
                        {
                            flag = false;
                        }
                    }
                    if (flag)break;
                }
                else if (id == 1)
                {//случай приломления
                    if (zz == 2)
                    {
                        if (dist(beg, prepatstvia[j].center) <= prepatstvia[j].r + epsilon)
                        {//если beg внутри круга
                            ptAnsMin = t1;
                        }
                        else if (dist(beg, t1) < dist(beg, t2) && dist(beg, t1) > epsilon)
                        {//если к t1 ближе 
                            ptAnsMin = t1;
                        }
                        else if (abs(dist(beg, prepatstvia[j].center) - prepatstvia[j].r) > epsilon)
                        {//если к t2 ближе
                            ptAnsMin = t1;
                        }
                        else
                            ptAnsMin = t2;
                    }
                    else if (zz == 1)
                    {
                        if (dist(beg, t1) > epsilon)
                            ptAnsMin = t1;
                        else
                        {
                            flag = false;
                        }
                    }
                    if (flag)break;
                }
            }
        //если точка пересечения - край, а не объект, то заканчиваем этот луч, не разделяя его;
        //иначе разделяем луч:
        if (!flag)
        {
            glColor4d(c.r, c.g, c.b, c.a);
            glVertex2d(beg.x, beg.y);
            glVertex2d(end.x, end.y);
        }
        else if (flag)
        {
            //ищем ближайшую точку пересечения с окружностями
            for (int i = j; i < numOfPrepatstviy; i++)
            {
                if (prepatstvia[i].is_crossing(beg, peresechenie))
                {
                    zz = prepatstvia[i].whereCrossing(beg, peresechenie, t1, t2);
                    if (id == 0)
                    {//случай отражения
                        if (zz == 2)
                        {
                            if (dist(beg, prepatstvia[i].center) <= prepatstvia[i].r + epsilon)
                            {//если beg внутри круга
                                ptAnsLocal = t1;
                            }
                            else if (dist(beg, t1) < dist(beg, t2) && dist(beg, t1) > epsilon)
                            {//если к t1 ближе 
                                ptAnsLocal = t2;
                            }
                            else if (abs(dist(beg, prepatstvia[i].center) - prepatstvia[i].r) > epsilon)
                            {//если к t2 ближе
                                ptAnsLocal = t2;
                            }
                            else
                                ptAnsLocal = t2;
                        }
                        else if (zz == 1)
                        {
                            if (dist(beg, t1) < epsilon)
                                ptAnsLocal = t1;
                            else
                                ptAnsLocal = point(1000, 1000);
                        }
                        if (dist(beg, ptAnsMin) > dist(beg, ptAnsLocal) && dist(beg, ptAnsLocal) > epsilon)
                        {
                            ptAnsMin = ptAnsLocal;
                            govno = prepatstvia[i];
                        }
                    }
                    else if (id == 1)
                    {//случай преломления
                        if (zz == 2)
                        {
                            if (dist(beg, prepatstvia[i].center) <= prepatstvia[i].r + epsilon)
                            {//если beg внутри круга
                                ptAnsLocal = t1;
                            }
                            else if (dist(beg, t1) < dist(beg, t2) && dist(beg, t1) > epsilon)
                            {//если к t1 ближе 
                                ptAnsLocal = t1;
                            }
                            else if (abs(dist(beg, prepatstvia[i].center) - prepatstvia[i].r) > epsilon)
                            {//если к t2 ближе
                                ptAnsLocal = t1;
                            }
                            else
                                ptAnsLocal = t2;
                        }
                        else if (zz == 1)
                        {
                            if (dist(beg, t1) < epsilon)
                                ptAnsLocal = t1;
                            else
                                ptAnsLocal = point(1000, 1000);
                        }
                        if (dist(beg, ptAnsMin) > dist(beg, ptAnsLocal) && dist(beg, ptAnsLocal) > epsilon)
                        {
                            ptAnsMin = ptAnsLocal;
                            govno = prepatstvia[i];
                        }
                    }
                }
            }
            end = ptAnsMin;
            glColor4d(c.r, c.g, c.b, c.a);
            glVertex2d(beg.x, beg.y);
            glVertex2d(end.x, end.y);
            if (c.r > 0 && c.g > 0 && c.b > 0)
            {//если это не одноцветный луч, то делаем три преломления
                rays = new Ray[4];
                rays[0] = Ray(ptAnsMin,
                    c,
                    govno.reflection(ptAnsMin, angle),
                    depth + 1,
                    0);//отражение
                rays[1] = Ray(ptAnsMin,
                    col(c.r, 0, 0, c.a),
                    govno.refraction(angle, ptAnsMin, 1.45),
                    depth + 1,
                    1);//преломлениеR
                rays[2] = Ray(ptAnsMin,
                    col(0, c.g, 0, c.a),
                    govno.refraction(angle, ptAnsMin, 1.5),
                    depth + 1,
                    1);//преломлениеG
                rays[3] = Ray(ptAnsMin,
                    col(0, 0, c.b, c.a),
                    govno.refraction(angle, ptAnsMin, 1.55),
                    depth + 1,
                    1);//преломлениеB
                //rays[4] = Ray(ptAnsMin,
                //    c,
                //    angle,
                //    depth + 1);//продолжение
                //
                free(rays);
            }
            else
            {//если же это одноцветный луч, то и преломление только одно
                rays = new Ray[2];
                rays[0] = Ray(ptAnsMin,
                    c,
                    govno.reflection(ptAnsMin, angle),
                    depth + 1,
                    0);//отражение
                double k = 1.5;//коэф приломления
                if (c.r > 0)
                    k = 1.45;
                else if (c.g > 0)
                    k = 1.5;
                else if (c.b > 0)
                    k = 1.55;
                rays[1] = Ray(ptAnsMin,
                    c,
                    govno.refraction(angle, ptAnsMin, k),
                    depth + 1,
                    1);//преломление
                //rays[2] = Ray(ptAnsMin,
                //    c,
                //    angle,
                //    depth + 1);//продолжение
                free(rays);
            }
        }
        //glColor4d(c.r, c.g, c.b, c.a);
        //glVertex2d(beg.x, beg.y);
        //glVertex2d(end.x, end.y);

    }
    }
    */
    //Рекурсивно разбиваем луч:
    Ray(point Begin, col color/*, world*/, double Angle,int Depth,int ID)
    {//надо как-то передать все объекты, которые надо преодолеть лучу
        depth = Depth;
        id = ID;
        if (depth < maxDepth)
        {//ставим ограничение на глубину рекурсии
            c = color;
            beg = Begin;
            angle = Angle;
            point peresechenie = point();
            //находим точку пересечения с объектами
            peresechenie = ray(beg, angle);//нашли точку пересечения луча с границей экрана
            end = peresechenie;
            bool flag = false;
            int j = 0;
            point t1, t2, ptAnsLocal, ptAnsMin;
            krushochek govno;//объект, с которым в итоге взаимодействуем
            int zz = 0;
            for (int j = 0; j < numOfPrepatstviy; j++)
            {
                zz = prepatstvia[j].whereCrossing(beg, peresechenie, t1, t2);
                if (zz>0)
                {//находим первое пересечение с каким-либо препятствием
                    govno = prepatstvia[j];
                    flag = true;
                    if (id == 0)
                    {//случай отражения
                        if (zz == 2)
                        {
                            if (dist(beg, prepatstvia[j].center) <= prepatstvia[j].r + epsilon)
                            {//если beg внутри круга
                                ptAnsMin = t1;
                            }
                            else if (dist(beg, t1) < dist(beg, t2) && dist(beg, t1) > epsilon)
                            {//если к t1 ближе 
                                ptAnsMin = t2;
                            }
                            else if (abs(dist(beg, prepatstvia[j].center) - prepatstvia[j].r) > epsilon)
                            {//если к t2 ближе
                                ptAnsMin = t2;
                            }
                            else
                                ptAnsMin = t2;
                        }
                        else if (zz == 1)
                        {
                            if (dist(beg, t1) > epsilon)
                                ptAnsMin = t1;
                            else
                            {
                                flag = false;
                            }
                        }
                        if (flag)break;
                    }
                    else if (id == 1)
                    {//случай приломления
                        if (zz == 2)
                        {
                            if (dist(beg, prepatstvia[j].center) <= prepatstvia[j].r + epsilon)
                            {//если beg внутри круга
                                ptAnsMin = t1;
                            }
                            else if (dist(beg, t1) < dist(beg, t2) && dist(beg, t1) > epsilon)
                            {//если к t1 ближе 
                                ptAnsMin = t1;
                            }
                            else if (abs(dist(beg, prepatstvia[j].center) - prepatstvia[j].r) > epsilon)
                            {//если к t2 ближе
                                ptAnsMin = t1;
                            }
                            else
                                ptAnsMin = t2;
                        }
                        else if (zz == 1)
                        {
                            if (dist(beg, t1) > epsilon)
                                ptAnsMin = t1;
                            else
                            {
                                flag = false;
                            }
                        }
                        if (flag)break;
                    }
                }
            }
            //если точка пересечения - край, а не объект, то заканчиваем этот луч, не разделяя его;
            //иначе разделяем луч:
            if (!flag)
            {
                glColor4d(c.r, c.g, c.b, c.a);
                glVertex2d(beg.x, beg.y);
                glVertex2d(end.x, end.y);
            }
            else if (flag)
            {
                //ищем ближайшую точку пересечения с окружностями
                for (int i = j; i < numOfPrepatstviy; i++)
                {
                    if (prepatstvia[i].is_crossing(beg, peresechenie))
                    {
                        zz = prepatstvia[i].whereCrossing(beg, peresechenie, t1, t2);
                        if (id == 0)
                        {//случай отражения
                            if (zz == 2)
                            {
                                if (dist(beg, prepatstvia[i].center) <= prepatstvia[i].r + epsilon)
                                {//если beg внутри круга
                                    ptAnsLocal = t1;
                                }
                                else if (dist(beg, t1) < dist(beg, t2) && dist(beg, t1) > epsilon)
                                {//если к t1 ближе 
                                    ptAnsLocal = t2;
                                }
                                else if (abs(dist(beg, prepatstvia[i].center) - prepatstvia[i].r) > epsilon)
                                {//если к t2 ближе
                                    ptAnsLocal = t2;
                                }
                                else
                                    ptAnsLocal = t2;
                            }
                            else if (zz == 1)
                            {
                                if (dist(beg, t1) < epsilon)
                                    ptAnsLocal = t1;
                                else
                                    ptAnsLocal = point(1000, 1000);
                            }
                            if (dist(beg, ptAnsMin) > dist(beg, ptAnsLocal) && dist(beg, ptAnsLocal) > epsilon)
                            {
                                ptAnsMin = ptAnsLocal;
                                govno = prepatstvia[i];
                            }
                        }
                        else if (id == 1)
                        {//случай преломления
                            if (zz == 2)
                            {
                                if (dist(beg, prepatstvia[i].center) <= prepatstvia[i].r + epsilon)
                                {//если beg внутри круга
                                    ptAnsLocal = t1;
                                }
                                else if (dist(beg, t1) < dist(beg, t2) && dist(beg, t1) > epsilon)
                                {//если к t1 ближе 
                                    ptAnsLocal = t1;
                                }
                                else if (abs(dist(beg, prepatstvia[i].center) - prepatstvia[i].r) > epsilon)
                                {//если к t2 ближе
                                    ptAnsLocal = t1;
                                }
                                else
                                    ptAnsLocal = t2;
                            }
                            else if (zz == 1)
                            {
                                if (dist(beg, t1) < epsilon)
                                    ptAnsLocal = t1;
                                else
                                    ptAnsLocal = point(1000, 1000);
                            }
                            if (dist(beg, ptAnsMin) > dist(beg, ptAnsLocal) && dist(beg, ptAnsLocal) > epsilon)
                            {
                                ptAnsMin = ptAnsLocal;
                                govno = prepatstvia[i];
                            }
                        }
                    }
                }
                end= ptAnsMin;
                glColor4d(c.r, c.g, c.b, c.a);
                glVertex2d(beg.x, beg.y);
                glVertex2d(end.x, end.y);
                if (c.r > 0 && c.g > 0 && c.b > 0)
                {//если это не одноцветный луч, то делаем три преломления
                    rays = new Ray[4];
                    rays[0] = Ray(ptAnsMin,
                        c,
                        govno.reflection(ptAnsMin,angle),
                        depth + 1,
                        0);//отражение
                    rays[1] = Ray(ptAnsMin,
                        col(c.r,0,0,c.a),
                        govno.refraction(angle,ptAnsMin,1.45),
                        depth + 1,
                        1);//преломлениеR
                    rays[2] = Ray(ptAnsMin,
                        col(0,c.g,0,c.a),
                        govno.refraction(angle, ptAnsMin, 1.5),
                        depth + 1,
                        1);//преломлениеG
                    rays[3] = Ray(ptAnsMin,
                        col(0,0,c.b,c.a),
                        govno.refraction(angle, ptAnsMin, 1.55),
                        depth + 1,
                        1);//преломлениеB
                    //rays[4] = Ray(ptAnsMin,
                    //    c,
                    //    angle,
                    //    depth + 1);//продолжение
                    //
                    free(rays);
                }
                else
                {//если же это одноцветный луч, то и преломление только одно
                    rays = new Ray[2];
                    rays[0] = Ray(ptAnsMin,
                        c,
                        govno.reflection(ptAnsMin,angle),
                        depth + 1,
                        0);//отражение
                    double k=1.5;//коэф приломления
                    if (c.r > 0)
                        k = 1.45;
                    else if (c.g > 0)
                        k = 1.5;
                    else if (c.b > 0)
                        k = 1.55;
                    rays[1] = Ray(ptAnsMin,
                        c,
                        govno.refraction(angle, ptAnsMin, k),
                        depth + 1,
                        1);//преломление
                    //rays[2] = Ray(ptAnsMin,
                    //    c,
                    //    angle,
                    //    depth + 1);//продолжение
                    free(rays);
                }
            }
                //glColor4d(c.r, c.g, c.b, c.a);
                //glVertex2d(beg.x, beg.y);
                //glVertex2d(end.x, end.y);
            
        }
    }
    //Рекурсивно разбиваем луч, передав ПРЕДпоследнюю точку:(че)
    //
    /*~Ray()
    {
        if (depth > 0)
            delete rays;
    }*/
};
Ray* allRays;
void herachitLuchamiEbanutemi(double xStart,double yStart,col color,int numOfRays)
{//точечный источник
    point start = point(xStart, yStart);
    double ang = 0, dAng = 2 * PI / (double)numOfRays;
    glBegin(GL_LINES);
    for (int i = 0; i < numOfRays; i++)
    {
        allRays[i] = Ray(start, color, ang, 0, 0);
        ang += dAng;
    }
    glEnd();
}

void ebuchiyTochechniyIstochnik(double xStart, double yStart, col color, int numOfRays)
{
    point start = point(xStart, yStart);
    double ang = 0, dAng = 2 * PI / (double)numOfRays;
    glBegin(GL_LINES);
    for (int i = 0; i < numOfRays; i++)
    {
        allRays[i] = Ray(start, color, ang, 0, 0);
        ang += dAng;
    }
    glEnd();
}
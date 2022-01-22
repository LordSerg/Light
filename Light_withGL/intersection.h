#pragma once
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
    double d = PI / 181;
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

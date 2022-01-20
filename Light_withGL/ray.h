#pragma once
//����� ����, ������ ������ �������� ������ ����, �. ������ � 5 ������ �����
krushochek* prepatstvia;
int numOfPrepatstviy;

class Ray
{
    static int const maxDepth = 1;//----------------------------------------������ ���������
public:
    int id; //0 - ��� ���������
            //1 - ��� �����������
    col c;//���� ����
    point beg;//��������� �����
    point end;//�������� �����
    double angle;//����������� ����
    int depth;//������� ������� �������� ����
    Ray *rays;//���������, ����������� (3 ��), ����������� ������
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
    //��������� ����� �� ������ ������:
    /*
    Ray(point Begin, col color, double Angle, int Depth, int ID)
    {//���� ���-�� �������� ��� �������, ������� ���� ���������� ����
    depth = Depth;
    id = ID;
    if (depth < maxDepth)
    {//������ ����������� �� ������� ��������
        c = color;
        beg = Begin;
        angle = Angle;
        point peresechenie = point();
        //������� ����� ����������� � ���������
        peresechenie = ray(beg, angle);//����� ����� ����������� ���� � �������� ������
        end = peresechenie;
        bool flag = false;
        int j = 0;
        point t1, t2, ptAnsLocal, ptAnsMin;
        krushochek govno;//������, � ������� � ����� ���������������
        int zz = 0;
        for (int j = 0; j < numOfPrepatstviy; j++)
            if (prepatstvia[j].is_crossing(beg, peresechenie))
            {//������� ������ ����������� � �����-���� ������������
                zz = prepatstvia[j].whereCrossing(beg, peresechenie, t1, t2);
                govno = prepatstvia[j];
                flag = true;
                if (id == 0)
                {//������ ���������
                    if (zz == 2)
                    {
                        if (dist(beg, prepatstvia[j].center) <= prepatstvia[j].r + epsilon)
                        {//���� beg ������ �����
                            ptAnsMin = t1;
                        }
                        else if (dist(beg, t1) < dist(beg, t2) && dist(beg, t1) > epsilon)
                        {//���� � t1 ����� 
                            ptAnsMin = t2;
                        }
                        else if (abs(dist(beg, prepatstvia[j].center) - prepatstvia[j].r) > epsilon)
                        {//���� � t2 �����
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
                {//������ �����������
                    if (zz == 2)
                    {
                        if (dist(beg, prepatstvia[j].center) <= prepatstvia[j].r + epsilon)
                        {//���� beg ������ �����
                            ptAnsMin = t1;
                        }
                        else if (dist(beg, t1) < dist(beg, t2) && dist(beg, t1) > epsilon)
                        {//���� � t1 ����� 
                            ptAnsMin = t1;
                        }
                        else if (abs(dist(beg, prepatstvia[j].center) - prepatstvia[j].r) > epsilon)
                        {//���� � t2 �����
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
        //���� ����� ����������� - ����, � �� ������, �� ����������� ���� ���, �� �������� ���;
        //����� ��������� ���:
        if (!flag)
        {
            glColor4d(c.r, c.g, c.b, c.a);
            glVertex2d(beg.x, beg.y);
            glVertex2d(end.x, end.y);
        }
        else if (flag)
        {
            //���� ��������� ����� ����������� � ������������
            for (int i = j; i < numOfPrepatstviy; i++)
            {
                if (prepatstvia[i].is_crossing(beg, peresechenie))
                {
                    zz = prepatstvia[i].whereCrossing(beg, peresechenie, t1, t2);
                    if (id == 0)
                    {//������ ���������
                        if (zz == 2)
                        {
                            if (dist(beg, prepatstvia[i].center) <= prepatstvia[i].r + epsilon)
                            {//���� beg ������ �����
                                ptAnsLocal = t1;
                            }
                            else if (dist(beg, t1) < dist(beg, t2) && dist(beg, t1) > epsilon)
                            {//���� � t1 ����� 
                                ptAnsLocal = t2;
                            }
                            else if (abs(dist(beg, prepatstvia[i].center) - prepatstvia[i].r) > epsilon)
                            {//���� � t2 �����
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
                    {//������ �����������
                        if (zz == 2)
                        {
                            if (dist(beg, prepatstvia[i].center) <= prepatstvia[i].r + epsilon)
                            {//���� beg ������ �����
                                ptAnsLocal = t1;
                            }
                            else if (dist(beg, t1) < dist(beg, t2) && dist(beg, t1) > epsilon)
                            {//���� � t1 ����� 
                                ptAnsLocal = t1;
                            }
                            else if (abs(dist(beg, prepatstvia[i].center) - prepatstvia[i].r) > epsilon)
                            {//���� � t2 �����
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
            {//���� ��� �� ����������� ���, �� ������ ��� �����������
                rays = new Ray[4];
                rays[0] = Ray(ptAnsMin,
                    c,
                    govno.reflection(ptAnsMin, angle),
                    depth + 1,
                    0);//���������
                rays[1] = Ray(ptAnsMin,
                    col(c.r, 0, 0, c.a),
                    govno.refraction(angle, ptAnsMin, 1.45),
                    depth + 1,
                    1);//�����������R
                rays[2] = Ray(ptAnsMin,
                    col(0, c.g, 0, c.a),
                    govno.refraction(angle, ptAnsMin, 1.5),
                    depth + 1,
                    1);//�����������G
                rays[3] = Ray(ptAnsMin,
                    col(0, 0, c.b, c.a),
                    govno.refraction(angle, ptAnsMin, 1.55),
                    depth + 1,
                    1);//�����������B
                //rays[4] = Ray(ptAnsMin,
                //    c,
                //    angle,
                //    depth + 1);//�����������
                //
                free(rays);
            }
            else
            {//���� �� ��� ����������� ���, �� � ����������� ������ ����
                rays = new Ray[2];
                rays[0] = Ray(ptAnsMin,
                    c,
                    govno.reflection(ptAnsMin, angle),
                    depth + 1,
                    0);//���������
                double k = 1.5;//���� �����������
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
                    1);//�����������
                //rays[2] = Ray(ptAnsMin,
                //    c,
                //    angle,
                //    depth + 1);//�����������
                free(rays);
            }
        }
        //glColor4d(c.r, c.g, c.b, c.a);
        //glVertex2d(beg.x, beg.y);
        //glVertex2d(end.x, end.y);

    }
    }
    */
    //���������� ��������� ���:
    Ray(point Begin, col color/*, world*/, double Angle,int Depth,int ID)
    {//���� ���-�� �������� ��� �������, ������� ���� ���������� ����
        depth = Depth;
        id = ID;
        if (depth < maxDepth)
        {//������ ����������� �� ������� ��������
            c = color;
            beg = Begin;
            angle = Angle;
            point peresechenie = point();
            //������� ����� ����������� � ���������
            peresechenie = ray(beg, angle);//����� ����� ����������� ���� � �������� ������
            end = peresechenie;
            bool flag = false;
            int j = 0;
            point t1, t2, ptAnsLocal, ptAnsMin;
            krushochek govno;//������, � ������� � ����� ���������������
            int zz = 0;
            for (int j = 0; j < numOfPrepatstviy; j++)
            {
                zz = prepatstvia[j].whereCrossing(beg, peresechenie, t1, t2);
                if (zz>0)
                {//������� ������ ����������� � �����-���� ������������
                    govno = prepatstvia[j];
                    flag = true;
                    if (id == 0)
                    {//������ ���������
                        if (zz == 2)
                        {
                            if (dist(beg, prepatstvia[j].center) <= prepatstvia[j].r + epsilon)
                            {//���� beg ������ �����
                                ptAnsMin = t1;
                            }
                            else if (dist(beg, t1) < dist(beg, t2) && dist(beg, t1) > epsilon)
                            {//���� � t1 ����� 
                                ptAnsMin = t2;
                            }
                            else if (abs(dist(beg, prepatstvia[j].center) - prepatstvia[j].r) > epsilon)
                            {//���� � t2 �����
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
                    {//������ �����������
                        if (zz == 2)
                        {
                            if (dist(beg, prepatstvia[j].center) <= prepatstvia[j].r + epsilon)
                            {//���� beg ������ �����
                                ptAnsMin = t1;
                            }
                            else if (dist(beg, t1) < dist(beg, t2) && dist(beg, t1) > epsilon)
                            {//���� � t1 ����� 
                                ptAnsMin = t1;
                            }
                            else if (abs(dist(beg, prepatstvia[j].center) - prepatstvia[j].r) > epsilon)
                            {//���� � t2 �����
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
            //���� ����� ����������� - ����, � �� ������, �� ����������� ���� ���, �� �������� ���;
            //����� ��������� ���:
            if (!flag)
            {
                glColor4d(c.r, c.g, c.b, c.a);
                glVertex2d(beg.x, beg.y);
                glVertex2d(end.x, end.y);
            }
            else if (flag)
            {
                //���� ��������� ����� ����������� � ������������
                for (int i = j; i < numOfPrepatstviy; i++)
                {
                    if (prepatstvia[i].is_crossing(beg, peresechenie))
                    {
                        zz = prepatstvia[i].whereCrossing(beg, peresechenie, t1, t2);
                        if (id == 0)
                        {//������ ���������
                            if (zz == 2)
                            {
                                if (dist(beg, prepatstvia[i].center) <= prepatstvia[i].r + epsilon)
                                {//���� beg ������ �����
                                    ptAnsLocal = t1;
                                }
                                else if (dist(beg, t1) < dist(beg, t2) && dist(beg, t1) > epsilon)
                                {//���� � t1 ����� 
                                    ptAnsLocal = t2;
                                }
                                else if (abs(dist(beg, prepatstvia[i].center) - prepatstvia[i].r) > epsilon)
                                {//���� � t2 �����
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
                        {//������ �����������
                            if (zz == 2)
                            {
                                if (dist(beg, prepatstvia[i].center) <= prepatstvia[i].r + epsilon)
                                {//���� beg ������ �����
                                    ptAnsLocal = t1;
                                }
                                else if (dist(beg, t1) < dist(beg, t2) && dist(beg, t1) > epsilon)
                                {//���� � t1 ����� 
                                    ptAnsLocal = t1;
                                }
                                else if (abs(dist(beg, prepatstvia[i].center) - prepatstvia[i].r) > epsilon)
                                {//���� � t2 �����
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
                {//���� ��� �� ����������� ���, �� ������ ��� �����������
                    rays = new Ray[4];
                    rays[0] = Ray(ptAnsMin,
                        c,
                        govno.reflection(ptAnsMin,angle),
                        depth + 1,
                        0);//���������
                    rays[1] = Ray(ptAnsMin,
                        col(c.r,0,0,c.a),
                        govno.refraction(angle,ptAnsMin,1.45),
                        depth + 1,
                        1);//�����������R
                    rays[2] = Ray(ptAnsMin,
                        col(0,c.g,0,c.a),
                        govno.refraction(angle, ptAnsMin, 1.5),
                        depth + 1,
                        1);//�����������G
                    rays[3] = Ray(ptAnsMin,
                        col(0,0,c.b,c.a),
                        govno.refraction(angle, ptAnsMin, 1.55),
                        depth + 1,
                        1);//�����������B
                    //rays[4] = Ray(ptAnsMin,
                    //    c,
                    //    angle,
                    //    depth + 1);//�����������
                    //
                    free(rays);
                }
                else
                {//���� �� ��� ����������� ���, �� � ����������� ������ ����
                    rays = new Ray[2];
                    rays[0] = Ray(ptAnsMin,
                        c,
                        govno.reflection(ptAnsMin,angle),
                        depth + 1,
                        0);//���������
                    double k=1.5;//���� �����������
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
                        1);//�����������
                    //rays[2] = Ray(ptAnsMin,
                    //    c,
                    //    angle,
                    //    depth + 1);//�����������
                    free(rays);
                }
            }
                //glColor4d(c.r, c.g, c.b, c.a);
                //glVertex2d(beg.x, beg.y);
                //glVertex2d(end.x, end.y);
            
        }
    }
    //���������� ��������� ���, ������� ������������� �����:(��)
    //
    /*~Ray()
    {
        if (depth > 0)
            delete rays;
    }*/
};
Ray* allRays;
void herachitLuchamiEbanutemi(double xStart,double yStart,col color,int numOfRays)
{//�������� ��������
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
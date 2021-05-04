#include "ConvexHull.h"
#include <iostream>
#include "cmath"    //acos
#define PI 3.1415926535897931
void ConvexHull::FindConvexHull_BF(const vector<Point> & input, vector<Point> &output) {
    for(int i=0;i<input.size();i++){
        for(int j=0;j<input.size();j++){
            if(input[i] == input[j])
                continue;
            else    // if p != q
            {
                Line L_pq = FindLine(input[i], input[j]);   // p = input[i], q = input[j]
                bool check = true;  //檢查所有r都在L_pq右邊
                for(int k=0;k<input.size();k++){
                    if(input[k] == input[i] || input[k] == input[j])
                        continue;
                    else   // if p,q,r 是不同點
                    {
                        if(!isOnRight(input[i], input[j], input[k])){
                            check = false;
                            break;
                        }
                    }
                }
                if(check){   //所有r都在L_pq右邊
                    //push p and q into output if they are not in output
                    bool pInOutput = false, qInOutput = false;
                    for(int k=0;k<output.size();k++){
                        if(output[k] == input[i])
                            pInOutput = true;
                        if(output[k] == input[j])
                            qInOutput = true;
                    }
                    //add p, q into output
                    if(!pInOutput)
                        output.push_back(input[i]);
                    if(!qInOutput)
                        output.push_back(input[j]);
                }
            }
        }
    }
}

void ConvexHull::FindConvexHull_JM(const vector<Point> & input, vector<Point> &output) {
    Point q = FindLowestPoint(input);
    output.push_back(q);
    Point p(q.getX()-1, q.getY());  //在q左邊
    Point r(p);

    while( !(r == output[0]) ){
        double max_angle = 0;
        double max_distance_sqr = 0;    //與q的距離平方
        for(int i=0;i<input.size();i++){
            double cur_angle = ComputeAngle(p, q, input[i]);
            if( !(input[i] == q) && !(input[i] == p) && cur_angle >= max_angle )
            {
                if( cur_angle == max_angle ) // same angle => 取距離更遠的
                {
                    if( (input[i].getX()-q.getX()) * (input[i].getX()-q.getX())
                      + (input[i].getY()-q.getY()) * (input[i].getY()-q.getY()) 
                      > max_distance_sqr )
                    {
                        max_distance_sqr = (input[i].getX()-q.getX()) * (input[i].getX()-q.getX()) + (input[i].getY()-q.getY()) * (input[i].getY()-q.getY());
                        r = input[i];
                    }
                }
                else    // > max angle
                {
                    max_angle = cur_angle;
                    max_distance_sqr = (input[i].getX()-q.getX()) * (input[i].getX()-q.getX()) + (input[i].getY()-q.getY()) * (input[i].getY()-q.getY());
                    r = input[i];
                }
            }
        }
        output.push_back(r);
        p = q;
        q = r;
    }
    //output最後一個和第一個一樣
    output.pop_back();
}

Line ConvexHull::FindLine(const Point &p, const Point &q) {
    if(q.getX() == p.getX()){    //垂直
        Line retLine(1, 0, -q.getX());   // x = q.x
        return retLine;
    }
    if(q.getY() == p.getY()){    //水平
        Line retLine(0, 1, -q.getY());   // y = q.y
        return retLine;
    }

    double v_x = q.getX() - p.getX(), v_y = q.getY() - p.getY();    //方向向量
    double a = 1;   //a > 0
    double b = -a * v_x / v_y;    //b=-a*v_x/v_y
    double c = -a*p.getX() - b*p.getY();    //c=-ax-by

    Line retLine(a, b, c);
    return retLine;
}

bool ConvexHull::isOnRight(const Point &p, const Point &q, const Point &r) {    //外積判斷
    Line L_pq = FindLine(p,q);
    double v_pq[2] = { q.getX()-p.getX(), q.getY()-p.getY() };
    double v_qr[2] = { r.getX()-q.getX(), r.getY()-q.getY() };
    double v_pr[2] = { r.getX()-p.getX(), r.getY()-p.getY() };
    double d_pq_sqr = (q.getX()-p.getX())*(q.getX()-p.getX()) + (q.getY()-p.getY())*(q.getY()-p.getY());
    double d_qr_sqr = (r.getX()-q.getX())*(r.getX()-q.getX()) + (r.getY()-q.getY())*(r.getY()-q.getY());
    double d_pr_sqr = (r.getX()-p.getX())*(r.getX()-p.getX()) + (r.getY()-p.getY())*(r.getY()-p.getY());

    // if |pq x qr| < 0 , then r is on right side of pq
    if( v_pq[0]*v_qr[1] - v_pq[1]*v_qr[0] < 0 )
        return true;
    else if( v_pq[0]*v_qr[1] - v_pq[1]*v_qr[0] == 0 )   //三點共線, 比較pq, qr, pr三線段長度, 
    {
        if( d_pq_sqr >= d_qr_sqr && d_pq_sqr >= d_pr_sqr )  //pq最長
            return true;
        else
            return false;
    }
    else
        return false;
}

Point ConvexHull::FindLowestPoint(const vector<Point> &input) {
    // 取y最小的, 如果y一樣, 則取x最小的
    Point pt(input[0]);
    for(int i=1;i<input.size();i++){
        if( input[i].getY() < pt.getY() ) {
            pt = input[i];
        }else if( input[i].getY() == pt.getY() ) {
            if( input[i].getX() < pt.getX() )
                pt = input[i];
        }
    }
    return pt;
}

double ConvexHull::ComputeAngle(const Point &p, const Point &q, const Point &r) {
    // cos(pqr) = (qp dot qr) / (|qp| * |qr|)
    double v_qp[2] = { p.getX()-q.getX(), p.getY()-q.getY() };
    double v_qr[2] = { r.getX()-q.getX(), r.getY()-q.getY() };

    double cos_pqr = ( v_qp[0]*v_qr[0] + v_qp[1]*v_qr[1] )
                    / ( sqrt( (v_qp[0]*v_qp[0] + v_qp[1]*v_qp[1]) * (v_qr[0]*v_qr[0] + v_qr[1]*v_qr[1]) ) );
    
    return acos(cos_pqr)*180 / PI;  //角度
}
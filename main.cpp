#include <iostream>
#include <fstream>
#include <algorithm>
#include <sys/time.h>
#include "ConvexHull.h"

using namespace std;

void DrawPoints(const vector<Point> &input, const char *filename); // provide data to the first part of Output.txt
void DrawLines(const vector<Point> &input, const char *filename); // provide data to the second part of Output.txt
void selectPoints(vector<Point> &v1, const vector<Point> &v2);
void sort_ConvexHullpt(vector<Point> &input);
void SWAP(Point &a, Point &b);
bool cmp1(const Point &a, const Point &b);
bool cmp2(const Point &a, const Point &b);

int main()
{
    struct timeval start1, end1, start2, end2, start3, end3;
    ifstream fin("test_Input.txt");
    int n=0;
    fin >> n;
    if(n<3){    // 點數少於3
        cout << "number of points less than 3 !" << endl;
        return 0;
    }

    vector<Point> input, output1, output2;
    for(int i=0;i<n;i++){   //input from file
        double x, y;
        fin >> x >> y;
        Point pt(x,y);
        input.push_back(pt);
    }

    //***BF***
    ConvexHull ch;
    gettimeofday(&start1, NULL);    //-----start1-----
    ch.FindConvexHull_BF(input, output2);
    gettimeofday(&end1, NULL);      //-----end1-----
    output1 = input;
    //part1
    selectPoints(output1, output2);
    DrawPoints(output1, "Output_brutal.txt");
    //part2
    gettimeofday(&start2, NULL);    //-----start2-----
    sort_ConvexHullpt(output2);   //排序成逆時針
    gettimeofday(&end2, NULL);    //-----end2-----
    DrawLines(output2, "Output_brutal.txt");

    cout << "brutal force: "
         << (end1.tv_sec - start1.tv_sec)*1000 + (end1.tv_usec - start1.tv_usec)/1000
          + (end2.tv_sec - start2.tv_sec)*1000 + (end2.tv_usec - start2.tv_usec)/1000
         << " ms" << endl;

    output1.clear();
    output2.clear();

    //***JM***
    gettimeofday(&start3, NULL);    //-----start3-----
    ch.FindConvexHull_JM(input, output2);
    gettimeofday(&end3, NULL);    //-----end3-----
    output1 = input;
    selectPoints(output1, output2);
    //part1
    DrawPoints(output1, "Output_Javis.txt");
    //part2
    //output2已經排序好了
    DrawLines(output2, "Output_Javis.txt");

    cout << "Javis: "
         << (end3.tv_sec - start3.tv_sec)*1000 + (end3.tv_usec - start3.tv_usec)/1000
         << " ms" << endl;    

    return 0;
}

void DrawPoints(const vector<Point> &input, const char *filename) // provide data to the first part of Output.txt
{
    ofstream fout(filename);
    fout << input.size() << endl;
    for(int i=0;i<input.size();i++){
        fout << input[i].getX() << " " << input[i].getY() << endl;
    }
    fout << endl;
}

void DrawLines(const vector<Point> &input, const char *filename) // provide data to the second part of Output.txt
{
    ofstream fout(filename, ios_base::app);
    fout << input.size() << endl;
    for(int i=0;i<input.size();i++){
        if(i == input.size()-1)
            fout << input[i].getX() << " " << input[i].getY();
        else
            fout << input[i].getX() << " " << input[i].getY() << endl;
    }
}

void selectPoints(vector<Point> &v1, const vector<Point> &v2)
{
    for(int i=0;i<v1.size();i++){
        for(int j=0;j<v2.size();j++){
            if(v1[i] == v2[j]){
                SWAP(v1[i], v1[v1.size()-1]);
                v1.pop_back();
                i--;
            }
        }
    }
}

void sort_ConvexHullpt(vector<Point> &input)
{
    //先找出右端點(下)和左端點(上)
    //從最右端開始, 逆時針排序
    //上半部: 下一個點是 比右端點y座標大 且 比上一個點x座標小 的點
    //下半部: 下一個點是 比左端點y座標小 且 比上一個點x座標大 的點
    int right = 0, left = 0;    //左右端點的index
    for(int i=1;i<input.size();i++){
        // rightPt
        if( input[i].getX() > input[right].getX() ){
            right = i;
        }
        else if( input[i].getX() == input[right].getX() ){
            if( input[i].getY() < input[right].getY() )
                right = i;
        }
        // leftPt
        if( input[i].getX() < input[left].getX() ){
            left = i;
        }
        else if( input[i].getX() == input[left].getX() ){
            if( input[i].getY() > input[left].getY() )
                left = i;
        }
    }

    //右端點換到第一個, 左端點換到最後一個
    if(left!=0){    //如果 left!=0 那就可以直接換
        SWAP(input[0], input[right]);   
        SWAP(input[input.size()-1], input[left]);
    }else{  //left==0
        SWAP(input[0], input[right]);
        left = right;
        SWAP(input[input.size()-1], input[left]);
    }
    left = input.size()-1, right = 0;
    // 區分上下半部
    int i=1;
    for( int j=1;j<input.size()-1;j++ ){
        double v_rl[2] = { input[left].getX()-input[right].getX(), input[left].getY()-input[right].getY() };
        double v_rj[2] = { input[j].getX()-input[right].getX(), input[j].getY()-input[right].getY() };
        if( v_rl[0]*v_rj[1] - v_rl[1]*v_rj[0] < 0 )  // |v_rl x  v_rj| < 0 , then j 屬於上半部
        {
            SWAP(input[i], input[j]);
            i++;
        }
    }
    SWAP(input[i], input[input.size()-1]);    //把左端點換回中間

    // 分段排序
    sort(input.begin()+1, input.begin()+i, cmp1);
    sort(input.begin()+i+1, input.end(), cmp2);
}

void SWAP(Point &a, Point &b)
{
    Point t = a;
    a = b;
    b = t;
}

bool cmp1(const Point &a, const Point &b)
{
    if(a.getX() == b.getX())
        return a.getY() < b.getY();
    return a.getX() > b.getX();
}

bool cmp2(const Point &a, const Point &b)
{
    if(a.getX() == b.getX())
        return a.getY() > b.getY();
    return a.getX() < b.getX();
}
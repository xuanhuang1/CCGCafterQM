//
//  functions.hpp
//  
//
//  Created by Xuan Huang on 6/28/16.
//
//

#ifndef functions_h
#define functions_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include "structs.h"


using namespace std;

int readIn(vector<vertex> &v, vector<edge> &e, vector<face> &f, string filename);
void clearNeighborFaceFormat(vector<vertex> &v,vector<edge> &e,vector<face> &f);

int smoothLapAng(vector<vertex> &v,vector<face> &f);
void smooth2Star(std::vector<vertex> &v, std::vector<face> &f, double ar);
void smooth2Q(std::vector<vertex> &v, std::vector<face> &f, double ar, double max, double min);
void smooth2QStar(std::vector<vertex> &v, std::vector<face> &f, double ar, double max, double min);



void maxminAng(vector<vertex> &v, vector<face> &f, double &max, double &min);
double aspectR(vector<vertex> &v, vector<face> &f, double &med);
double sortByMinAng(face &f1, face &f2);
double sortByAR(face &f1, face &f2);

double GetCircumCenterX(double Ax, double Ay, double Bx, double By, double Cx, double Cy);
double GetCircumCenterY(double Ax, double Ay, double Bx, double By, double Cx, double Cy);

double movePX(double Ax, double Ay, double cenX, double cenY, double ang, double rScalar);
double movePY(double Ax, double Ay, double cenX, double cenY, double ang, double rScalar);
double lineDistPoint(double x1, double y1, double x2, double y2,double px, double py);
double findShortestDistInStar(vector<vertex> &v, vector<face> &f, double, double, int);
double findShortestDistInStarT(vector<vertex> &v, vector<face> &f, double, double, std::vector<int> nbrs);



struct Vertex{
    double x;
    double y;
    double z;
};

struct CREAnalysis{
    double AR;
    double skew;
    double taperX;
    double taperY;
    double jacobianDet;
    double modifiedDet;
};

CREAnalysis analyze(Vertex vertices[4]);



#endif /* functions_h */


#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.1415926
enum moveMD{ARMODE, ANGLEMODE};

/*void getAngle(vector<vertex> &v, face f, 
    double &max, double &min,
    int &vMaxMid, int &vMinMid){ // middle vertex in cc order 

    double angleTemp = 20.123456;
    double vec1x, vec1y, vec2x, vec2y;
    for (int j=0; j<f.listOfV.size(); ++j) { // for each vertex in this face, there is an angle
        int last, self, next;
        self = f.listOfV[j];
        if (j == 0) {
            last = f.listOfV[f.listOfV.size()-1];
            next = f.listOfV[j+1];
        }else if(j==f.listOfV.size()-1){
            last = f.listOfV[j-1];
            next = f.listOfV[0];
        }else{
            last = f.listOfV[j-1];
            next = f.listOfV[j+1];
        }
        vec1x = v[last].x - v[self].x; vec1y = v[last].y - v[self].y;
        vec2x = v[next].x - v[self].x; vec2y = v[next].y - v[self].y;

        angleTemp = acos((vec1x*vec2x + vec1y*vec2y)/(sqrt(pow(vec1x,2)+pow(vec1y,2))
                                                    *sqrt(pow(vec2x,2)+pow(vec2y,2)) )  );

            //for test angle
            cout << "vec2x*vec1x + vec2y*vec1y: " << vec2x*vec1x + vec2y*vec1y << endl;
             cout << "sqrt(pow(vec1x,2)+pow(vec1y,2))*sqrt(pow(vec2x,2)+pow(vec2y,2)): " << sqrt(pow(vec1x,2)+pow(vec1y,2))*sqrt(pow(vec2x,2)+pow(vec2y,2)) << endl;
            

        if (j==0) {
            max = angleTemp;
            min = angleTemp;
            vMaxMid = j;
            vMinMid = j;
        }else{
            if (angleTemp > max) {
                vMaxMid = j;
                max = angleTemp;
            }
            else if (angleTemp < min){
                min = angleTemp;
                vMinMid = j;
            }
        }
    }// end of this angle
}*/

double angleOnVertex(vertex lastV, vertex thisV, vertex nextV){
    double  vec1x = lastV.x - thisV.x; double vec1y = lastV.y - thisV.y;
    double  vec2x = nextV.x - thisV.x; double vec2y = nextV.y - thisV.y;
            //cout << "vec2x*vec1x + vec2y*vec1y: " << vec2x*vec1x + vec2y*vec1y << endl;
            // cout << "sqrt(pow(vec1x,2)+pow(vec1y,2))*sqrt(pow(vec2x,2)+pow(vec2y,2)): " << sqrt(pow(vec1x,2)+pow(vec1y,2))*sqrt(pow(vec2x,2)+pow(vec2y,2)) << endl;

    return  acos((vec1x*vec2x + vec1y*vec2y)/(sqrt(pow(vec1x,2)+pow(vec1y,2))
                                                    *sqrt(pow(vec2x,2)+pow(vec2y,2)) )  );
}

void disCheckMove(int i, vector<vertex> &v, vector<face> &f, double moveX, double moveY){

    if(!v[i].onBound){

        double xTobeMove = v[i].x + moveX;
        double yTobeMove = v[i].y + moveY;

        double ordis = findShortestDistInStar(v, f, v[i].x, v[i].y, i);
        double dis = findShortestDistInStar(v, f, xTobeMove, yTobeMove, i);
        if(dis > ordis || dis == ordis){
            v[i].x = xTobeMove;  
            v[i].y = yTobeMove;
        }//else {cout <<" "<<i <<" " <<endl;
            //cout << " dis: "<< dis<<" ordis: "<< ordis<<"\n"<<endl;
        //cout << "dang dis: "<< dis<<" ordis: "<< ordis<<"\n"<<endl;}
    }

}
void disCheckMovePlan(int i, vector<vertex> &v, vector<face> &f, double moveX, double moveY, int mode){

    if(!v[i].onBound){

        double xTobeMove = v[i].x + moveX;
        double yTobeMove = v[i].y + moveY;

        int frac = 0; 

        //double ordis = findShortestDistInStar(v, f, v[i].x, v[i].y, i);
        //double dis = findShortestDistInStar(v, f, xTobeMove, yTobeMove, i);
        //if(dis > ordis || dis == ordis){
        if(mode == ANGLEMODE){
            frac = 1;
        }else{frac = v[i].numOfNeighborFace;}

            v[i].xPlan += frac*xTobeMove;  
            v[i].yPlan += frac*yTobeMove;
            v[i].planCount += frac;
        //}//else {cout <<" "<<i <<" " <<endl;
            //cout << " dis: "<< dis<<" ordis: "<< ordis<<"\n"<<endl;
        //cout << "dang dis: "<< dis<<" ordis: "<< ordis<<"\n"<<endl;}
    }
}

void smooth2Q(std::vector<vertex> &v, std::vector<face> &f, double ar, double maxThresh, double minThresh){
    // for each face 
    //  find largest angle && divide into two triangles
    //  if angle > 120, do
    // if the another triangle largest angle > 120 do, if smallest angel < 30 do
    for (int i = 0; i < f.size(); ++i){

        for (int j = 0; j < f[i].listOfV.size(); ++j){
            //cout <<f[i].listToS()<<endl;
            int self = f[i].listOfV[j];
            int last = f[i].listOfV[(j-1+f[i].listOfV.size())%(f[i].listOfV.size())];
            int next = f[i].listOfV[(j+1+f[i].listOfV.size())%(f[i].listOfV.size())];

            double angle = angleOnVertex(v[last], v[self], v[next])*180/PI;
            double dX = (v[next].x - v[last].x);
            double dY = (v[next].y - v[last].y);
            if(angle > maxThresh){
                double rat = angle/70000;

                disCheckMove(last, v, f, dX*rat, dY*rat);
                disCheckMove(next, v, f, -dX*rat, - dY*rat);
                disCheckMove(self, v, f, dY*rat, -dX*rat);

            }else if(angle < minThresh){
                double rat = (1.0)/50;

                disCheckMove(last, v, f, -dX*rat,  -dY*rat);
                disCheckMove(next, v, f, dX*rat,  dY*rat);
                disCheckMove(self, v, f, -dY*rat,  dX*rat);

            }

            double rat = 80;
            double leftDist = pow(v[last].x-v[self].x, 2) + pow(v[last].y-v[self].y, 2);
            double rightDist = pow(v[next].x-v[self].x, 2) + pow(v[next].y-v[self].y,2);

            double selfOffX = 0, selfOffY =0;
            if(leftDist / rightDist > ar){
                selfOffX = -dX/rat;
                selfOffY = -dY/rat;
                disCheckMove(self, v, f, selfOffX, selfOffY);
            }else if(rightDist / leftDist > ar){
                selfOffX = dX/rat;
                selfOffY = dY/rat;
                disCheckMove(self, v, f, selfOffX, selfOffY);
            }
            
            /*if(leftDist / rightDist > 3){
                selfOffX = (-v[next].x+v[self].x)/rat;
                selfOffY = (-v[next].y+v[self].y)/rat;
                disCheckMove(self, v, f, selfOffX, selfOffY);
            }else if(rightDist / leftDist > 3){
                selfOffX = (-v[last].x+v[self].x)/rat;
                selfOffY = (-v[last].y+v[self].y)/rat;
                disCheckMove(self, v, f, selfOffX, selfOffY);
            }*/

        }   

    }

}


void smooth2QStar(std::vector<vertex> &v, std::vector<face> &f, double ar, double maxThresh, double minThresh){
    // for each face 
    //  find largest angle && divide into two triangles
    //  if angle > 120, do
    // if the another triangle largest angle > 120 do, if smallest angel < 30 do
    for (int i = 0; i < f.size(); ++i){

        for (int j = 0; j < f[i].listOfV.size(); ++j){
            //cout <<f[i].listToS()<<endl;
            int self = f[i].listOfV[j];
            int last = f[i].listOfV[(j-1+f[i].listOfV.size())%(f[i].listOfV.size())];
            int next = f[i].listOfV[(j+1+f[i].listOfV.size())%(f[i].listOfV.size())];

            double angle = angleOnVertex(v[last], v[self], v[next])*180/PI;
            double dX = (v[next].x - v[last].x);
            double dY = (v[next].y - v[last].y);
            if(angle > maxThresh){
                double rat = 100000/angle;

                disCheckMovePlan(last, v, f, dX/rat, dY/rat, ANGLEMODE);
                disCheckMovePlan(next, v, f, -dX/rat, - dY/rat, ANGLEMODE);
                disCheckMovePlan(self, v, f, dY/rat, -dX/rat, ANGLEMODE);

            }else if(angle < minThresh){
                double rat = 100;

                disCheckMovePlan(last, v, f, -dX/rat,  -dY/rat, ANGLEMODE);
                disCheckMovePlan(next, v, f, dX/rat,  dY/rat, ANGLEMODE);
                disCheckMovePlan(self, v, f, -dY/rat,  dX/rat, ANGLEMODE);

            }

            double rat = 100;
            double leftDist = pow(v[last].x-v[self].x, 2) + pow(v[last].y-v[self].y, 2);
            double rightDist = pow(v[next].x-v[self].x, 2) + pow(v[next].y-v[self].y,2);

            double selfOffX = 0, selfOffY =0;
            if(leftDist / rightDist > ar){
                selfOffX = -dX/rat;
                selfOffY = -dY/rat;
                disCheckMovePlan(self, v, f, selfOffX, selfOffY, ARMODE);
            }else if(rightDist / leftDist > ar){
                selfOffX = dX/rat;
                selfOffY = dY/rat;
                disCheckMovePlan(self, v, f, selfOffX, selfOffY, ARMODE);
            }

        }   

    }
    for (int i = 0; i < v.size(); ++i)
    {
        if(v[i].planCount){
            disCheckMove(i, v, f, v[i].xPlan/v[i].planCount-v[i].x, v[i].yPlan/v[i].planCount-v[i].y);

            v[i].xPlan = 0;
            v[i].yPlan = 0;
            v[i].planCount = 0;
        }
    }


}


/* backup
void smooth2Star(std::vector<vertex> &v, std::vector<face> &f, double ar){
    for (int i=0; i<v.size(); i++) {// for each vertex in mesh
        double xnew = 0;
        double ynew = 0;
        vertex nextV(0,0,0), thisV(0,0,0);

        if(v[i].onBound == 0){  
            for(int j= 0; j<v[i].numOfNeighborFace; j++){ // for each vertex in neihgbor face
                // get the next vertex and forms a triangle with v[i]
                thisV = v[v[i].neighbors[j]];
                nextV = v[v[i].neighbors[(j+1)%v[i].numOfNeighborFace]];


                double vecx,vecy, angle = 0.01;
                double lastEdge, nextEdge,thisEdge, tempAR, minAng, thisAng;
                int self, next, ver2;
                double centerX, centerY;


                //for triangle only!!!
                centerX = GetCircumCenterX(thisV.x, thisV.y,nextV.x, nextV.y,v[i].x, v[i].y);
                centerY = GetCircumCenterY(thisV.x, thisV.y,nextV.x, nextV.y,v[i].x, v[i].y);

                std::vector<int> tempInts;
                tempInts.push_back(i);
                tempInts.push_back(v[i].neighbors[j]);
                tempInts.push_back(v[i].neighbors[(j+1)%v[i].numOfNeighborFace]);

                // in that triangle
                face aFaceInStar = face(tempInts);
                for(int k=0; k<aFaceInStar.listOfV.size(); k++){ 
                    self = aFaceInStar.listOfV[k];
                    next = aFaceInStar.listOfV[(k+1)%aFaceInStar.listOfV.size()];
                    int last = aFaceInStar.listOfV[(aFaceInStar.listOfV.size()+k-1)%aFaceInStar.listOfV.size()];
                    //cout << self <<" "<< next <<" "<< endl;

                    vecx = v[next].x - v[self].x;   
                    vecy = v[next].y - v[self].y; 
                    double vec1x = v[last].x - v[self].x;
                    double vec1y = v[last].y - v[self].y;

                    thisEdge = sqrt(pow(vecx,2)+ pow(vecy,2));
                    thisAng = acos((vec1x*vecx + vec1y*vecy)
                             /(sqrt(pow(vec1x,2)+pow(vec1y,2))
                               *sqrt(pow(vecx,2)+pow(vecy,2)) )  );

                    // find max and min edge
                    if(k==0){
                        minAng = thisAng;
                        lastEdge = sqrt(pow(vecx,2)+ pow(vecy,2));
                        nextEdge = sqrt(pow(vec1x,2)+ pow(vec1y,2));

                        if(lastEdge < nextEdge){
                            angle = -0.01;
                            ver2 = last;
                        }else{
                            angle = 0.01;
                            ver2 = next;
                        }

                    }else{
                        if(thisAng < minAng)
                            minAng = thisAng;
                    }
                }

                double xTobeMove = v[i].x, yTobeMove = v[i].y;

                // 2 for fine 1.5 for good mesh

                if ( max(lastEdge, nextEdge)/min(lastEdge, nextEdge) < ar )
                {
                    //cout <<"ratoi" <<endl;
                    //cout << max(lastEdge, nextEdge)/min(lastEdge, nextEdge) <<endl;
                }else{


                //cout << max << " "<< min <<endl;

                //if(minAng < 30*PI/180){

                    //cout << "min " <<minAng*PI/180 <<endl;
                    //cout << minAng << " "<<endl;
                    double count = 3;

                    while(count > 0){
                        xTobeMove = movePX(xTobeMove, yTobeMove, centerX, centerY, angle);
                        yTobeMove = movePY(xTobeMove, yTobeMove, centerX, centerY, angle);
                        count --;
                        //cout << "thisEdge+0.01*max: " <<thisEdge+0.01*max << " currentDist: " << currentDist<<endl;
                    }
                    
                }


                //}
                double ordis = findShortestDistInStar(v, f, v[i].x, v[i].y, v[i].neighbors);
                double dis = findShortestDistInStar(v, f, xTobeMove, yTobeMove, v[i].neighbors);
                if(dis < ordis){
                    xTobeMove = v[i].x;
                    yTobeMove = v[i].y;
                }


                xnew += xTobeMove;
                ynew += yTobeMove;


            }

            v[i].x = xnew/v[i].numOfNeighborFace;
            v[i].y = ynew/v[i].numOfNeighborFace;

        }
    }


}*/

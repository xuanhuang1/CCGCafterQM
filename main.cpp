//
//  main.cpp
//
//
//  Created by Xuan Huang on 6/23/16.
//
//

#include "functions.h"

#include <algorithm>
#include <string.h>
#include <math.h>
#include <time.h>

#define PI 3.1415926
#define ARLARGE 8
#define ARPRT 5
#define ANGPRT 9

using namespace std;
double min_angel_glb;


void printStats(vector<vertex> &v, vector<face> &f){
    double arStats[ARPRT];
    double angMaxStats[ANGPRT];
    double angMinStats[ANGPRT];
    double averAR = 0, averMin =0, averMax = 0;

    double angleInter = 90.0/ANGPRT;

    for (int i = 0; i < ARPRT; ++i)
        arStats[i] = 0;

    for (int i = 0; i < ANGPRT; ++i){
        angMaxStats[i] = 0;
        angMinStats[i] = 0;
    }

    for (int i = 0; i < f.size(); ++i){
        double tempAR = f[i].aspectR;
        double tempMin = f[i].minAng*180/PI;
        double tempMax = f[i].maxAng*180/PI;
        averAR += tempAR/f.size();
        averMin += tempMin/f.size();
        averMax += tempMax/f.size();

        //cout << "faces: " << tempAR <<" "<<tempMin <<" "<<tempMax <<endl;

        if(tempAR>ARLARGE){
            arStats[4] ++;
        }else if(tempAR>ARLARGE*0.75){
            arStats[3]++;
        }else if(tempAR>ARLARGE*0.5){
            arStats[2]++;
        }else if(tempAR>ARLARGE*0.25){
            arStats[1]++;
        }else arStats[0]++;

        for (int j = 0; j < ANGPRT; ++j){
            if((tempMin > j*angleInter) && (tempMin < (j+1)*angleInter+0.01))
                angMinStats[j]++;
            if((tempMax > j*angleInter+90) && (tempMax < (j+1)*angleInter+90+0.01))
                angMaxStats[j]++;
        }
        
    }

    cout <<endl<< "aver: AR " << averAR <<" min "<<averMin <<" max "<<averMax<<endl<<endl;

    for (int i = 1; i < (ARPRT+1); ++i){
        cout << "ar above " << (i-1)*ARLARGE*0.25 << ": " << arStats[i-1] 
        <<"   \t" <<int(arStats[i-1]*100/f.size())<<"%\t";
        for (int j = 0; j < 20; ++j)
            if(arStats[i-1]*100/f.size()> j*5) cout <<"*";
        cout<<endl;
    }
    cout<<endl;
    for (int i = 1; i < (ANGPRT+1); ++i){
        cout << "minang " << (i-1)*angleInter <<"-" <<(i)*angleInter << ": " << angMinStats[i-1] 
        <<"  \t" <<int(angMinStats[i-1]*100/f.size())<<"%\t";
        for (int j = 0; j < 20; ++j)
            if(angMinStats[i-1]*100/f.size()> j*5) cout <<"*";
        cout<<endl;
    }
    cout<<endl;
    for (int i = 1; i < (ANGPRT+1); ++i){
        cout << "maxang " << (i-1)*angleInter+90 <<"-" <<(i)*angleInter+90 << ": " << angMaxStats[i-1] 
        <<"  \t" <<int(angMaxStats[i-1]*100/f.size())<<"%\t";
        for (int j = 0; j < 20; ++j)
            if(angMaxStats[i-1]*100/f.size()> j*5) cout <<"*";
        cout<<endl;
    }
}



int main(int argc, char* argv[]){
    if(argc != 5){
        cout << "Usage: ./test [flag: -Lap -s -q -qStar] inputOff outputOff %%itr" <<endl;
        return 1; 
    }

    string runFlag = argv[1];
    string infi = argv[2];
    string outfi = argv[3];
    int itr = stoi(argv[4]);

    ifstream inputFile(infi);
    
    if(!inputFile){
        cout << "Cannot open file" << endl;
        return 1;
    }

    vector<vertex> v;
    vector<edge> e;
    vector<face> f;
    
    readIn(v, e, f, infi);
    if((!runFlag.compare("-qStar")) || (!runFlag.compare("-Lap")))
        clearNeighborFaceFormat(v,e,f);

    double maxAngle, minAngle, med, aspectratio;
    maxminAng(v, f, maxAngle, minAngle);
    aspectratio = aspectR(v, f, med);
    sort(f.begin(), f.end(), sortByMinAng);    // !!!!! change here for sorting type!!!!


    cout << "\noriginal :" <<endl;
    cout << "max: " << maxAngle*180/PI << " min: " << minAngle*180/PI<<endl;
    cout << "aspectR: " << aspectratio<<endl;
    min_angel_glb = minAngle;

    printStats(v,f);
    
    int a =1;
    float thresh = 3;
    clock_t t = clock();

    vector<vertex> v2;
    vector<face> f2; 
    while(a >0 ){
        if(!runFlag.compare("-Lap")){
            for (int i = 0; i < itr; ++i)
 
            smoothLapAng(v, f);
        }else if(!runFlag.compare("-s")){
            for (int i = 0; i < 8; ++i){
                double tempMin, tempMax, tempAR, tempMed;

                v2 = v;
                f2 = f;
                if(aspectratio < 4){
                    thresh = 1.2;
                }else if(aspectratio < 8 ){
                    thresh = aspectratio /4;
                }

                maxminAng(v, f, tempMax, tempMin);
                tempAR = aspectR(v, f, tempMed);

                smooth2Star(v, f, thresh);
                 if((tempMin < minAngle)||(tempMax>maxAngle)||(tempAR>aspectratio)){
                    v = v2;
                    f = f2;
                    a = 0;
                    printf("iteration: %d\n", i);
                    break;
                }
            }
        }else if(!runFlag.compare("-q")){
            printf("\n------The Smoothing-----\n\n" );
            for (int i = 0; i < itr; ++i){
                double tempMin, tempMax, tempAR, tempMed;

                v2 = v;
                f2 = f;
                if(i < 33)
                    smooth2Q(v, f, 20, 160, 20);
                else if(i < 66)
                    smooth2Q(v, f, 8, 150, 30);
                else
                    smooth2Q(v, f, 5, 130, 40);

                maxminAng(v, f, tempMax, tempMin);
                tempAR = aspectR(v, f, tempMed);

                printf("%d: max:%f min:%f ar:%f\n", i, tempMax*180/PI, tempMin*180/PI, tempAR);

                /*if((tempMin < minAngle)||(tempMax>maxAngle)||(tempAR>aspectratio)){
                    v = v2;
                    f = f2;
                    a = 0;
                    printf("iteration: %d\n", i);
                    break;
                }*/
            }
            printf("\n------Ends-----\n" );
        }else if(!runFlag.compare("-qStar")){
                printf("\n------The Smoothing-----\n\n" );
            for (int i = 0; i < itr; ++i){
                v2 = v;
                f2 = f;
                double tempMin, tempMax, tempAR, tempMed;
                if(i < itr/3)
                    smooth2QStar(v, f, 20, 150, 20);
                if(i < 2*itr/3)
                    smooth2QStar(v, f, 8, 130, 40);
                else
                    smooth2QStar(v, f, thresh, 110, 50);
                

                maxminAng(v, f, tempMax, tempMin);
                tempAR = aspectR(v, f, tempMed);

                if((tempMin < minAngle)||(tempMax>maxAngle)||(tempAR>aspectratio)){
                    v = v2;
                    f = f2;
                    a = 0;
                    printf("iteration: %d\n", i);
                    break;
                }
            }
            printf("\n------Ends-----\n" );

        }else{
            cout << "Wrong flag!" <<endl;
            return 1;
        }
        a--;
    }
    t = clock()-t;


    cout << "\nafter smooth : "<<endl;

    aspectratio = aspectR(v, f, med);
    maxminAng(v, f, maxAngle, minAngle);
    cout << "time: "<<((float)t)/CLOCKS_PER_SEC <<" sec"<<endl;
    cout << "max: " << maxAngle*180/PI << " min: " << minAngle*180/PI<<endl;
    cout << "aspectR: " << aspectratio<<endl;
    /*
    for (int i=0; i<v.size(); i++) {
        cout << v[i].x << " "<< v[i].y << " "<< v[i].z << " "<< endl;
    }
    for (int i=0; i<e.size(); i++) {
        cout << e[i].node1 << " "<< e[i].node2<< endl;
    }
    cout << f.size() <<endl;*/

    printStats(v,f);

    ofstream outputfile;
    outputfile.open(outfi, ios::out | ios::trunc);
    
    outputfile << "OFF" << endl;
    outputfile << v.size()<<" "<< f.size()<<" "<<e.size()<<" "<< endl;
    
    for (int i =0; i<v.size(); i++){
        outputfile << v[i].x << " " << v[i].y << " " << v[i].z;
        outputfile <<endl;
    }
    
    for (int i =0; i<f.size(); i++){
        outputfile << f[i].listOfV.size() << " "<< f[i].listToS();
        outputfile << " "<< 1 - 2*f[i].minAng/PI <<" 0 0";
        //printf("%f\n", 1 - f[i].minAng*180/(90.0*PI));
        outputfile <<"\n";
    }
    outputfile.close();


    //double s = lineDistPoint(1, 1, 2, 2, 3, 4);
    //cout << s <<endl;
    
    return 0;
    
}
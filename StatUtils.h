/* 
 * File:   Statistic.h
 * Author: titova_ekaterina
 *
 * Created on 14 ноября 2019 г., 14:22
 */


using namespace std;

#ifndef STATISTIC_H
#define	STATISTIC_H

#include "Configure.h"
#include <fstream>
#include <iostream>

class StatUtils {
public:

    static void getStat(const vector<float> &v, double &mean, double &sig) {

        float summOfElements = 0;
        float summOfSquareElements = 0;
        for (int i = 0; i < v.size(); ++i) {
            summOfElements += v[i];
            summOfSquareElements += v[i] * v[i];
        }
        mean = summOfElements / v.size();
        sig = sqrt(summOfSquareElements / v.size() - mean * mean);
        
        // E = sum(v[i)/v.size
        // sig = sum(v[i)*v[i))/v.size - E*E

    }

    static void writeInFile(std::vector<float> metrics, double RMSE_sig, double RMSE_E) {

        std::ofstream myfile;
        myfile.open("/home/titova_ekaterina/ML_Prod/NetflixPrize_Titova/RMSE_Res.csv");
        myfile << ",1,2,3,4,5,E,SD,\n";
        myfile << "RMSE," << (metrics[0]) << "," << (metrics[1]) << "," << (metrics[2]) << "," << (metrics[3]) << "," << (metrics[4]) << "," << RMSE_E << "," << RMSE_sig << ",\n";

        myfile.close();
    }

};

#endif	/* STATISTIC_H */


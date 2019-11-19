/* 
 * File:   FactorizationMachine.h
 * Author: titova_ekaterina
 * 
 * Created on 17 октября 2019 г., 10:13
 */


#ifndef FACTORIZATIONMACHINE_H
#define	FACTORIZATIONMACHINE_H

#include "Configure.h"

using namespace Eigen;
using namespace std;

class FactorizationMachine {
private:
    MyMatrixDense V, W;
    MyMatrixDense T;
    float wb, lr;
    int epoch, k;
    long int bs;
    long int maxU, maxI;

public:
    FactorizationMachine();
    FactorizationMachine(double lr, int epoch, long int bs, int k, long int maxU, long int maxI);
    void fit(MyMatrix &Xt, const MyVector &Yt);
    MyVector predict(MyMatrix &X);
    ~FactorizationMachine();
};

#endif	/* FACTORIZATIONMACHINE_H */


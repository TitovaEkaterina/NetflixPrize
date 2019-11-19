/* 
 * File:   MatrixUtils.h
 * Author: titova_ekaterina
 *
 * Created on 17 ноября 2019 г., 11:43
 */

#ifndef MATRIXUTILS_H
#define	MATRIXUTILS_H

#include "Configure.h"

class MatrixUtils {
public:

    static MyMatrix block(MyMatrix X, int from, int to) {
        return X.block(from, 0, to, X.cols());
    }

    static MyMatrix block(MyMatrix X, int from1, int to1, int from2, int to2) {

        MyMatrix X_train2 = X.block(from1, 0, to1, X.cols());
        MyMatrix X_train3 = X.block(from2, 0, to2, X.cols());

        MyMatrix M(X_train2.rows() + X_train3.rows(), X_train3.cols());
        M.reserve(X_train2.nonZeros() + X_train3.nonZeros());
        for (Eigen::Index r = 0; r < X_train2.rows(); ++r) {
            M.startVec(r);
            for (MyMatrix::InnerIterator itL(X_train2, r); itL; ++itL)
                M.insertBack(r, itL.col()) = itL.value();
        }
        for (Eigen::Index r = 0; r < X_train3.rows(); ++r) {
            M.startVec(r + X_train2.rows());
            for (MyMatrix::InnerIterator itC(X_train3, r); itC; ++itC) {
                M.insertBack(r + X_train2.rows(), itC.col()) = itC.value();
            }
        }
        M.finalize();

        return M;
    }

    static MyVector block(MyVector Y, int from, int to) {

        int index = 0;
        MyVector Ynew((to - from),1);
        for (int j = from; j < to; j++) {
            Ynew(index) = Y(j);
            ++index;
        }

        return Ynew;

    }

    static MyVector block(MyVector Y, int from1, int to1, int from2, int to2) {

        int index = 0;
        MyVector Ynew((to1 - from1) + (to2 - from2),1);
        for (int j = from1; j < to1; j++) {
            Ynew(index) = Y(j);
            ++index;
        }

        for (int j = from2; j < to2; j++) {
            Ynew(index) = Y(j);
            ++index;
        }

        return Ynew;

    }

    static MyMatrix block(MyMatrix X, int from, int to, vector<int> id) {

        MyMatrix newX((to - from), X.cols());
        newX.reserve((to - from) * 2);
        for (Eigen::Index r = from; r < to; ++r) {
            for (MyMatrix::InnerIterator itL(X, id[r]); itL; ++itL) {
                newX.insert(r - from, itL.col()) = itL.value();
            }
        }
        return newX;
    }

    static MyVector block(MyVector Y, int from, int to, vector<int> id) {
        MyVector newY((to - from),1);
        for (int k = from; k < to; ++k) {
            newY(k - from) = Y(id[k]);
        }
        return newY;
    }

};


#endif	/* MATRIXUTILS_H */


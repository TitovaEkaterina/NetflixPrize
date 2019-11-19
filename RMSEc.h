/* 
 * File:   RMSE.h
 * Author: titova_ekaterina
 *
 * Created on 24 октября 2019 г., 11:28
 */
#ifndef RMSE_METRIC_H
#define	RMSE_METRIC_H

#include "Configure.h"

class RMSE{
public:

    static double calculateMetric(const MyVector &Y_pred, const MyVector &Y_test) {
        return sqrt((Y_test - Y_pred).dot(Y_test - Y_pred) / Y_pred.rows());
        
        // RMSE = sum((y_i - y'_i)^2) / y.size 
        // or
        // RMSE = < y_i - y'_i, y_i - y'_i > / y.size 
    }
};

#endif	/* RMSE_METRIC_H */


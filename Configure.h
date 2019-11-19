/* 
 * File:   Configure.h
 * Author: titova_ekaterina
 *
 * Created on 16 ноября 2019 г., 17:59
 */

#ifndef CONFIGURE_H
#define	CONFIGURE_H

#include <eigen3/Eigen/Sparse>
#include <eigen3/Eigen/Core>

typedef Eigen::SparseMatrix<float, Eigen::RowMajor> MyMatrix;
typedef Eigen::MatrixXf MyMatrixDense;
typedef Eigen::VectorXf MyVector;
typedef Eigen::Triplet<float> TRipl;



#endif	/* CONFIGURE_H */


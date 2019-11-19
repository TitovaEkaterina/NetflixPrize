/* 
 * File:   FactorizationMachine.cpp
 * Author: titova_ekaterina
 * 
 * Created on 17 октября 2019 г., 10:13
 */

#include "FactorizationMachine.h"
#include "RMSE.h"
#include "StatUtils.h"
#include "Configure.h"
#include "MatrixUtils.h"
#include <random>
#include <numeric>

using namespace std;

using namespace Eigen;

FactorizationMachine::FactorizationMachine(double lr, int epoch, long int bs, int k, long int maxU, long int maxI) {

    this->lr = lr;
    this->epoch = epoch;
    this->bs = bs;
    this->k = k;
    this->maxU = maxU;
    this->maxI = maxI;

}

FactorizationMachine::~FactorizationMachine() {
}

void FactorizationMachine::fit(MyMatrix &X, const MyVector &Y) {

    srand(static_cast<unsigned> (time(0)));
    // Инициализируем вектор весов W(X.cols, 1)
    this->W.setZero(X.cols(), 1);
    // Инициализируем матрицу факторов V(X.cols, k)
    this->V.setZero(X.cols(), k);
    // Инициализируем случайное смещение 
    this->wb = static_cast<float> (rand()) / static_cast<float> (RAND_MAX);

    // Переменная, хранящая количество пройденных эпох
    int e = 0;

    // Инициализируем и заполняем вектор с индексами от 0 до X.rows
    std::vector<int> id(X.rows());
    std::iota(id.begin(), id.end(), 0);

    // Цикл по эпохам
    while (e < epoch) {
        // засекаем время на одну эпоху
        clock_t startEp = clock();

        std::random_device rd;
        std::mt19937 g(rd());
        // Перемешиваем индесы, чтобы не перемешивать матрицу X
        std::shuffle(id.begin(), id.end(), g);

        // Создаем переменные для хранения бачей по X и по Y
        MyMatrix X_bach;
        MyVector Y_bach;

        // Цикл по X идем бачами
        for (int i = 0; i < id.size(); i += this->bs) {

            // Заполняем переменные бачей
            if (i + bs < id.size()) { // случай, если можно взять блок полностью размером bs
                X_bach = MatrixUtils::block(X, i, i + bs, id);
                Y_bach = MatrixUtils::block(Y, i, i + bs, id);
            } else { // случай, если осталось элементов меньше bs. Берем просто оставшиеся
                X_bach = MatrixUtils::block(X, i, id.size(), id);
                Y_bach = MatrixUtils::block(Y, i, id.size(), id);
            }

            // Предсказываем значения по бачу
            MyVector predictTargetByBach = predict(X_bach);

            // Считаем производную от LOSS ( MSE ) и умножаем ее на lr ( learning rate )
            MyVector derivative = (Y_bach - predictTargetByBach) * lr;

            // Обновляем веса W = W + (X*lr*loss')/Y.size
            W = (W + (((X_bach.transpose() * derivative) / Y_bach.size())));


            VectorXf ones;
            ones.setOnes(Y_bach.size());
            // Обновляем вес wb = wb + (1*lr*loss')/Y.size
            wb = (wb + ((derivative.transpose() * ones) / Y_bach.size())(0, 0));


            MatrixXf M;
            M.setZero(V.rows(), V.cols());
            for (Index r = 0; r < X_bach.rows(); ++r) {
                for (Eigen::SparseMatrix<float, Eigen::RowMajor>::InnerIterator itL(X_bach, r); itL; ++itL)
                    for (int rr = 0; rr < V.cols(); ++rr) {
                        M(itL.col(), rr) += V.coeff(itL.col(), rr) * itL.value() * itL.value();
                    }
            }
            // Обновляем матрицу факторов V = V + ((X*T - M)*lr*loss')/Y.size
            V = (V + (((X_bach.transpose() * T - M)) / Y_bach.size()));
        }
        // Останавливаем подсчет времени
        clock_t endEp = clock();

        double s = (double) (endEp - startEp) / CLOCKS_PER_SEC;
        cout << " time = " << s << endl;
        ++e;
    }
}

MyVector FactorizationMachine::predict(MyMatrix &X) {
    T = (X * V);
    MyMatrixDense t_Pow = T.array().pow(2);
    MyMatrixDense powV = V.array().pow(2);
    MyMatrixDense resultMatrix = (t_Pow - X * powV);
    powV.resize(0, 0);

    MyMatrixDense resultVector = resultMatrix.col(0) + resultMatrix.col(1);
    for (int p = 2; p < k; ++p) {
        resultVector = resultVector + resultMatrix.col(p);
    }
    resultMatrix.resize(0, 0);

    resultVector = resultVector * 0.5;
    MyVector W_0vector;
    W_0vector.setConstant(X.rows(), wb);

    return W_0vector + X * W + resultVector;
}





/* 
 * File:   main.cpp
 * Author: titova_ekaterina
 *
 * Created on 15 октября 2019 г., 11:24
 */


#include "FactorizationMachine.h"
#include "Configure.h"
#include "ReadDataUtils.h"
#include "RMSE.h"
#include "StatUtils.h"
#include "MatrixUtils.h"


using namespace std;

int main(int argc, char** argv) {

    MyMatrix X; //Разреженная матрица признаков
    MyVector Y; // Вектор рейтингов

    MyMatrix TrainX; // Разреженная матрица для train
    MyMatrix TestX; // Разреженная матрица для test
    MyVector TrainY; // Вектор рейтингов train
    MyVector TestY; // Вектор рейтингов test


    int trainNum = 5; // Количество cross-validation
    int ep = 15; //Количество эпох
    int bs = 1000000; // bach size
    int factorCount = 3; //count of factors
    float lr = 0.6; //learning rate
    long int maxUsers = 2649420, maxItem = 17770;

    // Читаем данные из файлов в матрицы X и вектор Y
    ReadDataUtils::readData("/home/titova_ekaterina/ML_Prod/NetflixPrize_Titova/set/dataReiting/training_set", X, Y, maxUsers, maxItem);

    // Вектор для сохранения результатов метрик
    std::vector<float> results_Metric;
    
    // Старт цикла по cross-validation
    for (int i = 0; i < trainNum; i++) {
        
        // Заполняем тестовые и тренировочные матрицы
        if (i != 0) { // слйчай, если тренировочная матрица составная из двух блоков ( тестовый блок в середине )
            TrainX = MatrixUtils::block(X, 0, (X.rows() / trainNum) * i, (X.rows() / trainNum) * (i + 1), X.rows() - (X.rows() / trainNum) * (i + 1));
        } else {// Случай, если тренировочная матрица цельная, т.е. может быть изьята одним блоком
            TrainX = MatrixUtils::block(X, (X.rows() / trainNum), X.rows() - (X.rows() / trainNum));
        }
        TestX = MatrixUtils::block(X, (X.rows() / trainNum) * i, (X.rows() / trainNum));
        TrainY = MatrixUtils::block(Y, 0, (X.rows() / trainNum) * i, (X.rows() / trainNum) * (i + 1), Y.rows());
        TestY = MatrixUtils::block(Y, (X.rows() / trainNum) * i, (X.rows() / trainNum) * (i + 1));

        // Создаем модель с исходными параметрами
        FM model(lr, ep, bs, factorCount, maxUsers, maxItem);

        // Обучаем модель на тестовых данных
        model.fit(TrainX, TrainY);
        
        // Предсказываем рейтинги на тестовых данных
        MyVector Y_pred_test = model.predict(TestX);

        // Считаем метрику RMSE и записываем ее результат в вектор
        float result_RMSE_test = RMSE::calculateMetric(Y_pred_test, TestY);
        results_Metric.push_back(result_RMSE_test);

    }// конец цикла по cross-validation

    // переменная для хранения среднего метрики по фолдам
    double RMSE_M = 0;
    // переменная для хранения среднеквадратичного отклонения метрики по фолдам
    double RMSE_sig = 0;

    // Считаем среднее и сигму по метрикам по фолдам
    StatUtils::getStat(results_Metric, RMSE_M, RMSE_sig);
    // Записываем результаты метрик в фал
    StatUtils::writeInFile(results_Metric, RMSE_sig, RMSE_M);

    return 0;
}








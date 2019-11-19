/* 
 * File:   ReadDataUtils.h
 * Author: titova_ekaterina
 *
 * Created on 13 ноября 2019 г., 11:03
 */



#ifndef READDATAUTILS_H
#define	READDATAUTILS_H

#include "Configure.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class ReadDataUtils {
public:

    static vector<string>& split(const string &s, char delim, vector<string> &elems) {
        stringstream ss(s);
        string item;
        while (getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }

    static vector<string> split(const string &s, char delim) {
        vector<string> elems;
        split(s, delim, elems);
        return elems;
    }

    //Получение имени файла по номеру в формате "/mv_{i}.txt", где {i} это 7-значный номер с записью числа в правой стороне
    static string getFileName(unsigned int item) {
        string start = "/mv_";
        unsigned int n = 1000000;
        while (item / n == 0) {
            start.append("0");
            n /= 10;
        }
        while (n != 0) {
            start.append(to_string(item / n));

            item %= n;
            n /= 10;
        }
        start.append(".txt");
        return start;
    }

    // чтение файлов с данными в матрицу X и вектор рейтингов Y
    static void readData(char* path, MyMatrix &X, MyVector &Y, int maxU, int maxI) {

        // Переменная для хранения номера считанного рейтинга
        unsigned int numOfRaiting = 0;

        // Вектор для хранения Y (рейтингов)
        std::vector<float> y_vec;
        
        // Вектор для хранения триплетов в формате (номер строки, номер столбца, значение)
        std::vector<TRipl> trils;

        // Цикл по файлам от 0 до maxI (17770)
        for (unsigned int i = 1; i <= maxI; ++i) {

            std::string buf(path);
            string fileName = getFileName(i);
            buf.append(fileName);
            ifstream dataFile(buf);

            if (dataFile.is_open()) {
                string line;
                getline(dataFile, line);
                // Считываем id фильма
                std::vector<string> filmsIds = split(line, ':');
                unsigned int filmsId = stod(filmsIds[0]);
                // Цикл по рейтингам для фильма с id = i
                while (getline(dataFile, line)) {
                  
                    // Считываем строку в формате "id пользователя, рейтинг, дата" и сплитим по ','
                    vector<string> tokens = split(line, ',');

                    if (tokens.size() == 3) {
                        // Считываем id пользователя
                        unsigned int user = stod(tokens[0]);
                        // Считываем рейтинг
                        int raiting = stod(tokens[1]);
                        // Записываем триплеты с 1 на соответствующие места ( 2 единицы на строку )
                        trils.push_back(TRipl(numOfRaiting, user, 1));
                        trils.push_back(TRipl(numOfRaiting, filmsId + maxU, 1));
                        // Записываем рейтинг в вектор рейтингов Y
                        y_vec.push_back(raiting);
                        // инкремент количества прочитанных рейтингов
                        ++numOfRaiting;

                    }
                }
                dataFile.close();
            }
        }
        // Создаем вектор рейтингов и разреженную матрицу признаков
        Y = MyVector::Map(y_vec.data(), y_vec.size());
        X.resize(0, 0);
        X.resize(index, maxI + maxU);
        X.setFromTriplets(trils.begin(), trils.end());
        trils.clear();
    }
};


#endif	/* READDATAUTILS_H */


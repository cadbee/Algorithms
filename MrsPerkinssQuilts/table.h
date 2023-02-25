#ifndef PIAA_TABLE_H
#define PIAA_TABLE_H
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <fstream>
#define OUTPUT_ON 1 // 1 или 0, в зависимости от необходимого вывода

class Table {
private:
    unsigned int N; //размер таблицы(столешницы)
    std::vector<std::vector<unsigned int>>  table; //таблица N*N
    std::vector<std::vector<unsigned int>>  all_solutions; //список всех решений
    std::vector<std::pair<unsigned int, unsigned int>> solution_coordinates; //список координат квадратов в текущем решении
    unsigned int cur_area; //заполненная квадратами площадь
    unsigned int solution_count; //количество найденных решений
    unsigned int max_index; //индекс, до которого стоит просматривать места для вставки
    std::ofstream &fout; //поток вывода промежуточных результатов
public:
    bool isFull() const;
    explicit Table(unsigned int n, std::ofstream& out);
    void printTable();
    bool check(unsigned int c, unsigned int r, unsigned int n);
    bool paste(unsigned int n);
    void setStart(unsigned int n);
    void printSolutions(unsigned int c);
    bool emptyPlaceExist(unsigned int n);
    void addToSolutions(std::vector<unsigned int> vec, unsigned int coeff);
    void deleteLast();
    ~Table() = default;
};

#endif //PIAA_TABLE_H

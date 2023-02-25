#ifndef PIAA_TABLE_H
#define PIAA_TABLE_H
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <fstream>
#define OUTPUT_ON 1 // 1 ��� 0, � ����������� �� ������������ ������

class Table {
private:
    unsigned int N; //������ �������(����������)
    std::vector<std::vector<unsigned int>>  table; //������� N*N
    std::vector<std::vector<unsigned int>>  all_solutions; //������ ���� �������
    std::vector<std::pair<unsigned int, unsigned int>> solution_coordinates; //������ ��������� ��������� � ������� �������
    unsigned int cur_area; //����������� ���������� �������
    unsigned int solution_count; //���������� ��������� �������
    unsigned int max_index; //������, �� �������� ����� ������������� ����� ��� �������
    std::ofstream &fout; //����� ������ ������������� �����������
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

#include <iostream>
#include "table.h"
#include <vector>
#include <ctime>
#include <cmath>

#define TIMER_ON 0 // 1 ��� 0, � ����������� �� ������������� �������

unsigned int findMinDivisor(unsigned int N) {
    //���������� ������������ ��������
    for (unsigned int i = 2; i < N + 1; i++) {
        if (N % i == 0)
            return i;
    }
    return 0;
}

void print_stack(std::vector<unsigned int> stack, std::ofstream& out) {
    //����� ����������� ����� � out
    out << "����: ";
    for (auto& x:stack)
        out<<x<<' ';
    out<<std::endl;
}

bool is_prime(unsigned int N) {
    //�������� ����� �� ��������
    for (unsigned int i = 2; i <= sqrt(N); i++) {
        if (N % i == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    //��������� ���� ��� ������ ������������� ������� � ��������
    std::ofstream fout;
    fout.open("../logs/output.txt", std::ios::in | std::ios::out | std::ios::trunc);
    if (!fout) {
        std::cerr << "Error: Unable to open logs files" << std::endl;
        return 1;
    }
    //������� ������� ����� � �������� �������
    std::vector<unsigned int> stack;
    std::vector<unsigned int> cur_sol;
    unsigned int N;
    std::cin >> N;
    //������� ����������� �������� � ����������� ������ ��� ���������� ������� ��������
    unsigned int smaller_n = findMinDivisor(N);
    unsigned int coeff = N / smaller_n;
    Table table = Table(smaller_n, fout);
    N = smaller_n;
    unsigned int max_square; //������������ ������� �������� ��� �������
    unsigned int max_length = 2 * N + 1; //������������ ���-�� ��������� � �������
    //��� ������� ����� ������ ��������� ��������� �������
    if (is_prime(N) && N > 5) {
        table.setStart(N);
        cur_sol.push_back(int(N / 2) + 1);
        cur_sol.push_back(int(N / 2));
        cur_sol.push_back(int(N / 2));
        cur_sol.push_back(1);
        for (unsigned int i = 1; i < int(N / 3) + 1; i++)
            stack.push_back(i);
        max_square = int(N/3)+1;
    }
    else {
        max_square = N-1;
        for (unsigned int i = 1; i < max_square + 1; i++)
            stack.push_back(i);

    }
    unsigned int last_append; //��������� ����������� �������
#if TIMER_ON == 1
    unsigned int start_time =  clock();
#endif
    while (!stack.empty()) {
#if OUTPUT_ON == 1
        print_stack(stack, fout);
        table.printTable();
#endif
        //���������, ����� �� �������� ������� � ������������ �������
        if ((cur_sol.size() < max_length-1) && table.paste(stack.back())) {
#if OUTPUT_ON == 1
            fout << "��������� ������� �� �������� " << stack.back() << " � �������\n";
#endif
            cur_sol.push_back(stack.back());
            last_append = stack.back();
            stack.pop_back();
            //���� ������� ��������� ���������, ��������� ������� � ������ �������
            if (table.isFull()) {
                    table.addToSolutions(cur_sol, coeff);
                    max_length = cur_sol.size();
                    stack.push_back(1);
#if OUTPUT_ON == 1
                    fout << "������������ ��������� � �������:\n";
                    table.printTable();
                    fout<<std::endl;
#endif
                    continue;
            }
            //���� ����� ������� �� 1 ������ �������������, ���������� �������
            else if(cur_sol.size() == max_length-1) {
#if OUTPUT_ON == 1
                fout << "������� �� ����������, ������������ � �����������\n";
#endif
                if (last_append == 1) {
                    if (cur_sol.back() == 1) {
                        while (cur_sol.back() == 1) {
                            cur_sol.pop_back();
                            table.deleteLast();
                        }
                        cur_sol.pop_back();
                        table.deleteLast();
                        continue;
                    } else {
                        cur_sol.pop_back();
                        table.deleteLast();
                    }
                }
                cur_sol.pop_back();
                table.deleteLast();
            }
            //���� ����� ������� �� 2 ������ �������������, ��������� � ���� ������ ����������� ��������� ��� ������� �������
            else if(cur_sol.size() == max_length - 2) {
                stack.push_back(1);
                for (unsigned int i = 2; i < max_square; i++) {
                    if ((table.emptyPlaceExist(i))) {
                        if(i > 2)
                            stack.pop_back();
                        stack.push_back(i);
                    } else {
                        break;
                    }
                }
                continue;
            }
            //��������� � ���� ��������� ��� ������� ��������
            else{
                stack.push_back(1);
                for (unsigned int i = 2; i < max_square; i++) {
                    if ((table.emptyPlaceExist(i))) {
                        stack.push_back(i);
                    } else {
                        break;
                    }
                }
                continue;
            }

        }
        //���� �� ������� �������� ������� ��� ����� ������� �� 1 ������ �������������, ����������
        else {
#if OUTPUT_ON == 1
            fout << "������� �� ����������, ������������ � �����������\n";
#endif
            if (stack.back() == 1) {
                if (cur_sol.back() == 1) {
                    while (cur_sol.back() == 1) {
                        cur_sol.pop_back();
                        table.deleteLast();
                    }
                    stack.pop_back();
                    cur_sol.pop_back();
                    table.deleteLast();
                    continue;
                } else {
                    stack.pop_back();
                    cur_sol.pop_back();
                    table.deleteLast();
                    continue;
                }
            }
            else {
                if(cur_sol.size() == max_length-1){
                    while(stack.back() != 1)
                        stack.pop_back();
                    continue;
                }
                stack.pop_back();
                continue;
            }
        }
    }
#if TIMER_ON == 1
    unsigned int end_time = clock();
    double search_time = double(end_time - start_time)/CLOCKS_PER_SEC;
    std::cout<< "Search time: " << search_time << " sec." << std::endl;
#endif
    table.printSolutions(coeff);
    fout.close();
    return 0;
}

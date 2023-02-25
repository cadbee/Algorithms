#include <iostream>
#include "table.h"
#include <vector>
#include <ctime>
#include <cmath>

#define TIMER_ON 0 // 1 или 0, в зависимости от необходимости таймера

unsigned int findMinDivisor(unsigned int N) {
    //нахождение минимального делителя
    for (unsigned int i = 2; i < N + 1; i++) {
        if (N % i == 0)
            return i;
    }
    return 0;
}

void print_stack(std::vector<unsigned int> stack, std::ofstream& out) {
    //вывод содержимого стека в out
    out << "Стек: ";
    for (auto& x:stack)
        out<<x<<' ';
    out<<std::endl;
}

bool is_prime(unsigned int N) {
    //проверка числа на простоту
    for (unsigned int i = 2; i <= sqrt(N); i++) {
        if (N % i == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    //открываем файл для записи промежуточных решений и процесса
    std::ofstream fout;
    fout.open("../logs/output.txt", std::ios::in | std::ios::out | std::ios::trunc);
    if (!fout) {
        std::cerr << "Error: Unable to open logs files" << std::endl;
        return 1;
    }
    //создаем векторы стека и текущего решения
    std::vector<unsigned int> stack;
    std::vector<unsigned int> cur_sol;
    unsigned int N;
    std::cin >> N;
    //находим минимальный делитель и коэффициент сжатия для считанного размера квадрата
    unsigned int smaller_n = findMinDivisor(N);
    unsigned int coeff = N / smaller_n;
    Table table = Table(smaller_n, fout);
    N = smaller_n;
    unsigned int max_square; //максимальная сторона квадрата для вставки
    unsigned int max_length = 2 * N + 1; //максимальное кол-во квадратов в решении
    //для простых чисел задаем отдельные начальные условия
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
    unsigned int last_append; //последний добавленный квадрат
#if TIMER_ON == 1
    unsigned int start_time =  clock();
#endif
    while (!stack.empty()) {
#if OUTPUT_ON == 1
        print_stack(stack, fout);
        table.printTable();
#endif
        //проверяем, можно ли вставить квадрат и актуальность решения
        if ((cur_sol.size() < max_length-1) && table.paste(stack.back())) {
#if OUTPUT_ON == 1
            fout << "Вставляем квадрат со стороной " << stack.back() << " в таблицу\n";
#endif
            cur_sol.push_back(stack.back());
            last_append = stack.back();
            stack.pop_back();
            //если таблица полностью заполнена, добавляем решение в список решений
            if (table.isFull()) {
                    table.addToSolutions(cur_sol, coeff);
                    max_length = cur_sol.size();
                    stack.push_back(1);
#if OUTPUT_ON == 1
                    fout << "Расположение квадратов в решении:\n";
                    table.printTable();
                    fout<<std::endl;
#endif
                    continue;
            }
            //если длина решения на 1 меньше максимального, откатываем решение
            else if(cur_sol.size() == max_length-1) {
#if OUTPUT_ON == 1
                fout << "Решение не оптимально, возвращаемся к предыдущему\n";
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
            //если длина решения на 2 меньше максимального, добавляем в стек только максимально возможный для вставки квадрат
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
            //добавляем в стек возможные для вставки квадраты
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
        //если не удалось вставить квадрат или длина решения на 1 меньше максимального, откатываем
        else {
#if OUTPUT_ON == 1
            fout << "Решение не оптимально, возвращаемся к предыдущему\n";
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

#include "table.h"

Table::Table(unsigned int n, std::ofstream& out):fout(out), N(n) {
    //заполняем таблицу нулями
    table = std::vector<std::vector<unsigned int>>(N);
    for (unsigned int i = 0; i < N; i++) {
        table[i] = std::vector<unsigned int>(N);
    }
    //инициализируем список всех решений
    all_solutions = std::vector<std::vector<unsigned int>>(N);
    cur_area = 0;
    solution_count = 0;
}

bool Table::check(unsigned int c, unsigned int r, unsigned int n) {
    //проверка на возможность вставки квадрата со стороной n
    N == 3 || N == 5 ? max_index = N : max_index = int(N / 2) + 1;
    if (((n + c) > (max_index)) || ((n + r) > (max_index)))
        return false;
    for(unsigned int i = c+1; i < c+n; i++)
        if(table[i][r] != 0 || table[i][r+n-1] != 0)
            return false;
    for(unsigned int i = r+1; i < r+n; i++)
        if(table[c][i] != 0 || table[c+n-1][i] != 0)
            return false;
    return true;
}

bool Table::emptyPlaceExist(unsigned int n) {
    //проверка на возможность последующей вставки квадрата со стороной n
    N == 3 || N == 5  ? max_index = N : max_index = int(N / 2) + 1;
    for (unsigned int i = 0; i < max_index; i++) {
        for (unsigned int j = 0; j < max_index; j++) {
            if (table[i][j] == 0) {
                if (check(i, j, n)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Table::paste(unsigned int n) {
    //вставка квадрата в таблицу
    N == 3 || N == 5  ? max_index = N : max_index = int(N / 2) + 1;
    for (unsigned int i = 0; i < max_index; i++) {
        for (unsigned int j = 0; j < max_index; j++) {
            if (table[i][j] == 0) {
                if (check(i, j, n)) { //проверяем на возможность вставки
                    for (unsigned int c = i; c < i + n; c++) {
                        for (unsigned int r = j; r < j + n; r++) {
                            table[c][r] = n;
                        }
                    }
                    //увеличиваем занятую сложность и добавляем координаты в список
                    cur_area += n*n;
                    solution_coordinates.emplace_back(i, j);
                    return true;
                }
            }
        }
    }
    return false;
}

void Table::deleteLast() {
    //удаляем последний добавленный квадрат и его координаты из соответствующих векторов
    //также уменьшаем заполненную площадь
    if (!solution_coordinates.empty()) {
        std::pair<unsigned int, unsigned int> last = solution_coordinates.back();
        unsigned int n = table[last.first][last.second];
        for (unsigned int c = last.first; c < last.first + n; c++) {
            for (unsigned int r = last.second; r < last.second + n; r++) {
                table[c][r] = 0;
            }
        }
        cur_area -= n*n;
        solution_coordinates.pop_back();
    }
}

void Table::setStart(unsigned int n) {
    //задаем начальное состояние таблицы
    //заполняем ~3/4 таблицы единицами
    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < n; j++) {
            table[i][j] = 1;
        }
    }
    for (unsigned int i = 0; i < int(n / 2) + 1; i++) {
        for (unsigned int j = 0; j < int(n / 2) + 1; j++) {
            table[i][j] = 0;
        }
    }
    table[int(n / 2)][int(n / 2)] = 1;
    table[int(n / 2) - 1][int(n / 2)] = 1;
    //добавляем координаты добавленных квадратов в список координат решения
    solution_coordinates.emplace_back(int(n / 2), int(n / 2));
    solution_coordinates.emplace_back(0, int(n / 2) + 1);
    solution_coordinates.emplace_back(int(n / 2)+1, 0);
    solution_coordinates.emplace_back(int(n / 2)-1, int(n / 2));
    //вычисляем заполненную квадратами площадь
    cur_area = 1 + 2*int(n/2)*int(n/2) + int(n/2 + 1)*int(n/2 + 1);
}

bool Table::isFull() const {
    //проверка на заполненность таблицы
    if(cur_area == N*N)
        return true;
    return false;
}

void Table::addToSolutions(std::vector<unsigned int> vec, unsigned int coeff) {
    //добавляем текущее решение в список решений
    for(unsigned int i = 0; i< vec.size(); i++){
        all_solutions[solution_count].push_back(coeff*vec[i]);
        all_solutions[solution_count].push_back(solution_coordinates[i].first);
        all_solutions[solution_count].push_back(solution_coordinates[i].second);
    }
    solution_count+=1;
#if OUTPUT_ON ==1
    fout<< "Найдено новое решение: кол-во квадратов = " <<  all_solutions[solution_count-1].size()/3 << std::endl;
    for(unsigned int j = 0; j < all_solutions[solution_count-1].size(); j+=3){
        fout<< coeff*all_solutions[solution_count-1][j+1]+1 << ' ' << coeff*all_solutions[solution_count-1][j+2]+1 << ' ' << all_solutions[solution_count-1][j] << std::endl;
    }
    //fout << std::endl;
#endif
}

void Table::printSolutions(unsigned int c) {
    //выводим все, либо только оптимальное решения
#if OUTPUT_ON == 1
    for(unsigned int i = 0; i < solution_count; i++){
        fout << "Решение №" << i+1 << ": " << "кол-во квадратов = " << all_solutions[i].size()/3 << std::endl;
        for(unsigned int j = 0; j < all_solutions[i].size(); j+=3){
            fout<< c*all_solutions[i][j+1]+1 << ' ' << c*all_solutions[i][j+2]+1 << ' ' << all_solutions[i][j] << std::endl;
        }
    }
#else
    std::cout<< all_solutions[solution_count-1].size()/3 << std::endl;
    for(unsigned int j = 0; j < all_solutions[solution_count-1].size(); j+=3){
        std::cout<< c*all_solutions[solution_count-1][j+1]+1 << ' ' << c*all_solutions[solution_count-1][j+2]+1 << ' ' << all_solutions[solution_count-1][j] << std::endl;
    }
#endif
}


void Table::printTable() {
    //выводим текущее состояние таблицы
    fout<<' ';
    for(unsigned int i = 0; i < N*4-1; fout<<'-', i++);
    fout<<'\n';
    for (unsigned int i = 0; i < N; i++) {
        fout << "| ";
        for (unsigned int j = 0; j < N; j++) {
            fout << this->table[i][j] << " | ";
        }
        fout<<'\n';
        fout<<' ';
        for(unsigned int j = 0; j < N*4-1; fout<<'-', j++);
        fout<<'\n';
    }
}
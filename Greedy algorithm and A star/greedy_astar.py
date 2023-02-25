from heapq import heappop, heappush

NUM1 = 1  # = 1 для выполнения первого задания, = 0 иначе
NUM2 = 1  # = 1 для выполнения второго задания, = 0 иначе
IND = 0  # = 1 для применения индивидуализации, = 0 иначе
OUT = 1  # = 1 для вывода промежуточных данных, = 0 иначе


class Graph:
    def __init__(self):
        self.start = None  # стартовая вершина
        self.end = None  # конечная вершина
        self.graph = {}  # граф (хранит вершины как ключи и смежные к ним как значения)
        self.paths = {}  # словарь, ключи вершины, значения - откуда в них можно попасть

    def graph_init(self, ribs):
        self.start, self.end = ribs[0].split()  # инициализируем начальную и конечную вершины
        ribs.pop(0)
        if OUT == 1:
            print("\n======================ИНИЦИАЛИЗАЦИЯ ГРАФА=====================")
            print("Стартовая вершина: {}, конечная: {}".format(self.start, self.end))
        for tmp in ribs:
            tmp = tmp.split()
            if tmp[0] not in self.graph.keys():  # добавляем ребро в граф
                self.graph.update({tmp[0]: [[tmp[1], float(tmp[2])]]})
                if OUT == 1:
                    print("Добавляем ребро {} - {} с весом {}".format(tmp[0], tmp[1], tmp[2]))
            else:
                if OUT == 1:
                    print("Добавляем к {} смежную вершину {}, вес ребра = {}".format(tmp[0], tmp[1], tmp[2]))
                self.graph[tmp[0]].append([tmp[1], float(tmp[2])])  # обновляем список смежных вершин для начала ребра
        if OUT == 1:
            print("\nПолучившийся граф:")
            for i in self.graph.keys():
                print("\tВершина: {}, смежные: {}".format(i, self.graph[i]))
        if NUM2 == 1 and IND == 1:
            for i in self.graph:
                self.graph.update({i: sorted(self.graph.get(i), key=lambda x: x[1] + self.heuristic(x[0]))})  # сортируем списки смежных вершин по приоритету
            if OUT == 1:
                print("\n[IND] Граф после сортировки смежных вершин по приоритету:")
                for i in self.graph.keys():
                    print("\tВершина: {}, смежные: {}".format(i, self.graph[i]))

    def greedy_algorithm(self):
        if OUT == 1:
            print("\n=======================ЖАДНЫЙ АЛГОРИТМ========================")
        stack = [[self.start, 0]]  # инициализация стека и добавление в стек начальной вершины
        already_been = []  # список посещенных вершин
        while len(stack) != 0:
            if OUT == 1:
                print("Стек на данный момент: {}".format(stack))
            cur = stack.pop()  # берем вершину из стека
            already_been.append(cur[0])  # добавляем в список посещенных
            if OUT == 1:
                print("Достаем из стека вершину {}".format(cur[0]))
            if cur[0] == self.end:
                if OUT == 1:
                    print("Найдена конечная вершина, завершаем алгоритм")
                    print("===========================================================")
                break
            if cur[0] in self.graph:
                children = [i for i in self.graph[cur[0]] if i[0] not in already_been]  # берем смежные непосещенные вершины
                if children:
                    if OUT == 1:
                        print("\tСмежные с ней непосещенные вершины и расстояние до каждой: {}".format(children))
                    children = sorted(children, key=lambda x: x[1])[::-1]  # сортируем в порядке невозрастания стоимости
                    if OUT == 1:
                        print("\tОтсортируем в порядке невозрастания {}".format(children))
                else:
                    if OUT == 1:
                        print("\tСмежных непосещенных вершин нет")
                for child in children:  # проходимся по полученному списку смежных вершин
                    stack.append(child)  # добавляем в стек, начиная с вершины с большим весом ребра
                    self.paths.update({child[0]: cur[0]})  # обновляем переход
                    if OUT == 1:
                        print("\t\tОбновляем путь к вершине {} и добавляем ее в стек".format(child[0]))
                        print("\t\tТекущие пути {}".format(self.paths))
            elif OUT == 1:
                print("У этой вершины нет исходящих ребер")
            if OUT == 1:
                print("Переходим к следующей вершине")
                print('-'*62)

    def a_star(self):
        if OUT == 1:
            print("\n=========================АЛГОРИТМ A*==========================")
        queue = []  # очередь с приоритетом
        heappush(queue, (0, [self.start, 0]))
        distance = {}  # словарь для хранения растояний от начально до осталных верщин
        distance.update({self.start: 0})
        while len(queue) != 0:
            if OUT == 1:
                print("Очередь на данный момент: {}".format(queue))
                print("Расстояния до вершин на данный момент: {}".format(distance))
            cur = heappop(queue)[1]  # достаем из очереди элемент
            if OUT == 1:
                print("Достаем из очереди вершину {}".format(cur))
            if cur[0] == self.end:
                if OUT == 1:
                    print("Найдена конечная вершина, заканчиваем алгоритм")
                    print('='*62)
                break
            if cur[0] in self.graph:  # если от текущей верщины есть исходящие ребра
                children = [i for i in self.graph[cur[0]]]  # получаем список смежных вершин
                if OUT == 1:
                    print("\tСмежные с данной вершиной: {}".format(children))
                for child in children:  # проходимся по всем смежным верщинам
                    tmp_cost = distance[cur[0]] + self.get_weight(cur[0], child[0])  # считаем расстояние до верщины
                    if OUT == 1:
                        print("\tРасстояние до вершины {} = {}: расстояние до {} = {} + вес ребра {} -> {} = {}".format(child[0], tmp_cost, cur[0], distance[cur[0]], cur[0], child[0], self.get_weight(cur[0], child[0])))
                    if child[0] not in distance or tmp_cost < distance[child[0]]:  # если расстояния до смежной нет или полученное меньше текущего
                        if OUT == 1:
                            print("\t\tПолученное расстояние меньше текущего, обновим значение")
                        distance.update({child[0]: tmp_cost})  # обновляем расстояние до вершины
                        prioritise = tmp_cost + self.heuristic(child[0])  # считаем приоритет верщины
                        if OUT == 1:
                            print("\t\tБлизость символов {} и {} в таблице ASCII: {}".format(child[0], self.end, abs(
                                ord(child[0]) - ord(self.end))))
                            print("\t\tДобавим вершину в очередь с приоритетом = {}: полученная стоимость = {} + "
                                  "эвристика = {}".format(prioritise, tmp_cost, abs(
                                ord(child[0]) - ord(self.end))))
                        heappush(queue, (prioritise, child))  # добавляем верщшину в очередь
                        self.paths.update({child[0]: cur[0]})  # обновляем путь до вершины
                    else:
                        if OUT == 1:
                            print("\tТекущее расстояние до {} = {} <= полученного".format(child[0], distance[child[0]]))
            elif OUT == 1:
                print("Из текущей вершины нет исходящих ребер")
            if OUT == 1:
                print("Переходим к следующей вершине")
                print('-'*62)
        return self.paths

    def heuristic(self, char):  # эвристическая функция
        return abs(ord(char) - ord(self.end))

    def get_weight(self, char, destination):  # поиск веса ребра char - destination
        for i in self.graph[char]:
            if i[0] == destination:
                return i[1]

    def print_path(self):  # восстанавливаем путь
        tmp = self.end
        path = self.end
        while tmp != self.start:
            tmp = self.paths[tmp]
            path += tmp
        path = path[::-1]
        if OUT == 1:
            sum = 0
            print("Полученный путь: {}".format(path))
            for i in range(len(path) - 1):
                weight = self.get_weight(path[i], path[i + 1])
                print("\t{}. {} -{}-> {}".format(i+1, path[i], weight, path[i + 1]))
                sum += weight
            print("\tДлина пути: {}".format(sum))
        else:
            print(path)


if __name__ == '__main__':
    ribs = []  # список ребер
    tmp = input()
    while tmp:  # считывание данных
        try:
            ribs.append(tmp)
            tmp = input()
        except EOFError:
            break
    graph = Graph()  # создаем граф
    graph.graph_init(ribs)  # инициализация
    if NUM1 == 1:
        graph.greedy_algorithm()
        graph.print_path()
    if NUM2 == 1:
        graph.a_star()
        graph.print_path()

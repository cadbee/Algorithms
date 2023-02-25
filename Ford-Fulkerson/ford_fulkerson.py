import copy
from collections import deque

OUT = 1  # = 1 вывод промежуточных данных с dfs, = 2 вывод без dfs, = 0 только ответ


class Graph:
    def __init__(self, source, sink, graph):
        self.source = source  # исток
        self.sink = sink  # сток
        self.graph = graph  # сеть
        self.net = copy.deepcopy(self.graph)  # сеть с обратными ребрами
        self.max_flow = 0  # максимальный поток в сети
        self.parents = {}  # словарь родителей для вершин
        self.ribs_weight = []  # словарь хранящий величину потока на ребрах

    def set_ret(self):  # установим обратные ребра
        for vertex in list(self.graph.keys()):  # проходимся по вершинам
            for value in self.net[vertex]:  # проходимся по смежным вершинам
                if value in self.net:  # если из нее есть исходящие ребра
                    if vertex in self.net[value]:  # если есть ребро в вершину родителя, продолжаем
                        continue
                    else:
                        self.net[value].update(
                            {vertex: 0})  # иначе - создаем обратное ребро с пропускной способностью = 0
                else:
                    self.net[value] = {vertex: 0}  # иначе - создаем обратное ребро с пропускной способностью = 0

    def path_exists_dfs(self):  # пуиск пути к стоку при помощи обхода в глубину
        stack = deque()  # стек рассматриваемых вершин
        stack.append(self.source)  # добавляем в него исток
        visited = [self.source]  # создаем список уже посещенных вершин
        if OUT == 1:
            print("==========================ОБХОД В ГЛУБИНУ=========================")
            print("Создаем стек и вставляем в него исток.")
            order = []  # порядок рассмотрения верщин для проиежуточного вывода
        while stack:
            if OUT == 1:
                print("Стек на текущий момент: ", list(stack))
                print("Список посещенных вершин:", visited)
            curr = stack.popleft()  # достаем вершину из стека
            if OUT == 1:
                order.append(curr)
                print("Достаем из стека вершину {}".format(curr))
            if curr == self.sink:  # если эта вершина - сток, то путь найден
                if OUT == 1:
                    print("Найден сток, завершаем обход")
                    print("Порядок, в котором рассматривались вершины для нахождения стока:", order)
                    print('-' * 66)
                return True
            if OUT == 1:
                print("\tСмежные с этой вершиной и пропускные способности соответствующих ребер:",
                      self.net[curr])
            for child in list(self.net[curr].keys()):  # проходимся по смежным вершинам
                if OUT == 1:
                    print("\tРассмотрим вершину {}".format(child))
                    if self.net[curr][child] <= 0:
                        print("\t\tПоток, который можно пропустить по ребру {}->{} = {}".format(curr, child,
                                                                                                self.net[curr][child]))
                        print("\t\tПереходим к следующей смежной вершине")
                    elif child in visited:
                        print("\t\tВершина уже была посещена ранее")
                        print("\t\tПереходим к следующей смежной вершине")

                if self.net[curr][child] > 0 and child not in visited:  # если ребро в смежную вершину не переполнено и по нему еще не ходили
                    if OUT == 1:
                        print("\t\tДобавляем вершину в стек и переходим к следующей смежной")
                    stack.appendleft(child)  # добавляем вершину в стек
                    visited.append(child)  # отмечаем вершину как посещенную
                    self.parents[
                        child] = curr  # записываем родителя для этой смежной вершины для дальнейшего восстановления пути
            if OUT == 1:
                print("\tПодходящих смежных вершин больше нет, достаем следующую вершину из стека")
                print('-' * 66)
        if OUT == 1:
            print("Вершины в стеке закончились, пути в сток найдено не было")
        return False

    def ford_fulkerson(self):  # алгоритм Форда-Фалкерсона
        if OUT == 2 or OUT == 1:
            print("======================АЛГОРИТМ ФОРДА-ФАЛКЕРСОНА===================")
            print("Запускаем обход в глубину")
        self.set_ret()  # устанавливаем обратные ребра
        while self.path_exists_dfs():  # пока можно добраться до стока
            if OUT == 2 or OUT == 1:
                print("Путь к стоку был найден, восстановим его. Начнем из стока:")
            path = self.sink
            while path[0] != self.source:  # восстанавливаем путь к стоку
                if OUT == 2 or OUT == 1:
                    print("\tК {} мы пришли по ребру {}->{}".format(path[0], self.parents[path[0]], path[0]))
                path = self.parents[path[0]] + path  # добавляем ребро в путь
            if OUT == 2 or OUT == 1:
                print("Весь путь к стоку:", path)
            min_flow = float('inf')  # устанавливаем минимальную величину потока = inf
            if OUT == 2 or OUT == 1:
                print("Найдем максимальный поток, который можно пропустить по этому пути:")
            for i in range(
                    len(path) - 1):  # ищем максимальную величину потока, которую можно пропустить по найденному пути
                min_flow = min(min_flow, self.net[path[i]][path[i + 1]])  #
                if OUT == 2 or OUT == 1:
                    print("\tПо ребру {}->{} можно пропустить поток = {}".format(path[i], path[i + 1],
                                                                                 self.net[path[i]][path[i + 1]]))
            if OUT == 2 or OUT == 1:
                print("Тогда максимальный поток, который можно пропустить = {}".format(min_flow))
                print("Уменьшим остаточные пропускные способности ребер и увеличим на обратных:")
            for i in range(len(path) - 1):  # проходимся по ребрам в пути до стока
                if OUT == 2 or OUT == 1:
                    print(
                        "\t{}. Остаточная пропускная способность ребра {}->{} = {}".format(i + 1, path[i], path[i + 1],
                                                                                           self.net[path[i]][
                                                                                               path[i + 1]] - min_flow))
                    print("\t   Пропускная способность обратного ребра {}->{} = {}".format(path[i + 1], path[i],
                                                                                           self.net[path[i + 1]][
                                                                                               path[i]] + min_flow))
                self.net[path[i]][path[i + 1]] -= min_flow  # уменьшаем пропускную способность ребра
                self.net[path[i + 1]][path[i]] += min_flow  # увеличиваем пропускную способность обратного ребра
            self.max_flow += min_flow  # увеличиваем поток в сети
            if OUT == 2 or OUT == 1:
                print('-' * 66)
                print("Запускаем обход в глубину")

        if OUT == 2 or OUT == 1:
            print("Путей к стоку больше нет, завершаем алгоритм")
            print('-' * 66)
        for vertex in self.graph:  # проходимся по вершинам изначального графа
            for dest_vertex in self.graph[vertex]:  # проходимся по ребрам из этой вершины
                if self.graph[vertex][dest_vertex] - self.net[vertex][dest_vertex] < 0:
                    self.ribs_weight.append((vertex, dest_vertex, 0))  # если поток на ребре <0 заменяем его на = 0
                else:  # иначе - записываем в ответ ребро и величину потока на нем
                    self.ribs_weight.append((vertex, dest_vertex,
                                              self.graph[vertex][dest_vertex] - self.net[vertex][
                                                  dest_vertex]))
        self.ribs_weight.sort(key=lambda x: (x[0], x[1]))  # сортируем ответ согласно заданию


if __name__ == "__main__":
    count = int(input())
    source = input()
    sink = input()
    graph = {}
    for i in range(count):
        vertex1, vertex2, weight = input().split()
        if vertex1 in graph:
            graph[vertex1].update({vertex2: int(weight)})
        else:
            graph[vertex1] = {vertex2: int(weight)}
    graph = Graph(source, sink, graph)
    graph.ford_fulkerson()
    print(graph.max_flow)
    for i in graph.ribs_weight:
        print(i[0], i[1], i[2])

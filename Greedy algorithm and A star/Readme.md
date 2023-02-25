*Решение задачи построения пути в ориентированном графе при помощи жадного алгоритма. Жадность в данном случае понимается следующим образом: на каждом шаге выбирается последняя посещённая вершина. Переместиться необходимо в ту вершину, путь до которой является самым дешёвым из последней посещённой вершины. Каждая вершина в графе имеет буквенное обозначение ("a", "b", "c"...), каждое ребро имеет неотрицательный вес.*

    Пример входных данных:
    a e
    a b 3.0
    b c 1.0
    c d 1.0
    a d 5.0
    d e 1.0
    
    В первой строке через пробел указываются начальная и конечная вершины
    Далее в каждой строке указываются ребра графа и их вес
    Выходные данные: строка, в которой перечислены вершины, по которым необходимо пройти от начальной вершины до конечной. 

# Описание жадного алгоритма
Алгоритм начинается с начальной вершины. На каждом шаге осуществляется переход по ребру с минимальным весом по которому еще не осуществлялся переход. Если из текущей вершины нет доступных переходов, то осуществляется возврат к предыдущей. Алгоритм заканчивает работу, когда текущая вершина оказывается конечной.

    Для приведённых в примере входных данных ответом будет:	abcde
    
---
*Решение задачи построения кратчайшего пути в ориентированном графе методом А*. Каждая вершина в графе имеет буквенное обозначение ("a", "b", "c"...), каждое ребро имеет неотрицательный вес. В качестве эвристической функции следует взять близость символов, обозначающих вершины графа, в таблице ASCII.*

    Пример входных данных:
    a e
    a b 3.0
    b c 1.0
    c d 1.0
    a d 5.0
    d e 1.0
    
    В первой строке через пробел указываются начальная и конечная вершины
    Далее в каждой строке указываются ребра графа и их вес
    Выходные данные: строка, в которой перечислены вершины, по которым необходимо пройти от начальной вершины до конечной. 

# Описание алгоритма А*
Создается очередь с приоритетом. Приоритет каждой вершины складывается из известного минимального расстояния до этой вершины и значения эвристической функции. В начале работы алгоритма в эту очередь помещается начальная вершина.

На каждом шаге из очереди извлекается вершина, далее для всех смежных с ней вершин вычисляется сумма длины пути до взятой из очереди вершины и веса ребра до рассматриваемой смежной. Если полученная сумма меньше вычисленной ранее длины пути до этой смежной вершины, то ее значение обновляется, иначе – остается низменной. После этого все рассмотренные смежные вершины, которым помещаются в очередь согласно их приоритетам.

Алгоритм заканчивает работу, когда очередная взятая из очереди вершина оказывается конечной.

    Для приведённых в примере входных данных ответом будет:	ade
    
    Индивидуализация:
    Для того, чтобы перед выполнением А* выполнить предобработку графа(для каждой вершины отсортировать список смежных вершин по приоритету) необходимо установить значение переменной IND = 1.
    
---

# Оценка сложности алгоритмов
Обозначения: V — количество вершин, E — количество ребер.

Оценим временную сложность алгоритмов:  . В худшем случае для поиска пути потребуется обход всех ребер и вершин.

Для обоих алгоритмов O(E + V) — оценка используемой памяти для хранения графа (V вершин и E указателей на смежные вершины). В алгоритме A* используется дополнительная память O(V + E) для хранения очереди с приоритетом.

# Тестирование
|    <br>Ввод                                                                                               |    <br>Жадный алгоритм       |    <br>А*     |
|-----------------------------------------------------------------------------------------------------------|------------------------------|---------------|
| a e<br>a b 3.0<br>b c 1.0<br>c d 1.0<br>a d 5.0<br>d e 1.0                                                |    <br>abcde<br>   <br>      |    <br>ade    |
| a g<br>a b 3.0<br>a c 1.0<br>b d 2.0<br>b e 3.0<br>d e 4.0<br>e a 3.0<br>e f 2.0<br>a g 8.0<br>f g 1.0    |    <br>abdefg<br>   <br>     |    <br>ag     |
| b e<br>a b 1.0 <br>a c 2.0<br>b d 7.0<br>b e 8.0<br>a g 2.0<br>b g 6.0<br>c e 4.0<br>d e 4.0<br>g e 1.0   |    <br>bge<br>   <br>        |    <br>be     |
| a b<br>a b 1.0<br>a c 1.0                                                                                 |    <br>ab<br>   <br>         |    <br>ab     |
| a b<br>a b 1.0                                                                                            |    <br>ab<br>   <br>         |    <br>ab     |
| a d<br>a b 1.0<br>b c 1.0<br>c a 1.0<br>a d 8.0                                                           |    <br>ad<br>   <br>         |    <br>ad     |
| a g<br>a b 3.0<br>a c 1.0<br>b d 2.0<br>b e 3.0<br>d e 4.0<br>e a 1.0<br>e f 2.0<br>a g 8.0<br>f g 1.0    |    <br>abdefg<br>   <br>     |    <br>ag     |
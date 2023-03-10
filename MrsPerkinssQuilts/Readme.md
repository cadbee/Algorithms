*A Mrs. Perkins's quilt is a dissection of a square of side n into a number of smaller squares. The name "Mrs. Perkins's Quilt" comes from a problem in one of Dudeney's books, where he gives a solution for n=13. Unlike a perfect square dissection, however, the smaller squares need not be all different sizes. In addition, only prime dissections are considered so that patterns which can be dissected into lower-order squares are not permitted.*

Алгоритм поиска с возвратом - общий метод нахождения решений задачи, в которой требуется полный перебор всех возможных вариантов в некотором множестве. Решение задачи методом поиска с возвратом сводится к последовательному расширению частичного решения. Если на очередном шаге такое расширение провести не удается, то возвращаются к более короткому частичному решению и продолжают поиск дальше. Данный алгоритм позволяет найти все решения поставленной задачи, если они существуют. 

    Входные данные:
	  Размер квадрата для разбиения - одно целое число N (2≤N≤20).
    Выходные данные:
      Одно число K, задающее минимальное количество обрезков(квадратов), из которых можно построить квадрат заданного размера N. Далее должны идти K строк, каждая из которых должна содержать три целых числа x, y и w, задающие координаты левого верхнего угла и длину стороны соответствующего обрезка(квадрата).

# Описание алгоритма
Для решения был использован итеративный поиск с возвратом. Поиск осуществляется перебором вариантов для расстановки внутри квадрата размера N*N квадратов меньших размеров. Размеры квадрата для подстановки берутся от большего к меньшему, координаты от левого верхнего угла, к правому нижнему. Когда поле оказывается заполнено, кол-во квадратов в текущем решении сравнивается с предыдущим оптимальным решением и заносится в список решений, после чего происходит возврат к предыдущему состоянию(откат) и рассматривается другая расстановка.
Сложность алгоритма без оптимизаций является многочленом большой степени, приблизительно $О((n-1)^{n^2})$, и близка к экспоненте. Затраты по памяти не превышают $n^2$. 

# Выполнение работы
Создается поток вывода частичных решений в файл “output.txt” - fout, создаются векторы для стека – stack и текущего решения – cur_sol и считывается значение размера столешницы N.

С помощью функции finMinDivisor(N) находится минимальный простой делитель для N – smaller_n, вычисляется коэффициент сжатия и создается таблица размера smaller_n.

В соответствии с размером таблицы задаются начальные условия алгоритма. Если ее размер простое число, то вызывается метод table.setStart(n) для вставки в нее 4х квадратов, также они добавляются и в текущее решение cur_sol. В стек добавляются квадраты размеров от 1 до (N/3) включительно, а максимальное значение стороны квадрата для вставки устанавливается N/3 + 1.

Если же размер столешницы четное число, либо равен 3 или 5(3 или 5 т.к. вставлять 3 квадрата как в случае с остальными простыми числами здесь нет), то max_square = N – 1, а в стек добавляются квадраты со сторонами от 1 до N-1 включительно.

### Итеративный процесс:
Процесс продолжается, пока в стеке есть квадраты для вставки.

Берется последний добавленный в стек квадрат. Если его можно вставить в таблицу и при этом длина текущего решения не больше max_length-2, то происходит процесс вставки. После вставки происходит проверка, заполнена ли таблица. Если да, то решение добавляется в список решений и происходит откат.

Если же таблица не заполнена, то в зависимости от длины текущего решения в стек добавляется соответствующее кол-во квадратов.

Если вставить квадрат не удалось или длина текущего решения равна максимальной – 1, то происходит откат: если сверху стека лежит 1, и при этом последний добавленный квадрат тоже 1, то решение откатывается до момента, пока сверху стека не окажется значение, отличное от 1, это значение также удаляется из стека, вектора текущего решения и таблицы, и алгоритм продолжается. Иначе – из стека удаляется верхнее значение, если при этом длина текущего решения равна максимальной – 1, то значения удаляются до момента, пока сверху не окажется 1.

# Примененные оптимизации
- При нахождении хотя бы одного решения любое решение хуже завершается преждевременно и происходит откат
- Новый квадрат устанавливается в максимально возможную верхнюю левую клетку 
- Если введенная сторона квадрата четная, то решение находится за константное время путем деления квадрата на 4
- Для сторон квадрата, имеющих простые множители задача сводится к нахождению решения для квадрата со стороной равной наименьшему простому множителю
- Если сторона квадрата - простое число, то перед началом алгоритма ставится несколько квадратов, которые точно будут содержаться в разложении. Их расположение следующее: в нижний правый угол ставится квадрат со стороной n//2+1, а дальше два квадрата слева и сверху от него, также в центр «столешницы» ставится квадрат со стороной 1.
- В стек добавляются только квадраты, которые в текущий момент могут быть вставлены в таблицу
- Перед началом алгоритма в случае с простой стороной изначального квадрата в стек добавляются только квадраты размеров от 1 до N/3 включительно (при больших значениях очевидно, что решение будет не оптимальным).
- Если после добавления в таблицу квадрата длина текущего решения становится равной максимальной – 1, то происходит откат, т.к. дальнейшее рассмотрение частичного решения не имеет смысла.
- Если после добавления в таблицу квадрата длина текущего решения становится равной максимальной – 2, то в стек добавляется только квадрат с максимально возможной для вставки стороной.

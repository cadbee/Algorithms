MOD = 0  # =1, если необходимо применить индивидуализацию, иначе =0
NUM = 1  # =1, если требуется выолнить первое задание, =2 - второе
OUT = 1  # =1, если нужен подробный вывод, =0 иначе


class Node:
    # Класс узла бора
    def __init__(self, num, ntype, ch, parent):
        self.children = {}  # список детей
        self.samples = []  # список подстрок соответствующих этой вершине
        self.link = None  # суффиксная ссылка
        self.num_in_trie = num  # номер вершины в дереве
        self.type = ntype  # тип вершины
        self.char = ch  # соответствующий символ
        self.parent = parent  # ссылка на родителя


def create_trie(patterns):
    # Создаем бор и присваиваем каждой вершине порядковый номер
    root = Node(0, "ROOT", None, None)
    if OUT == 1:
        print("-----------СОЗДАЕМ БОР-----------")
        print("Добавляем вершину №", 0, "Ее тип:", root.type)
    count = 1
    # Проходимся по строкам-образцам
    for path in patterns:
        node = root
        if OUT == 1:
            print("Добавим в бор паттерн", path)
        # Проходимся по символам в образце
        for symbol in path:
            if OUT == 1:
                print("Добавляемый символ:", symbol)
            # Проверяем, есть ли символ среди потомков данной вершины
            if symbol in node.children.keys():
                # Если есть, переходим к соответсвующей вершине
                if OUT == 1:
                    print("Символ", symbol, "есть среди потомков вершины c номером", node.num_in_trie)
                node = node.children.get(symbol)
            else:
                # Иначе - создаем новую вершину
                node = node.children.setdefault(symbol, Node(count, "INNER", symbol, node))
                if OUT == 1:
                    print("Добавляем вершину №", count, "Ее тип:", node.type, "Ее родитель - вершина с номером",
                          node.parent.num_in_trie, "Соответствующий ей символ:", node.char)
                count += 1
        node.samples.append(path)
        node.type = "LEAF"
        if OUT == 1:
            print("Вершина с номером", node.num_in_trie, "конечная", "Тогда ее тип:", node.type)
            print('-' * 50)
    # Выведем получившийся бор
    if OUT == 1:
        print("ПОЛУЧИВШИЙСЯ БОР:")
        queue = []
        for node in root.children.values():
            queue.append(node)
        print("№ 0 ROOT Children:", [i.num_in_trie for i in root.children.values()])
        while len(queue) > 0:
            rnode = queue.pop(0)
            for key, unode in rnode.children.items():
                queue.append(unode)
            print("№", rnode.num_in_trie, "Char:", rnode.char, "Type:", rnode.type, "Children:",
                  [i.num_in_trie for i in rnode.children.values()])
    return root


def set_links(patterns):
    # Создаем бор и устанавливаем суффиксные ссылки его потомков на корень
    root = create_trie(patterns)
    if OUT == 1:
        print("\n-----------УСТАНОВИМ СУФФИКСНЫЕ ССЫЛКИ-----------")
    queue = []
    for node in root.children.values():
        queue.append(node)
        node.link = root
    if OUT == 1:
        print("Для всех потомков корня суффиксные ссылки будут указывать на корень")
    # Устанавливаем суффиксные ссылки для остальных узлов путем обхода дерева в ширину
    while len(queue) > 0:
        rnode = queue.pop(0)
        if OUT == 1:
            print("Берем из очереди вершину с номером", rnode.num_in_trie, "Ее символ:", rnode.char)
            print("Ее суффиксная ссылка указывает на вершину с номером:", rnode.link.num_in_trie)
        for key, unode in rnode.children.items():
            # Заносим потомков выбранной вершины в очередь
            queue.append(unode)
            fnode = rnode.link
            if OUT == 1:
                print("\tРассматриваем вершину потомок с номером:", unode.num_in_trie, "Ее символ:", unode.char)
            # Ищем подходящую суффиксную ссылку
            while fnode is not None and key not in fnode.children:
                fnode = fnode.link
                if OUT == 1:
                    if fnode:
                        print(
                            "\tСреди потомков суффиксной ссылки нет нужного символа, переходим к следующей ссылке, ее номер:",
                            fnode.num_in_trie)
            if fnode:
                unode.link = fnode.children[key]
                if OUT == 1:
                    print("\tНайдена подходящая суффиксная ссылка, номер узла в дереве:", unode.link.num_in_trie)
            else:
                if OUT == 1:
                    print("\tНе найдено подходящей ссылки, ссылаемся на корень")
                unode.link = root
            unode.samples += unode.link.samples
            if OUT == 1:
                print('-' * 60)
        if OUT == 1:
            print("Все потомки рассматриваемой вершины закончились")
            print('-' * 60)
    # Выводим суффиксные ссылки для вершин
    if OUT == 1:
        print("Все суффиксные ссылки:")
        queue = []
        for node in root.children.values():
            queue.append(node)
        print("№ 0 ROOT Link:", root.link)
        while len(queue) > 0:
            rnode = queue.pop(0)
            for key, unode in rnode.children.items():
                queue.append(unode)
            print("№", rnode.num_in_trie, "Char:", rnode.char, "Type:", rnode.type, "Link:", rnode.link.num_in_trie)
    return root


def solve(text, root, patterns, p_count, s_len):
    if OUT == 1:
        print("\n-------------НАЙДЕМ ВХОЖДЕНИЯ ВСЕХ ПАТТЕРНОВ------------")
    # Находит все возможные подстроки из набора паттернов в строке
    node = root
    if NUM == 1:
        ans = []
    else:
        ans = [0] * len(text)
    # Проходимся по всем символам строки
    for i in range(len(text)):
        if OUT == 1:
            print("Рассматриваемый символ строки", text[i], "на позиции", i + 1)
            print("Текущий узел", node.num_in_trie, "Его символ", node.char)
        while node is not None and text[i] not in node.children:
            if OUT == 1:
                print(
                    "\tПеремещаемся по суффиксным ссылкам, пока среди потомков текущего узла не будет нужного символа")
            node = node.link
            if node is not None and OUT == 1:
                print("\tПеремещаемся в узел с номером", node.num_in_trie)
        if node is None:
            node = root
            if OUT == 1:
                print("\tПеремещаемся в корень и переходим к следующему символу строки")
                print('-' * 60)
            continue
        if OUT == 1:
            print("\tСреди потомков узла с номером", node.num_in_trie,
                  "есть рассматриваемый символ, перемещаемся в этот узел")
        node = node.children[text[i]]
        # Если узел конец паттерна, записываем индекс в решение
        if len(node.samples) != 0:
            if OUT == 1:
                print("\tВ данном узле заканчиваются паттерны:", node.samples, "Их позиции соответственно:")
            for pattern in node.samples:
                pos = i - len(pattern) + 1
                if OUT == 1:
                    print("\t\t{}: {}|{}|{}".format(pos+1, text[:pos], text[pos:pos+len(pattern)], text[pos+len(pattern):]))
                if NUM == 1:
                    ans.append([pos + 1, pattern])
                    if MOD == 1:
                        break
                else:
                    for num in patterns[pattern]:
                        ind = pos - num + 1
                        if (ind >= 0) and (ind < len(ans)):
                            if MOD == 1:
                                check = 0
                                if p_count in ans:
                                    for j in range(ind - s_len + 1, ind + s_len):
                                        if (0 <= j < len(ans)) and ans[j] == p_count:
                                            check = 1
                                            if OUT == 1:
                                                print("\t\t[IND][NUM 2] НЕ увеличиваем значение ans[{}], т.к. в пределах длины обазца уже найдено вхожение".format(ind))
                                            break
                                    if check == 0:
                                        ans[ind] += 1
                                        if OUT == 1:
                                            print("\t\t[NUM 2] Увеличиваем значение ans[{}] на 1.".format(ind),"Сам массив:", ans)
                                else:
                                    ans[ind] += 1
                                    if OUT == 1:
                                        print("\t\t[NUM 2] Увеличиваем значение ans[{}] на 1.".format(ind), "Сам массив:", ans)
                            else:
                                ans[pos - num + 1] += 1
                                if OUT == 1:
                                    print("\t\t[NUM 2] Увеличиваем значение ans[{}] на 1.".format(ind), "Сам массив:", ans)
            if MOD == 1 and NUM == 1:
                if OUT == 1:
                    print("\t\t[IND] Найдено вхождение, переходим в корень")
                node = root
        elif OUT == 1:
            print(
                "Данный узел не является конечным, паттернов на этой позиции не обнаружено, переходим к следующему символу строки")
        if OUT == 1:
            print('-' * 60)
    return ans


def sep_sample(sample, wild):
    # Удаляем из образца джокеры и формируем список паттернов
    w_dict = {}
    check = 0
    part = ""
    ind = 0
    count = 0
    for i in range(len(sample)):
        if sample[i] != wild:
            if check == 0:
                part = sample[i]
                ind = i
                check = 1
                if i == len(sample) - 1:
                    count += 1
                    w_dict.setdefault(part, [])
                    w_dict.get(part).append(ind + 1)
            else:
                part += sample[i]
                if i == len(sample) - 1:
                    count += 1
                    w_dict.setdefault(part, [])
                    w_dict.get(part).append(ind + 1)
        else:
            if check == 1:
                count += 1
                check = 0
                w_dict.setdefault(part, [])
                w_dict.get(part).append(ind + 1)
            else:
                continue
    return [w_dict, count]


if __name__ == "__main__":
    text = input()
    ans = []
    if NUM == 1:
        patterns_dict = {}
        n = int(input())
        for i in range(n):
            patterns_dict.update({input(): i + 1})
        root = set_links(list(patterns_dict.keys()))
        res = solve(text, root, [], 0, 0)
        res = sorted(res, key=lambda x: (x[0], patterns_dict.get(x[1])))
        for i in res:
            ans.append([i[0], patterns_dict.get(i[1])])

    else:
        sample = input()
        wild = input()
        count_before = 0
        parts_dict, parts_count = sep_sample(sample, wild)
        # Считаем количество джкеров в начале строки
        for ch in sample:
            if ch == wild:
                count_before += 1
            else:
                break
        root = set_links(list(parts_dict.keys()))
        res = solve(text, root, parts_dict, parts_count, len(sample))
        for i in range(len(res) - len(sample) + 1):
            if res[i] == parts_count:
                ans.append([i + 1])

    if OUT == 1:
        if len(ans) != 0:
            print("ВСЕ ВХОЖДЕНИЯ:")
        else:
            print("ВХОЖДЕНИЙ НЕ НАЙДЕНО")
    k = -1
    for i in ans:
        k += 1
        if OUT == 1:
            if NUM == 1:
                print("{} {}: {}|{}|{}".format(*i, text[:i[0]-1], text[i[0]-1:i[0]-1+len(res[k][1])], text[i[0]-1+len(res[k][1]):]))
            else:
                print("{}: {}|{}|{}".format(*i, text[:i[0] - 1], text[i[0] - 1:i[0]-1+len(sample)], text[i[0]-1+len(sample):]))
        else:
            print(*i)

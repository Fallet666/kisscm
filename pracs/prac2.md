## Задача 1

Вывести служебную информацию о пакете matplotlib (Python). Разобрать основные элементы содержимого файла со служебной информацией из пакета.

```bash
pip show matplotlib
```
<img width="310" alt="Снимок экрана 2024-09-16 в 14 07 45" src="https://github.com/user-attachments/assets/5248c335-a26e-4db2-b6d8-79d36103fc35">

Как получить пакет без менеджера пакетов, прямо из репозитория?
```bash
git clone https://github.com/matplotlib/matplotlib.git
```
<img width="482" alt="Снимок экрана 2024-09-16 в 16 41 51" src="https://github.com/user-attachments/assets/d4eee5a3-b006-41ba-9b67-2aa2018eb614">

## Задача 2
Вывести служебную информацию о пакете express (JavaScript). Разобрать основные элементы содержимого файла со служебной информацией из пакета.

```bash
npm info express
```

<img width="1105" alt="Снимок экрана 2024-09-16 в 16 46 04" src="https://github.com/user-attachments/assets/90a18069-9310-428f-9699-1d427974646d">

Как получить пакет без менеджера пакетов, прямо из репозитория?
```bash
git clone https://github.com/expressjs/express.git
```
<img width="597" alt="Снимок экрана 2024-09-16 в 16 52 54" src="https://github.com/user-attachments/assets/c44ffea4-f6a2-4812-bfc3-972de906f25b">

## Задача 3
Сформировать graphviz-код и получить изображения зависимостей matplotlib и express.

Следующие задачи можно решать с помощью инструментов на выбор:

```bash
echo 'digraph G { node [shape=box]; matplotlib [label="matplotlib"]; numpy [label="numpy"]; pillow [label="pillow"]; cycler [label="cycler"]; matplotlib -> numpy; matplotlib -> pillow; matplotlib -> cycler; }' > matplotlib.dot
echo 'digraph G { node [shape=box]; express [label="express"]; accepts [label="accepts"]; array_flatten [label="array-flatten"]; content_type [label="content-type"]; express -> accepts; express -> array_flatten; express -> content_type; }' > express.dot
dot -Tpng matplotlib.dot -o matplotlib.png
dot -Tpng express.dot -o matplotlib.png
fim matplotlib.png
fim matplotlib.png
```

<img width="1440" alt="Снимок экрана 2024-09-16 в 17 36 44" src="https://github.com/user-attachments/assets/363aed25-fec3-4f5d-86fd-7d1937070fb2">
<img width="1440" alt="Снимок экрана 2024-09-16 в 17 38 04" src="https://github.com/user-attachments/assets/2c5edd34-6124-4c0e-82ff-59e41efc6827">

## Задача 4
Следующие задачи можно решать с помощью инструментов на выбор:

Решатель задачи удовлетворения ограничениям (MiniZinc).
SAT-решатель (MiniSAT).
SMT-решатель (Z3).
Изучить основы программирования в ограничениях. Установить MiniZinc, разобраться с основами его синтаксиса и работы в IDE.

Решить на MiniZinc задачу о счастливых билетах. Добавить ограничение на то, что все цифры билета должны быть различными (подсказка: используйте all_different). Найти минимальное решение для суммы 3 цифр.

```bash
sudo apt install minizinc
echo 'include "globals.mzn"; array[1..6] of var 0..9: digits; constraint sum(digits[1..3]) = sum(digits[4..6]); constraint all_different(digits); solve minimize sum(digits[1..3]); output ["digits: \(digits)"];' > lucky_ticket.mzn
minizinc lucky_ticket.mzn
```

<img width="277" alt="Снимок экрана 2024-09-23 в 13 19 17" src="https://github.com/user-attachments/assets/079035c5-3bb2-4e69-9d1a-bbe229a1341a">

## Задача 5
Решить на MiniZinc задачу о зависимостях пакетов для рисунка, приведенного ниже.

![pubgrub](https://github.com/user-attachments/assets/d1b61a1e-18dd-4e9c-9b58-498bfb48492b)

```minizinc
enum Package = {
  root,
  menu_1_0_0,
  menu_1_1_0,
  menu_1_2_0,
  menu_1_3_0,
  menu_1_4_0,
  menu_1_5_0,

  dropdown_1_8_0,
  dropdown_2_0_0,
  dropdown_2_1_0,
  dropdown_2_2_0,
  dropdown_2_3_0,

  icons_1_0_0,
  icons_2_0_0,
};

array[1..5] of set of Package: targets = [
  1: { icons_1_0_0 },
  2: { menu_1_0_0, menu_1_5_0 },
  3: { dropdown_1_8_0 },
  4: { dropdown_2_0_0, dropdown_2_3_0 },
  5: { icons_2_0_0 }
];

% set points to targets array
array[Package] of set of 1..5: dependencies = [
  root: { 1, 2 },

  menu_1_0_0: { 3 },
  menu_1_1_0: { 4 },
  menu_1_2_0: { 4 },
  menu_1_3_0: { 4 },
  menu_1_4_0: { 4 },
  menu_1_5_0: { 4 },

  dropdown_1_8_0: {},
  dropdown_2_0_0: { 5 },
  dropdown_2_1_0: { 5 },
  dropdown_2_2_0: { 5 },
  dropdown_2_3_0: { 5 },

  icons_1_0_0: {},
  icons_2_0_0: {},
];

array[Package] of var opt (1..100): install_order;

constraint occurs(install_order[root]);

constraint forall(p in Package where occurs(install_order[p])) (
  forall(dep in dependencies[p]) (
    exists(t in targets[dep]) (
      occurs(install_order[t]) /\
      install_order[t] < install_order[p]
    )
  )
);

output [
  if fix(occurs(install_order[p]))
  then "\(p): \(install_order[p])\n"
  else ""
  endif | p in Package
];
  ```

<img width="106" alt="Снимок экрана 2024-10-07 в 17 43 05" src="https://github.com/user-attachments/assets/ab7ef726-ff77-4675-88a0-e1ac3f12b245">

## Задача 6

Решить на MiniZinc задачу о зависимостях пакетов для следующих данных:
```
root 1.0.0 зависит от foo ^1.0.0 и target ^2.0.0.
foo 1.1.0 зависит от left ^1.0.0 и right ^1.0.0.
foo 1.0.0 не имеет зависимостей.
left 1.0.0 зависит от shared >=1.0.0.
right 1.0.0 зависит от shared <2.0.0.
shared 2.0.0 не имеет зависимостей.
shared 1.0.0 зависит от target ^1.0.0.
target 2.0.0 и 1.0.0 не имеют зависимостей.
```
Решение
```minizinc
enum Package = {
  root_1_0_0,

  foo_1_1_0,
  foo_1_0_0,

  left_1_0_0,
  right_1_0_0,

  shared_2_0_0,
  shared_1_0_0,

  target_2_0_0,
  target_1_0_0,
};

int: n = 7;

array[1..n] of set of Package: targets = [
  % Deps of root 1.0.0
  1: { foo_1_1_0, foo_1_0_0 },
  2: { target_2_0_0 },

  % Deps of foo 1.1.0
  3: { left_1_0_0 },
  4: { right_1_0_0 },

  % Deps of left 1.0.0
  5: { shared_1_0_0, shared_2_0_0 },

  % Deps of right 1.0.0
  6: { shared_1_0_0 },

  % Deps of shared 1.0.0
  7: { target_1_0_0 },
];

% set points to targets array
array[Package] of set of 1..n: dependencies = [
  root_1_0_0: { 1, 2 },
  foo_1_1_0: { 3, 4 },
  left_1_0_0: { 5 },
  right_1_0_0: { 6 },
  shared_1_0_0: { 7 },

  foo_1_0_0: { },
  shared_2_0_0: { },

  target_2_0_0: { },
  target_1_0_0: { },
];

array[Package] of var opt (1..100): install_order;

constraint occurs(install_order[root_1_0_0]);

constraint forall(p in Package where occurs(install_order[p])) (
  forall(dep in dependencies[p]) (
    exists(t in targets[dep]) (
      occurs(install_order[t]) /\
      install_order[t] < install_order[p]
    )
  )
);

output [
  if fix(occurs(install_order[p]))
  then "\(p): \(install_order[p])\n"
  else ""
  endif | p in Package
];
```
<img width="102" alt="Снимок экрана 2024-10-07 в 17 44 52" src="https://github.com/user-attachments/assets/b9c524ea-4c80-407a-bd99-7201790fc5ad">

## Задача 7

Представить задачу о зависимостях пакетов в общей форме. Здесь необходимо действовать аналогично реальному менеджеру пакетов. То есть получить описание пакета, а также его зависимости в виде структуры данных. Например, в виде словаря. В предыдущих задачах зависимости были явно заданы в системе ограничений. Теперь же систему ограничений надо построить автоматически, по метаданным.

```python
# Пример структуры данных с зависимостями и версиями
packages = {
    "root": {"dependencies": ["foo", "target"], "version": None},
    "foo": {"dependencies": ["left", "right"], "version": None},
    "left": {"dependencies": ["shared>=1.0.0"], "version": None},
    "right": {"dependencies": ["shared<2.0.0"], "version": None},
    "shared": {"dependencies": [], "version": None},
    "target": {"dependencies": [], "version": None},
}

def generate_minizinc_code(packages):
    package_names = ', '.join(packages.keys())
    # Генерация массива установленных пакетов
    minizinc_code = f"enum PACKAGES = {{{package_names}}};\n"
    minizinc_code += "array[PACKAGES] of var 0..1: installed;\n\n"
    # Добавляем условие для root
    minizinc_code += "constraint installed[root] == 1;\n"
    # Генерация ограничений
    for package, details in packages.items():
        dependencies = details["dependencies"]
        if dependencies:
            dep_constraints = []
            for dep in dependencies:
                if '>' in dep or '<' in dep or '=' in dep:
                    dep_name = dep.split('>=')[0].split('<')[0].split('=')[0]
                    dep_constraints.append(f"installed[{dep_name}] == 1")
                else:
                    dep_constraints.append(f"installed[{dep}] == 1")
            constraint = "constraint installed[{}] == 1 -> ({});\n".format(
                package, ' /\\ '.join(dep_constraints)
            )
            minizinc_code += constraint
    minizinc_code += "\nsolve minimize sum(installed);\n"
    minizinc_code += 'output ["Installed packages: ", show(installed)];\n'
    return minizinc_code


# Генерация и вывод MiniZinc кода
minizinc_code = generate_minizinc_code(packages)
print(minizinc_code)
```

Вывод
```enum PACKAGES = {root, foo, left, right, shared, target};
array[PACKAGES] of var 0..1: installed;

constraint installed[root] == 1;
constraint installed[root] == 1 -> (installed[foo] == 1 /\ installed[target] == 1);
constraint installed[foo] == 1 -> (installed[left] == 1 /\ installed[right] == 1);
constraint installed[left] == 1 -> (installed[shared] == 1);
constraint installed[right] == 1 -> (installed[shared] == 1);

solve minimize sum(installed);
output ["Installed packages: ", show(installed)];```
```
Вывод: 
MiniZinc: Installed packages: [1, 1, 1, 1, 1, 1]
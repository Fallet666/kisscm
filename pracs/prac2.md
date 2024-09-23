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
% Определяем пакеты
  enum PACKAGES = {
      root, 
      menu_1_0_0, menu_1_1_0, menu_1_2_0, menu_1_3_0, menu_1_4_0, menu_1_5_0, 
      dropdown_2_0_0, dropdown_2_1_0, dropdown_2_2_0, dropdown_2_3_0, dropdown_1_8_0,
      icons_1_0_0, icons_2_0_0
  };
  
  % Переменные, указывающие, установлен ли пакет (1) или нет (0)
  array[PACKAGES] of var 0..1: installed;
  
  % Обязательно устанавливаем root
  constraint
      installed[root] == 1;
  
  % Ограничения зависимостей
  constraint
      (installed[root] == 1) -> (installed[menu_1_0_0] == 1 /\ installed[menu_1_5_0] == 1 /\ installed[icons_1_0_0] == 1) /\
      (installed[menu_1_5_0] == 1) -> (installed[dropdown_2_3_0] == 1 /\ installed[dropdown_2_0_0] == 1) /\
      (installed[menu_1_4_0] == 1) -> (installed[dropdown_2_3_0] == 1 /\ installed[dropdown_2_0_0] == 1) /\
      (installed[menu_1_3_0] == 1) -> (installed[dropdown_2_3_0] == 1 /\ installed[dropdown_2_0_0] == 1) /\
      (installed[menu_1_2_0] == 1) -> (installed[dropdown_2_3_0] == 1 /\ installed[dropdown_2_0_0] == 1) /\
      (installed[menu_1_1_0] == 1) -> (installed[dropdown_2_3_0] == 1 /\ installed[dropdown_2_0_0] == 1) /\
      (installed[menu_1_0_0] == 1) -> (installed[dropdown_1_8_0] == 1) /\
      (installed[dropdown_2_0_0] == 1) -> (installed[icons_2_0_0] == 1) /\
      (installed[dropdown_2_1_0] == 1) -> (installed[icons_2_0_0] == 1) /\
      (installed[dropdown_2_2_0] == 1) -> (installed[icons_2_0_0] == 1) /\
      (installed[dropdown_2_3_0] == 1) -> (installed[icons_2_0_0] == 1);
  
  % Целевая функция: минимизируем количество установленных пакетов
  solve minimize sum(installed);
  
  % Выводим результат
  output [
      "Installed packages: ", show(installed)
  ];
  ```

<img width="480" alt="Снимок экрана 2024-09-23 в 13 35 29" src="https://github.com/user-attachments/assets/b15a069b-f8a9-458d-8fca-cc64ae832f3e">

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
 % Определяем пакеты
  enum PACKAGES = {
      root, 
      foo_1_0_0, foo_1_1_0, 
      left_1_0_0, right_1_0_0, 
      shared_1_0_0, shared_2_0_0, 
      target_1_0_0, target_2_0_0
  };
  
  % Переменные, указывающие, установлен ли пакет (1) или нет (0)
  array[PACKAGES] of var 0..1: installed;
  
  % Ограничения зависимостей
  constraint
      (installed[root] == 1) -> (installed[foo_1_1_0] == 1 /\ installed[target_2_0_0] == 1) /\
      (installed[foo_1_1_0] == 1) -> (installed[left_1_0_0] == 1 /\ installed[right_1_0_0] == 1) /\
      (installed[left_1_0_0] == 1) -> (installed[shared_1_0_0] == 1) /\
      (installed[right_1_0_0] == 1) -> (installed[shared_2_0_0] == 1) /\ (installed[shared_1_0_0] == 0) /\
      (installed[shared_1_0_0] == 1) -> (installed[target_1_0_0] == 1);
  
  % Обязательно устанавливаем root
  constraint
      installed[root] == 1;
  
  % Целевая функция: минимизируем количество установленных пакетов
  solve minimize sum(installed);
  
  % Выводим результат
  output [
      "Installed packages: ", show(installed)
  ];
```
<img width="380" alt="Снимок экрана 2024-09-23 в 13 53 13" src="https://github.com/user-attachments/assets/2b57693e-0b8a-4306-ba97-cbadbffd77c6">

## Задача 7

Представить задачу о зависимостях пакетов в общей форме. Здесь необходимо действовать аналогично реальному менеджеру пакетов. То есть получить описание пакета, а также его зависимости в виде структуры данных. Например, в виде словаря. В предыдущих задачах зависимости были явно заданы в системе ограничений. Теперь же систему ограничений надо построить автоматически, по метаданным.


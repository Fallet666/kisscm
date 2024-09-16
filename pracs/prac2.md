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


## Задача 1
```bash
cut -d: -f1 /etc/passwd | sort
```
<img width="535" alt="Снимок экрана 2024-09-02 в 17 23 10" src="https://github.com/user-attachments/assets/1da34af6-0a62-4d29-a974-f489c2641102">

## Задача 2
```bash
cat /etc/protocols | tail -n 5 | sort -nrk2 | awk '{print $2, $1}'
```
<img width="649" alt="Снимок экрана 2024-09-02 в 17 23 50" src="https://github.com/user-attachments/assets/fd300c35-fb6f-444a-9bca-ed7ad99922dd">

## Задача 3

```bash
#!/bin/bash
string=$1
size=${#string}
echo -n "+"
for ((i=-2;i<size;i++))
do
echo -n "-"
done
echo "+"
echo "| $string |"
echo -n "+"
for ((i=-2;i<size;i++))
do
echo -n "-"
done
echo "+"
```

<img width="419" alt="Снимок экрана 2024-09-02 в 17 46 44" src="https://github.com/user-attachments/assets/1ee736f0-1cc2-4ecb-b299-fbaf5e5179ac">


## Задача 4

```bash
grep -o '\b[a-zA-Z_][a-zA-Z0-9_]*\b' main.cpp | sort | uniq
```
<img width="624" alt="Снимок экрана 2024-09-02 в 17 24 30" src="https://github.com/user-attachments/assets/74f37063-b7ef-4d1d-bce8-916bae674247">


## Задача 5

```bash
#!/bin/bash

chmod +x "$1"
sudo cp "$1" /usr/local/bin/

```
<img width="357" alt="Снимок экрана 2024-09-08 в 22 13 00" src="https://github.com/user-attachments/assets/cd705903-4b9f-491d-865a-c49d69fdf23e">


## Задача 6

```bash
#!/bin/bash

for file in "$@"; do
  # Проверка на наличие допустимого расширения
  if [[ "$file" =~ \.(c|js|py)$ ]]; then
    first_line=$(head -n 1 "$file")

    # Проверка на комментарий в первой строке для разных типов файлов
    if [[ "$file" =~ \.c$ && "$first_line" =~ ^// ]] || \
       [[ "$file" =~ \.js$ && "$first_line" =~ ^// ]] || \
       [[ "$file" =~ \.py$ && "$first_line" =~ ^# ]]; then
      echo "$file has a comment in the first line."
    else
      echo "$file does not have a comment in the first line."
    fi
  fi
done
```
<img width="376" alt="Снимок экрана 2024-09-08 в 22 11 31" src="https://github.com/user-attachments/assets/815b94e6-90cc-4130-899b-05531ec06a77">

## Задача 7

```bash
#!/bin/bash

find "$1" -type f -exec md5sum {} + | sort | uniq -w32 -dD
```
так должно работать, но проверить я не могу (на MacOS нет w32)

```bash
#!/bin/bash

# Создание временного файла для хранения хэшей
temp_file=$(mktemp)

# Поиск всех файлов в указанной директории и вычисление их MD5-хэшей
find "$1" -type f -exec md5 -r {} + -maxdepth 1 | sort > "$temp_file"

# Нахождение и вывод дубликатов
awk '{seen[substr($0, 1, 32)]++} seen[substr($0, 1, 32)] == 2 {print $2}' "$temp_file"

rm "$temp_file"
```
<img width="397" alt="Снимок экрана 2024-09-08 в 22 50 38" src="https://github.com/user-attachments/assets/a86d58f7-ea80-46f8-af34-aa86a9c13d41">




## Задача 8

```bash
#!/bin/bash

find . -name "*.$1" -print0 -maxdepth 1 | tar -czvf archive.tar.gz --null -T -
```
<img width="340" alt="Снимок экрана 2024-09-08 в 22 23 35" src="https://github.com/user-attachments/assets/453333e8-ce6e-4a1c-87bb-2685e01c6504">


## Задача 9

```bash
#!/bin/bash

sed 's/    /\t/g' "$1" > "$2"
```
<img width="486" alt="Снимок экрана 2024-09-08 в 22 26 33" src="https://github.com/user-attachments/assets/c1d2c237-63eb-45ad-9964-6b90f018cb2d">
<img width="243" alt="Снимок экрана 2024-09-08 в 22 26 56" src="https://github.com/user-attachments/assets/8da96636-3d24-4327-960c-daffcb06b623">
<img width="202" alt="Снимок экрана 2024-09-08 в 22 27 03" src="https://github.com/user-attachments/assets/82e8b5e3-fd30-4a13-850a-59fb2933ea81">



## Задача 10

```bash
#!/bin/bash

find "$1" -type f -empty -name "*.txt"
```

<img width="907" alt="Снимок экрана 2024-09-08 в 22 29 17" src="https://github.com/user-attachments/assets/664cf864-43fd-444f-bb70-6d0e736a639a">


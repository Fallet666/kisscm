# Практика 4

## Задание 1

<img width="919" alt="Снимок экрана 2024-11-04 в 19 39 21" src="https://github.com/user-attachments/assets/06909d97-1574-45cf-8795-1924018a0d13">

## Задание 2

Создаем локальный Git-репозиторий:
```bash
mkdir project
cd project
git init
```
Устанавливаем имя и почту пользователя 
```bash
git config user.name "Alex Korotkov"
git config user.email "Fallet666@users.noreply.github.com"
```
Создаем файл prog.py с произвольными данными и добавляем его в репозиторий
```bash
echo "# Sample Python program" > prog.py
git add prog.py
git commit -m "first commit"
```
Ответ терминала
```bash
[main (root-commit) 1334cca] first commit
 1 file changed, 1 insertion(+)
 create mode 100644 prog.py
```

## Задание 3

Создаем bare-репозиторий рядом с локальным
```bash
cd ..
git init --bare server.git
```

```bash
Initialized empty Git repository in /Users/user/Downloads/server.git/
```

Добавляем сервер как удаленный репозиторий и пушим содержимое
```bash
cd project
git remote add server ../server.git
git remote -v
git push -u server main
```
```bash
server	../server.git (fetch)
server	../server.git (push)
Enumerating objects: 3, done.
Counting objects: 100% (3/3), done.
Writing objects: 100% (3/3), 258 bytes | 258.00 KiB/s, done.
Total 3 (delta 0), reused 0 (delta 0), pack-reused 0
To ../server.git
 * [new branch]      main -> main
branch 'main' set up to track 'server/main'.
```

Клонируем server в отдельную папку и задаем имя и почту
```bash
cd ..
git clone server.git coder2_project
cd coder2_project
git config user.name "Alex Korotkov"
git config user.email "Fallet666@users.noreply.github.com"
```
```bash
Cloning into 'coder2_project'...
done.
```

Добавляем файл readme.md и пушим изменения на сервер
```bash
echo "# Description of the project" > readme.md
git add readme.md
git commit -m "docs"
git push
```
```bash
[main 3701e30] docs
 1 file changed, 1 insertion(+)
 create mode 100644 readme.md
Enumerating objects: 4, done.
Counting objects: 100% (4/4), done.
Delta compression using up to 8 threads
Compressing objects: 100% (2/2), done.
Writing objects: 100% (3/3), 325 bytes | 325.00 KiB/s, done.
Total 3 (delta 0), reused 0 (delta 0), pack-reused 0
To /Users/user/Downloads/server.git
   1334cca..3701e30  main -> main
```

Возвращаемся к coder1, получаем актуальные данные, добавляем информацию об авторе, и отправляем изменения
```bash
cd ../project
git pull server main
echo "## Authors\nCoder 1" >> readme.md
git add readme.md
git commit -m "coder 1 info"
git push
```
```bash
remote: Enumerating objects: 4, done.
remote: Counting objects: 100% (4/4), done.
remote: Compressing objects: 100% (2/2), done.
remote: Total 3 (delta 0), reused 0 (delta 0), pack-reused 0
Unpacking objects: 100% (3/3), 305 bytes | 305.00 KiB/s, done.
From ../server
 * branch            main       -> FETCH_HEAD
   1334cca..3701e30  main       -> server/main
Updating 1334cca..3701e30
Fast-forward
 readme.md | 1 +
 1 file changed, 1 insertion(+)
 create mode 100644 readme.md
[main da025c3] coder 1 info
 1 file changed, 2 insertions(+)
Enumerating objects: 5, done.
Counting objects: 100% (5/5), done.
Delta compression using up to 8 threads
Compressing objects: 100% (2/2), done.
Writing objects: 100% (3/3), 346 bytes | 346.00 KiB/s, done.
Total 3 (delta 0), reused 0 (delta 0), pack-reused 0
To ../server.git
   3701e30..da025c3  main -> main
```

Coder 2 добавляет свою информацию, решает конфликты и пушит на сервер
```bash
cd ../coder2_project
git pull --rebase
echo "Coder 2" >> readme.md
git add readme.md
git commit -m "coder 2 info"
git push
```
```bash
remote: Enumerating objects: 5, done.
remote: Counting objects: 100% (5/5), done.
remote: Compressing objects: 100% (2/2), done.
remote: Total 3 (delta 0), reused 0 (delta 0), pack-reused 0
Unpacking objects: 100% (3/3), 326 bytes | 326.00 KiB/s, done.
From /Users/user/Downloads/server
   3701e30..da025c3  main       -> origin/main
Updating 3701e30..da025c3
Fast-forward
 readme.md | 2 ++
 1 file changed, 2 insertions(+)
[main 49388c1] coder 2 info
 1 file changed, 1 insertion(+)
Enumerating objects: 5, done.
Counting objects: 100% (5/5), done.
Delta compression using up to 8 threads
Compressing objects: 100% (3/3), done.
Writing objects: 100% (3/3), 316 bytes | 316.00 KiB/s, done.
Total 3 (delta 0), reused 0 (delta 0), pack-reused 0
To /Users/user/Downloads/server.git
   da025c3..49388c1  main -> main
```

## Задание 4
```python
import subprocess

def get_git_objects():
    # Получаем список всех объектов (хешей) в репозитории
    result = subprocess.run(["git", "rev-list", "--all", "--objects"], capture_output=True, text=True)
    if result.returncode != 0:
        print("Ошибка при получении списка объектов:", result.stderr)
        return []
    # Парсим вывод и извлекаем хеши объектов
    return [line.split()[0] for line in result.stdout.splitlines() if line]

def show_object_content(obj_hash):
    # Выводим содержимое объекта с помощью git cat-file -p
    result = subprocess.run(["git", "cat-file", "-p", obj_hash], capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Ошибка при выводе содержимого объекта {obj_hash}:", result.stderr)
    else:
        print(f"\n--- Содержимое объекта {obj_hash} ---\n")
        print(result.stdout)

def main():
    # Получаем все объекты и выводим их содержимое
    objects = get_git_objects()
    for obj_hash in objects:
        show_object_content(obj_hash)

if __name__ == "__main__":
    main()
```

Результат

```bash
/usr/local/bin/python3.11 /Users/user/Downloads/project/prog.py

--- Содержимое объекта da025c3e36a14f0b64ffab6b18824b5fde8bf2f1 ---

tree d5a5f8ade8540cc988954c22f3dc0d38eb5a6984
parent 3701e301d3ece44de2dbb4e1da31ac5e252488c1
author Alex Korotkov <115859651+Fallet666@users.noreply.github.com> 1730734308 +0300
committer Alex Korotkov <115859651+Fallet666@users.noreply.github.com> 1730734308 +0300

coder 1 info


--- Содержимое объекта 3701e301d3ece44de2dbb4e1da31ac5e252488c1 ---

tree 9a4878997c8b7f626bdd2f3d8c0604fafa93ad2e
parent 1334ccaa928b9619fa8620e0de22e024b81f1900
author Alex Korotkov <115859651+Fallet666@users.noreply.github.com> 1730734198 +0300
committer Alex Korotkov <115859651+Fallet666@users.noreply.github.com> 1730734198 +0300

docs


--- Содержимое объекта 1334ccaa928b9619fa8620e0de22e024b81f1900 ---

tree a0eadedc56e2b7dcb61b25ce4547e6f18fadb9fc
author Alex Korotkov <115859651+Fallet666@users.noreply.github.com> 1730733601 +0300
committer Alex Korotkov <115859651+Fallet666@users.noreply.github.com> 1730733601 +0300

first commit


--- Содержимое объекта d5a5f8ade8540cc988954c22f3dc0d38eb5a6984 ---

100644 blob 11832af078d315b7eb0f2f150a4301e6e1e2b866	prog.py
100644 blob 688f7ef6fa215b707b08d64c771a0ae43adb333f	readme.md


--- Содержимое объекта 11832af078d315b7eb0f2f150a4301e6e1e2b866 ---

# Sample Python program


--- Содержимое объекта 688f7ef6fa215b707b08d64c771a0ae43adb333f ---

# Description of the project
## Authors
Coder 1


--- Содержимое объекта 9a4878997c8b7f626bdd2f3d8c0604fafa93ad2e ---

100644 blob 11832af078d315b7eb0f2f150a4301e6e1e2b866	prog.py
100644 blob e7891f922b6e3d35a264b38c75ec9104e8d4e005	readme.md


--- Содержимое объекта e7891f922b6e3d35a264b38c75ec9104e8d4e005 ---

# Description of the project


--- Содержимое объекта a0eadedc56e2b7dcb61b25ce4547e6f18fadb9fc ---

100644 blob 11832af078d315b7eb0f2f150a4301e6e1e2b866	prog.py


Process finished with exit code 0
```
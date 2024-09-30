ЗАДАНИЕ 1. Реализовать на Jsonnet приведенный ниже пример в формате JSON. Использовать в реализации свойство программируемости и принцип DRY.

РЕШЕНИЕ:
```
    groups: [
      "ИКБО-" + std.asString(x) + "-23" for x in std.range(1, 24)
    ],
    students: [
      { name: "Иванов И.И.", group: "ИКБО-4-20", age: 19 },
      { name: "Петров П.П.", group: "ИКБО-5-20", age: 18 },
      { name: "Сидоров С.С.", group: "ИКБО-5-20", age: 18 },
      { name: "Жагло И. Д.", group: "ИКБО-10-23", age: 19 }
    ],
    subject: "Конфигурационное управление"
 ```


ЗАДАНИЕ 2. Реализовать на Dhall приведенный ниже пример в формате JSON. Использовать в реализации свойство программируемости и принцип DRY.

РЕШЕНИЕ:
```
 let List/length =
        https://prelude.dhall-lang.org/v21.1.0/List/length

  let groups =
        List/map Natural Text (\(i : Natural) -> "ИКБО-${Natural/show i}-23")
        (List/replicate 24 Natural/successor 0)
  
  let students =
      [ { name = "Иванов И.И.", group = "ИКБО-4-20", age = 19 }
      , { name = "Петров П.П.", group = "ИКБО-5-20", age = 18 }
      , { name = "Сидоров С.С.", group = "ИКБО-5-20", age = 18 }
      , { name = "Жагло И. Д.", group = "ИКБО-10-23", age = 19 }
      ]
  
  in  { groups = groups
      , students = students
      , subject = "Конфигурационное управление"
      }
```


Для решения дальнейших задач потребуется программа на Питоне, представленная ниже. Разбираться в самом языке Питон при этом необязательно.

```python
import random


def parse_bnf(text):
    '''
    Преобразовать текстовую запись БНФ в словарь.
    '''
    grammar = {}
    rules = [line.split('=') for line in text.strip().split('\n')]
    for name, body in rules:
        grammar[name.strip()] = [alt.split() for alt in body.split('|')]
    return grammar


def generate_phrase(grammar, start):
    '''
    Сгенерировать случайную фразу.
    '''
    if start in grammar:
        seq = random.choice(grammar[start])
        return ''.join([generate_phrase(grammar, name) for name in seq])
    return str(start)


BNF = '''
E = a
'''

for i in range(10):
    print(generate_phrase(parse_bnf(BNF), 'E'))
``` 

Реализовать грамматики, описывающие следующие языки (для каждого решения привести БНФ). Код решения должен содержаться в переменной BNF:

ЗАДАНИЕ 3. Язык нулей и единиц.
~~~
10
100
11
101101
000
~~~
РЕШЕНИЕ:
  ~~~
  BNF = '''
  E = 0 | 1 | 0 E | 1 E
  '''
  ~~~


ЗАДАНИЕ 4. Язык правильно расставленных скобок двух видов.
~~~
(({((()))}))
{}
{()}
()
{}
~~~
РЕШЕНИЕ:
  ~~~
  BNF = '''
  E =  | ( E ) | { E } | E E
  '''
  ~~~

ЗАДАНИЕ 5. Язык выражений алгебры логики.
```
((~(y & x)) | (y) & ~x | ~x) & x
y & ~(y)
(~(y) & y & ~y)
~x
~((x) & y | (y) | (x)) & x | x | (y & ~y)
```
  

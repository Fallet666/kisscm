## Задание 1. 
Реализовать на Jsonnet приведенный ниже пример в формате JSON. Использовать в реализации свойство программируемости и принцип DRY.


Решение:
```
local groupPrefix = 'ИКБО-';
local year = '-23';
local groupNum = std.range(1, 10);

local studentData = [
  {name: "Коротков А. А.", age: 52, groupIndex: 3},
  {name: "Жагло И. Д.", age: 20, groupIndex: 2},
  {name: "Запрягаев М. А.", age: 19, groupIndex: 1},
  {name: "Красоткин А. А.", age: 18, groupIndex: 4}
];

{
  groups: [groupPrefix + std.toString(i) + year for i in groupNum],

  students: [
    {
      age: student.age,
      group: groupPrefix + std.toString(student.groupIndex) + year,
      name: student.name
    } for student in studentData
  ],

  subject: "Конфигурационное управление"
}
 ```


## Задание 2. 
Реализовать на Dhall приведенный ниже пример в формате JSON. Использовать в реализации свойство программируемости и принцип DRY.

Решение:
```
let Group = Text
let Student = { age : Natural, group : Group, name : Text }

let createGroup : Natural -> Text =
      λ(n : Natural) → "ИКБО-" ++ Natural/show n ++ "-23"

let groups : List Text =
      [ createGroup 1
      , createGroup 2
      , createGroup 3
      , createGroup 4
      , createGroup 5
      , createGroup 6
      , createGroup 7
      , createGroup 8
      , createGroup 9
      , createGroup 10
      ]

let createStudent : Natural -> Group -> Text -> Student =
      λ(age : Natural) →
      λ(group : Group) →
      λ(name : Text) →
        { age = age, group = group, name = name }

let students : List Student =
  [ createStudent 52 (createGroup 3) "Коротков А. А."
  , createStudent 19 (createGroup 2) "Жагло И. Д."
  , createStudent 22 (createGroup 1) "Запрягаев М. А."
  , createStudent 20 (createGroup 4) "Красоткин А. А."
  ]

in  { groups = groups, students = students, subject = "Конфигурационное управление" }
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

## Задание 3. 
Язык нулей и единиц.
```
10
100
11
101101
000
```
### Решение:

```
  BNF = '''
  E = 0 | 1 | 0 E | 1 E
  '''
```


## Задание 4. 
Язык правильно расставленных скобок двух видов.

(({((()))}))
{}
{()}
()
{}

Решение:
  ~~~
  BNF = '''
  E =  | ( E ) | { E } | E E
  '''
  ~~~

## Задание 5. 
Язык выражений алгебры логики.
```
((~(y & x)) | (y) & ~x | ~x) & x
y & ~(y)
(~(y) & y & ~y)
~x
~((x) & y | (y) | (x)) & x | x | (y & ~y)
```

### Решение:
```
<expression> ::= <term>
               | <open> <term> <operation> <term> <close>
               | <negative> <open> <term> <operation> <term> <close>
               | <open> <expression> <operation> <expression> <close>
               | <negative> <open> <expression> <close>

<term> ::= <variable>
         | <negative> <variable>
         | <open> <variable> <operation> <variable> <close>
         | <negative> <open> <variable> <operation> <variable> <close>

<variable> ::= x | y | z | w

<operation> ::= & | |

<negative> ::= ~

<open> ::= (

<close> ::= )
```
  

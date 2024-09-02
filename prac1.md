## Задача 1
```bash
cut -d: -f1 /etc/passwd | sort
```

## Задача 2
```bash
cat /etc/protocols | tail -n 5 | sort -nrk2 | awk '{print $2, $1}'
```


## Задача 3

```bash
#!/bin/bash

msg="$1"
border="+$(printf '%0.s-' $(seq ${#msg} + 2))+"

echo "$border"
echo "| $msg |"
echo "$border"
```
## Задача 4

```bash
grep -o '\b[a-zA-Z_][a-zA-Z0-9_]*\b' main.cpp | sort | uniq
```

## Задача 5

```bash
#!/bin/bash

chmod +x "$1"
sudo cp "$1" /usr/local/bin/

```

## Задача 6

```bash
#!/bin/bash

for file in "$@"; do
  if [[ "$file" =~ \.(c|js|py)$ ]]; then
    first_line=$(head -n 1 "$file")
    if [[ "$first_line" =~ ^# ]] || [[ "$first_line" =~ ^// ]]; then
      echo "$file has a comment in the first line."
    else
      echo "$file does not have a comment in the first line."
    fi
  fi
done

```

## Задача 7

```bash
find "$1" -type f -exec md5sum {} + | sort | uniq -w32 -dD
```

## Задача 8

```bash
find . -name "*.$1" -print0 | tar -czvf archive.tar.gz --null -T -
```

## Задача 9

```bash
#!/bin/bash

sed 's/    /\t/g' "$1" > "$2"
```

## Задача 10

```bash
find "$1" -type f -empty -name "*.txt"
```

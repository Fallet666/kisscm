import shutil
import zipfile
import argparse
import os

import yaml

class VShell:
    def __init__(self, config_path):
        self.config = self.load_config(config_path)
        self.currentpath = ""
        self.config = self.load_config(config_path)
        self.user = self.config['user']
        self.host = self.config['host']
        self.start_script = self.config['startup_script']
        self.filesystem = zipfile.ZipFile(self.config['filesystem'])
        self.filesystemlist = self.filesystem.filelist


    def load_config(self, config_path):
        with open(config_path, 'r') as file:
            return yaml.safe_load(file)
    def start(self):
        while True:
            cmd = input(f'{self.user}:{self.currentpath}/$ ').split(" ")
            if cmd[0].lower() == "ls":
                self.ls(cmd[1] if len(cmd) > 1 else "")
            elif cmd[0].lower() == "cd":
                self.cd(cmd[1] if len(cmd) > 1 else "")
            elif cmd[0].lower() == "rev":
                self.rev(cmd[1] if len(cmd) > 1 else "")
            elif cmd[0].lower() == "du":
                self.du(cmd[1] if len(cmd) > 1 else "")
            elif cmd[0].lower() == "mv":
                if len(cmd) >= 3:
                    self.mv(cmd[1], cmd[2])
                else:
                    print("Usage: mv <source> <destination>")
            elif cmd[0].lower() == "clear" or cmd[0].lower() == "cls":
                self.clear()
            elif cmd[0].lower() == 'exit':
                break
            else:
                print('Unknown command.')

    def ls(self, newpath: str):
        path = self.currentpath
        if "/root" in newpath:
            path = newpath.replace('/root/', '')
        elif newpath in ("~", "/"):
            path = ""
        else:
            path += "/" + newpath
        if path != "":
            path = path.lstrip("/")

        # Проверка существования директории
        flag = any(path in file.filename for file in self.filesystemlist)
        if not flag:
            print(f'Directory "{newpath}" does not exist.')
            return

        displayed_files = set()  # Множество для отслеживания уникальных имен
        for file in self.filesystemlist:
            if path in file.filename:
                files = file.filename[len(path):].split("/")
                files = list(filter(None, files))
                if len(files) > 1 or not files:
                    continue
                if files[0] not in displayed_files:
                    print(files[0])
                    displayed_files.add(files[0])  # Добавляем файл или каталог в множество


    def cd(self, newpath: str):
        if "/root" in newpath:
            newpath = newpath.replace('/root/', '')
            if any(newpath in file.filename for file in self.filesystemlist):
                self.currentpath = "/" + newpath
        elif newpath == "..":
            self.currentpath = '/'.join(self.currentpath.split('/')[:-1])
            return True
        elif newpath in ("~", "/"):
            self.currentpath = ""
            return True
        elif newpath:
            if any(newpath in file.filename for file in self.filesystemlist):
                self.currentpath += "/" + newpath
                return True
        return False

    def clear(self):
        os.system('cls||clear')

    def rev(self, filename: str):
        # Объединяем имя файла, если оно содержит пробелы
        filename = ' '.join(filename.split())

        # Формируем полный путь к файлу
        file_path = self.currentpath + "/" + filename if not filename.startswith("/") else filename
        file_path = file_path.lstrip("/")

        # Проверяем, существует ли файл в файловой системе
        if not any(file.filename == file_path for file in self.filesystemlist):
            print(f"Error while opening {filename}. File not found.")
            return

        try:
            # Открываем файл из zip-архива
            with self.filesystem.open(file_path, 'r') as f:
                # Пытаемся определить тип файла
                if file_path.endswith('.txt'):
                    # Читаем как текст
                    lines = [x.decode('utf8').strip() for x in f.readlines()]
                    for line in lines[::-1]:
                        print(line)
                else:
                    # Если файл не текстовый
                    print(f"Error: '{filename}' is not a text file and cannot be reversed.")
        except UnicodeDecodeError:
            print(f"Error: '{filename}' is not a text file and cannot be read as UTF-8.")
        except KeyError:
            print(f"Error while opening {filename}.")
        except Exception as e:
            print(f"An unexpected error occurred: {e}")


    def du(self, path: str):
        path = self.currentpath + "/" + path if path else self.currentpath
        path = path.lstrip("/")
        total_size = 0
        for file in self.filesystemlist:
            if file.filename.startswith(path):
                total_size += file.file_size
        print(f"Size: {total_size} bytes")

    # def mv(self, source: str, destination: str):
    def mv(self, source: str, destination: str):
        source_path = self.currentpath + "/" + source if not source.startswith("/") else source
        source_path = source_path.lstrip("/")

        destination_path = self.currentpath + "/" + destination if not destination.startswith("/") else destination
        destination_path = destination_path.lstrip("/")

        # Проверяем, существует ли файл
        if not any(file.filename == source_path for file in self.filesystemlist):
            print(f"Source file '{source}' not found.")
            return

        # Перемещаем файл
        try:
            # Извлекаем содержимое файла
            with self.filesystem.open(source_path, 'r') as source_file:
                file_data = source_file.read()

            # Создаём временный zip-файл, чтобы переместить данные
            temp_zip_path = 'temp_fs.zip'
            with zipfile.ZipFile(temp_zip_path, 'w') as temp_zip:
                # Копируем все файлы, кроме перемещаемого
                for file in self.filesystemlist:
                    if file.filename != source_path:
                        temp_zip.writestr(file.filename, self.filesystem.read(file.filename))

                # Добавляем файл с новым путем
                temp_zip.writestr(destination_path + "/" + os.path.basename(source_path), file_data)

            # Заменяем старый архив новым
            shutil.move(temp_zip_path, self.config['filesystem'])

            # Обновляем файловую систему
            self.filesystem = zipfile.ZipFile(self.config['filesystem'])
            self.filesystemlist = self.filesystem.filelist

            print(f"Moved '{source}' to '{destination}'")

        except KeyError:
            print(f"Error: file '{source}' could not be found or moved.")
        except Exception as e:
            print(f"An unexpected error occurred: {e}")



    def remove_file(self, filename):
        with zipfile.ZipFile(self.config['filesystem'], 'r') as zip_file:
            file_data = {f.filename: f for f in zip_file.filelist}
            file_data.pop(filename, None)
        with zipfile.ZipFile(self.config['filesystem'], 'w') as zip_file:
            for file in file_data.values():
                zip_file.writestr(file.filename, zip_file.read(file.filename))

    def run_script(self, script_file: str):
        try:
            with open(script_file, 'r') as file:
                for line in file:
                    self.execute_command(line.strip())
        except FileNotFoundError:
            print(f"Script file '{script_file}' not found.")

    def execute_command(self, command: str):
        cmd = command.split(" ")
        if cmd[0].lower() == "ls":
            self.ls(cmd[1] if len(cmd) > 1 else "")
        elif cmd[0].lower() == "cd":
            self.cd(cmd[1] if len(cmd) > 1 else "")
        elif cmd[0].lower() == "rev":
            self.rev(cmd[1] if len(cmd) > 1 else "")
        elif cmd[0].lower() == "du":
            self.du(cmd[1] if len(cmd) > 1 else "")
        elif cmd[0].lower() == "mv":
            if len(cmd) >= 3:
                self.mv(cmd[1], cmd[2])
            else:
                print("Usage: mv <source> <destination>")
        elif cmd[0].lower() == "clear" or cmd[0].lower() == "cls":
            self.clear()
        elif cmd[0].lower() == 'exit':
            exit()
        else:
            print('Unknown command.')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="VShell")
    parser.add_argument('filesystem_archive', help="Path to the filesystem archive.")
    parser.add_argument('--script', help="Path to a script file containing commands.")

    args = parser.parse_args()

    vshell = VShell(args.filesystem_archive)

    if args.script:
        vshell.run_script(args.script)
    else:
        vshell.start()

import os
import argparse
import xml.etree.ElementTree as ET
from collections import defaultdict
import requests

class MavenDependencyVisualizer:
    def __init__(self, package_name, output_path, repo_url=None):
        self.package_name = package_name
        self.output_path = output_path
        self.repo_url = repo_url or "https://repo1.maven.org/maven2"
        self.dependencies = defaultdict(list)
        self.processed_artifacts = set()  # Чтобы избежать повторной обработки зависимостей

    def download_pom(self, url):
        """Скачиваем файл pom.xml по указанному URL."""
        response = requests.get(url)
        if response.status_code != 200:
            raise Exception(f"Ошибка загрузки файла pom.xml с URL: {url}")
        return response.text

    def get_package_pom_url(self, package_name):
        """Формируем URL для загрузки pom.xml из Maven Central по имени пакета."""
        group_id, artifact_id, version = package_name.split(":")
        url = f"{self.repo_url}/{group_id.replace('.', '/')}/{artifact_id}/{version}/{artifact_id}-{version}.pom"
        return url

    def parse_pom(self, pom_content):
        """Парсим содержимое pom.xml и извлекаем зависимости вместе со свойствами."""
        root = ET.fromstring(pom_content)
        ns = {'mvn': 'http://maven.apache.org/POM/4.0.0'}  # Пространство имен Maven

        # Извлекаем свойства
        properties = {}
        for prop in root.findall('.//mvn:properties/*', ns):
            properties[prop.tag.split('}')[1]] = prop.text

        dependencies = root.findall('.//mvn:dependency', ns)
        parsed_deps = []

        for dependency in dependencies:
            group_id = dependency.find('mvn:groupId', ns).text
            artifact_id = dependency.find('mvn:artifactId', ns).text
            version_elem = dependency.find('mvn:version', ns)
            version = version_elem.text if version_elem is not None else "unknown"

            # Если версия представлена как переменная (${...}), ищем её в properties
            if version.startswith('${') and version.endswith('}'):
                version_key = version[2:-1]  # Удаляем ${ и }
                version = properties.get(version_key, "unknown")

            print(f"Обрабатываем зависимость: {group_id}:{artifact_id}, версия: {version}")
            parsed_deps.append((group_id, artifact_id, version))

        return parsed_deps

    def process_dependencies(self):
        """Обрабатываем зависимости, включая транзитивные."""
        url = self.get_package_pom_url(self.package_name)
        print(f"Загружаем pom.xml по URL: {url}")
        pom_content = self.download_pom(url)

        dependencies = self.parse_pom(pom_content)
        for group_id, artifact_id, version in dependencies:
            dep_key = f"{group_id}:{artifact_id}:{version}"

            if version != "unknown" and dep_key not in self.processed_artifacts:
                self.dependencies[group_id].append((artifact_id, version))
                self.processed_artifacts.add(dep_key)

                # Попробуем найти транзитивные зависимости
                transitive_url = f"{self.repo_url}/{group_id.replace('.', '/')}/{artifact_id}/{version}/{artifact_id}-{version}.pom"
                try:
                    transitive_pom = self.download_pom(transitive_url)
                    transitive_deps = self.parse_pom(transitive_pom)
                    for t_group_id, t_artifact_id, t_version in transitive_deps:
                        if t_version != "unknown":
                            t_dep_key = f"{t_group_id}:{t_artifact_id}:{t_version}"
                            if t_dep_key not in self.processed_artifacts:
                                self.dependencies[group_id].append((t_artifact_id, t_version))
                                self.processed_artifacts.add(t_dep_key)
                except Exception as e:
                    print(f"Не удалось загрузить транзитивные зависимости для {dep_key}: {e}")

    def generate_mermaid_code(self):
        """Генерируем Mermaid-код для визуализации зависимостей."""
        mermaid_lines = ["%%{init: {'theme': 'base', 'themeVariables': {}, 'flowchart': {'rankSpacing': 250, 'nodeSpacing': 100}} }%%","graph TD;"]
        arts = []
        for group, deps in self.dependencies.items():
            for dep in deps:
                artifact, version = dep
                arts.append([artifact, version])
        for group, deps in self.dependencies.items():
            for dep in deps:
                artifact, version = dep
                if version != "unknown":
                    artifact = artifact.replace(" ", "")
                    artifact = artifact.replace("\n", "")
                    version = version.replace(" ", "")
                    version = version.replace("\n", "")
                    tmp = str(group).replace("org.", "")
                    tmp = tmp.replace("com.", "")
                    tmp = tmp.replace("io.", "")
                    for i in arts:
                        tmp_art, tmp_ver = i
                        if tmp_art == tmp:
                            tmp = tmp + ":" + tmp_ver
                    if tmp!=artifact+":"+version:
                        mermaid_lines.append(f'    {tmp} --> {artifact}:{version};')
        return "\n".join(mermaid_lines)

    def save_to_file(self, code):
        """Сохраняем сгенерированный код в указанный файл."""
        with open(self.output_path, 'w') as f:
            f.write(code)

    def run(self):
        """Запуск основной логики программы."""
        print(f"Парсинг пакета {self.package_name}...")
        self.process_dependencies()

        print("Генерация Mermaid-графа...")
        mermaid_code = self.generate_mermaid_code()

        print(f"Сохранение кода в {self.output_path}...")
        self.save_to_file(mermaid_code)

        print("Готово!")


def main():
    parser = argparse.ArgumentParser(description="Визуализатор зависимостей Maven-пакета.")
    parser.add_argument('--package-name', required=True, help="Имя пакета в формате groupId:artifactId:version.")
    parser.add_argument('--output-path', required=True, help="Путь к файлу для записи результата.")
    parser.add_argument('--repo-url', help="URL-адрес Maven-репозитория для загрузки транзитивных зависимостей.", default="https://repo1.maven.org/maven2")

    args = parser.parse_args()

    visualizer = MavenDependencyVisualizer(args.package_name, args.output_path, args.repo_url)
    visualizer.run()


if __name__ == "__main__":
    main()

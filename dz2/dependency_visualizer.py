import os
import argparse
import xml.etree.ElementTree as ET
from collections import defaultdict
import requests

class MavenDependencyVisualizer:
    def __init__(self, package_path, output_path, repo_url=None):
        self.package_path = package_path
        self.output_path = output_path
        self.repo_url = repo_url
        self.dependencies = defaultdict(list)
        self.processed_artifacts = set()  # Чтобы избежать повторной обработки зависимостей

    def download_pom(self, url):
        """Скачиваем файл pom.xml по указанному URL."""
        response = requests.get(url)
        if response.status_code != 200:
            raise Exception(f"Ошибка загрузки файла pom.xml с URL: {url}")
        return response.text

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
            version = dependency.find('mvn:version', ns).text if dependency.find('mvn:version', ns) is not None else "unknown"

            # Если версия представлена как переменная (${...}), ищем её в properties
            if version.startswith('${') and version.endswith('}'):
                version_key = version[2:-1]  # Удаляем ${ и }
                version = properties.get(version_key, "unknown")

            parsed_deps.append((group_id, artifact_id, version))

        return parsed_deps


    def process_dependencies(self, pom_file, current_group=None):
        """Обрабатываем зависимости, включая транзитивные."""
        with open(pom_file, 'r') as file:
            pom_content = file.read()

        dependencies = self.parse_pom(pom_content)
        for group_id, artifact_id, version in dependencies:
            dep_key = f"{group_id}:{artifact_id}:{version}"

            # Проверяем, обрабатывали ли мы эту зависимость ранее
            if dep_key not in self.processed_artifacts:
                self.dependencies[current_group].append((artifact_id, version))
                self.processed_artifacts.add(dep_key)

                # Попробуем найти транзитивные зависимости
                if self.repo_url:
                    # Загружаем pom.xml транзитивных зависимостей по URL
                    transitive_url = f"{self.repo_url}/{group_id.replace('.', '/')}/{artifact_id}/{version}/{artifact_id}-{version}.pom"
                    try:
                        transitive_pom = self.download_pom(transitive_url)
                        transitive_deps = self.parse_pom(transitive_pom)
                        for t_group_id, t_artifact_id, t_version in transitive_deps:
                            t_dep_key = f"{t_group_id}.{t_artifact_id}:{t_version}"
                            if t_dep_key not in self.processed_artifacts:
                                self.dependencies[group_id].append((t_artifact_id, t_version))
                                self.processed_artifacts.add(t_dep_key)
                    except Exception as e:
                        print(f"Не удалось загрузить транзитивные зависимости для {dep_key}: {e}")

    def generate_mermaid_code(self):
        """Генерируем Mermaid-код для визуализации зависимостей."""
        mermaid_lines = ["graph TD;"]
        for group, deps in self.dependencies.items():
            for dep in deps:
                artifact, version = dep
                mermaid_lines.append(f'    {group} --> {artifact}:{version};')

        return "\n".join(mermaid_lines)

    def save_to_file(self, code):
        """Сохраняем сгенерированный код в указанный файл."""
        with open(self.output_path, 'w') as f:
            f.write(code)

    def run(self):
        """Запуск основной логики программы."""
        pom_file = os.path.join(self.package_path, 'pom.xml')
        if not os.path.exists(pom_file):
            raise FileNotFoundError(f"Файл pom.xml не найден по пути: {pom_file}")

        print(f"Парсинг файла {pom_file}...")
        self.process_dependencies(pom_file)

        print("Генерация Mermaid-графа...")
        mermaid_code = self.generate_mermaid_code()

        print(f"Сохранение кода в {self.output_path}...")
        self.save_to_file(mermaid_code)

        print("Готово!")


def main():
    parser = argparse.ArgumentParser(description="Визуализатор зависимостей Maven-пакета.")
    parser.add_argument('--package-path', required=True, help="Путь к анализируемому Maven-пакету.")
    parser.add_argument('--output-path', required=True, help="Путь к файлу для записи результата.")
    parser.add_argument('--repo-url', help="URL-адрес Maven-репозитория для загрузки транзитивных зависимостей.")

    args = parser.parse_args()

    visualizer = MavenDependencyVisualizer(args.package_path, args.output_path, args.repo_url)
    visualizer.run()


if __name__ == "__main__":
    main()

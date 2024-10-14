import unittest
from unittest.mock import patch, mock_open, MagicMock
from dependency_visualizer import MavenDependencyVisualizer
import requests

class TestMavenDependencyVisualizer(unittest.TestCase):

    @patch('requests.get')
    def test_download_pom(self, mock_get):
        """Тестируем загрузку POM-файла."""
        mock_response = MagicMock()
        mock_response.status_code = 200
        mock_response.text = "<project></project>"
        mock_get.return_value = mock_response

        visualizer = MavenDependencyVisualizer("org.example:example-artifact:1.0", "output.mmd")
        pom_content = visualizer.download_pom("https://repo1.maven.org/maven2/org/example/example-artifact/1.0/example-artifact-1.0.pom")

        mock_get.assert_called_once_with("https://repo1.maven.org/maven2/org/example/example-artifact/1.0/example-artifact-1.0.pom")
        self.assertEqual(pom_content, "<project></project>")

    def test_get_package_pom_url(self):
        """Тестируем правильность формирования URL для загрузки POM."""
        visualizer = MavenDependencyVisualizer("org.example:example-artifact:1.0", "output.mmd")
        expected_url = "https://repo1.maven.org/maven2/org/example/example-artifact/1.0/example-artifact-1.0.pom"
        actual_url = visualizer.get_package_pom_url("org.example:example-artifact:1.0")

        self.assertEqual(actual_url, expected_url)

    @patch('requests.get')
    def test_parse_pom(self, mock_get):
        """Тестируем парсинг POM-файла и извлечение зависимостей."""
        pom_content = '''<project xmlns="http://maven.apache.org/POM/4.0.0">
            <dependencies>
                <dependency>
                    <groupId>org.example</groupId>
                    <artifactId>example-dependency</artifactId>
                    <version>1.0</version>
                </dependency>
            </dependencies>
        </project>'''

        mock_response = MagicMock()
        mock_response.status_code = 200
        mock_response.text = pom_content
        mock_get.return_value = mock_response

        visualizer = MavenDependencyVisualizer("org.example:example-artifact:1.0", "output.mmd")
        dependencies = visualizer.parse_pom(pom_content)

        expected_dependencies = [('org.example', 'example-dependency', '1.0')]
        self.assertEqual(dependencies, expected_dependencies)

    @patch('requests.get')
    def test_process_dependencies(self, mock_get):
        """Тестируем обработку зависимостей, включая транзитивные."""
        # Основной POM
        main_pom_content = '''<project xmlns="http://maven.apache.org/POM/4.0.0">
            <dependencies>
                <dependency>
                    <groupId>org.example</groupId>
                    <artifactId>example-dependency</artifactId>
                    <version>1.0</version>
                </dependency>
            </dependencies>
        </project>'''

        # Транзитивный POM
        transitive_pom_content = '''<project xmlns="http://maven.apache.org/POM/4.0.0">
            <dependencies>
                <dependency>
                    <groupId>org.transitive</groupId>
                    <artifactId>transitive-dependency</artifactId>
                    <version>1.1</version>
                </dependency>
            </dependencies>
        </project>'''

        mock_response = MagicMock()
        mock_response.status_code = 200
        mock_response.text = main_pom_content
        mock_get.side_effect = [mock_response, MagicMock(status_code=200, text=transitive_pom_content)]

        visualizer = MavenDependencyVisualizer("org.example:example-artifact:1.0", "output.mmd")
        visualizer.process_dependencies()

        expected_dependencies = {
            'org.example': [('example-dependency', '1.0')],
            'org.transitive': [('transitive-dependency', '1.1')]
        }
        self.assertEqual(visualizer.dependencies, expected_dependencies)

    def test_generate_mermaid_code(self):
        """Тестируем генерацию кода Mermaid."""
        visualizer = MavenDependencyVisualizer("org.example:example-artifact:1.0", "output.mmd")
        visualizer.dependencies = {
            'org.example': [('example-dependency', '1.0')],
            'org.transitive': [('transitive-dependency', '1.1')]
        }

        expected_mermaid_code = (
            "%%{init: {'theme': 'base', 'themeVariables': {}, 'flowchart': {'rankSpacing': 250, 'nodeSpacing': 100}} }%%\n"
            "graph TD;\n"
            "    example-dependency:1.0 --> transitive-dependency:1.1;"
        )
        actual_mermaid_code = visualizer.generate_mermaid_code()

        self.assertIn("example-dependency:1.0 --> transitive-dependency:1.1;", actual_mermaid_code)

if __name__ == '__main__':
    unittest.main()

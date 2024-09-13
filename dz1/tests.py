import io
import sys
import unittest
import os
import zipfile
import yaml
import emulator as vcl


class TestShellEmulator(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        config_data = {
            'user': 'test_user',
            'host': 'test_host',
            'filesystem': 'virtual_fs_test.zip',
            'startup_script': 'startup_test.sh'
        }
        with open('config_test.yaml', 'w') as file:
            yaml.dump(config_data, file)

    @classmethod
    def tearDownClass(cls):
        os.remove('config_test.yaml')
        os.remove('virtual_fs_test.zip')

    @classmethod
    def setUp(self):
        with zipfile.ZipFile('virtual_fs_test.zip', 'w') as zf:
            zf.writestr('test_directory/file1.txt', 'Test file 1 content')
            zf.writestr('test_directory/file2.txt', 'Test file 2 content')
            zf.writestr('test_directory/subdir/file3.txt', 'Test file 3 content')

    def test_load_config(self):
        self.setUp()
        vc = vcl.VCL('config_test.yaml')

        config = vc.load_config('config_test.yaml')
        self.assertEqual(config['user'], 'test_user')
        self.assertEqual(config['host'], 'test_host')
        self.assertEqual(config['filesystem'], 'virtual_fs_test.zip')
        self.assertEqual(config['startup_script'], 'startup_test.sh')

    # LS
    def test_ls_command(self):
        self.setUp()
        vc = vcl.VCL('config_test.yaml')
        vc.currentpath = 'test_directory'

        captured_output = io.StringIO()
        sys.stdout = captured_output
        vc.ls("")
        sys.stdout = sys.__stdout__

        output = captured_output.getvalue().strip().split('\n')
        expected_output = ['file1.txt', 'file2.txt']
        self.assertEqual(output, expected_output)

    def test_ls_subdirectory(self):
        self.setUp()
        vc = vcl.VCL('config_test.yaml')
        vc.currentpath = 'test_directory/subdir'

        captured_output = io.StringIO()
        sys.stdout = captured_output
        vc.ls("")
        sys.stdout = sys.__stdout__

        output = captured_output.getvalue().strip().split('\n')
        expected_output = ['file3.txt']
        self.assertEqual(output, expected_output)

    # CD
    def test_cd_to_subdirectory(self):
        self.setUp()
        vc = vcl.VCL('config_test.yaml')
        vc.currentpath = 'test_directory'
        vc.cd('subdir')
        self.assertEqual(vc.currentpath, 'test_directory/subdir')

    def test_cd_back_to_parent(self):
        self.setUp()
        vc = vcl.VCL('config_test.yaml')
        vc.currentpath = 'test_directory/subdir'
        vc.cd('..')
        self.assertEqual(vc.currentpath, 'test_directory')

    # REV
    def test_rev_text_file(self):
        self.setUp()
        vc = vcl.VCL('config_test.yaml')
        vc.currentpath = 'test_directory'

        captured_output = io.StringIO()
        sys.stdout = captured_output
        vc.rev('file1.txt')
        sys.stdout = sys.__stdout__

        output = captured_output.getvalue().strip()
        expected_output = "Test file 1 content"[::-1]
        self.assertEqual(output, expected_output)

    def test_rev_nonexistent_file(self):
        self.setUp()
        vc = vcl.VCL('config_test.yaml')
        vc.currentpath = 'test_directory'

        captured_output = io.StringIO()
        sys.stdout = captured_output
        vc.rev('nonexistent.txt')
        sys.stdout = sys.__stdout__

        output = captured_output.getvalue().strip()
        self.assertIn('File not found', output)

    # DU
    def test_du_directory(self):
        self.setUp()
        vc = vcl.VCL('config_test.yaml')
        vc.currentpath = 'test_directory'

        captured_output = io.StringIO()
        sys.stdout = captured_output
        vc.du("")
        sys.stdout = sys.__stdout__

        output = captured_output.getvalue().strip()
        self.assertIn('Size:', output)

    def test_du_subdirectory(self):
        self.setUp()
        vc = vcl.VCL('config_test.yaml')
        vc.currentpath = 'test_directory'

        captured_output = io.StringIO()
        sys.stdout = captured_output
        vc.du('subdir')
        sys.stdout = sys.__stdout__

        output = captured_output.getvalue().strip()
        self.assertIn('Size:', output)

    # MV
    def test_mv_file(self):
        self.setUp()
        vc = vcl.VCL('config_test.yaml')
        vc.currentpath = 'test_directory'
        vc.mv('file1.txt', 'subdir')

        self.assertIn('test_directory/subdir/file1.txt', [f.filename for f in vc.filesystemlist])

    def test_mv_nonexistent_file(self):
        self.setUp()
        vc = vcl.VCL('config_test.yaml')
        vc.currentpath = 'test_directory'

        captured_output = io.StringIO()
        sys.stdout = captured_output
        vc.mv('nonexistent.txt', 'file2.txt')
        sys.stdout = sys.__stdout__

        output = captured_output.getvalue().strip()
        self.assertIn('Source file', output)


if __name__ == '__main__':
    unittest.main()

from glob import glob
from os.path import abspath, dirname, join, basename
import re
from subprocess import check_output
import unittest as ut


class EmitterTest(ut.TestCase):
    IS_WHITESPACE_RE = re.compile('^\s*$')
    INPUT_RE = re.compile('^\s*;\s*INPUT\s*:\s*(.+?)\s*$')

    def test_emitter(self):
        testdir = abspath(dirname(__file__))
        for input_file in glob(join(testdir, 'EmitterTest/*.ll')):
            test_name = basename(input_file).rsplit('.', 1)[0]
            with self.subTest(test_name):
                self.emitter_test_file(input_file)

    def emitter_test_file(self, input_file):
        with open(input_file, 'r') as fp:
            command = None

            for line in fp.readlines():
                if self.IS_WHITESPACE_RE.match(line):
                    continue
                elif self.INPUT_RE.match(line):
                    line = self.INPUT_RE.match(line)
                    command = line.groups(0)[0]
                    print(command)
                elif not command:
                    self.fail(input_file + ' did not specify a test command')
                else:
                    actual = check_output(command, shell=True)

        with open(input_file, 'r') as fp:
            expect = fp.readlines()

        actual = actual.decode('utf8')
        actual = actual.split('\n')

        i = 0  # expect
        j = 0  # actual

        while i < len(expect) and j < len(actual):
            expect_line = expect[i].strip()
            actual_line = actual[j].strip()

            if not expect_line or expect_line.startswith(';'):
                i += 1
                continue

            if not actual_line or actual_line.startswith(';'):
                j += 1
                continue

            expect_line = re.split(r'(\/\/.*?\/\/)', expect_line)

            for k in range(len(expect_line)):
                if expect_line[k].startswith('//'):
                    expect_line[k] = expect_line[k].strip('/')
                else:
                    expect_line[k] = re.escape(expect_line[k])

            self.assertTrue(
                re.match(''.join(expect_line), actual_line),
                '{} line {}'.format(input_file, i + 1))
            i += 1
            j += 1


if __name__ == '__main__':
    ut.main()
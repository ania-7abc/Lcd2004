#/ BESTF (Platformio part)

from platformio.public import TestCase, TestCaseSource, TestStatus, TestRunnerBase
import time
import click
class CustomTestRunner(TestRunnerBase):
    def setup(self):
        self.intro_count = 0
    def on_testing_line_output(self, line):
        if self.intro_count < 4:
            click.echo(line, nl = False)
            self.intro_count += 1
        else:
            cmd = line[0]
            args = line[1:-1].split(';')
            if cmd == 'B':
                self.test_suite.on_start()
            elif cmd == 'E':
                self.test_suite.on_finish()
            elif cmd == 'S':
                self.test_name = args[0]
                self.test_file = args[1]
                self.test_timestamp = time.time()
            elif cmd in ('T', 'F'):
                self.test_suite.add_case(TestCase(
                    self.test_name,
                    TestStatus.PASSED if cmd == 'T' else TestStatus.FAILED,
                    None if cmd == 'T' else args[0],
                    f"Test '{self.test_name}' at {self.test_file}:{args[-1]} {"passed" if cmd == 'T' else f"failed: {args[0]}"}",
                    TestCaseSource(self.test_file, args[-1]),
                    time.time() - self.test_timestamp
                ))
            elif cmd == 'D':
                click.echo(f"From test '{self.test_name}' at {self.test_file}:{args[-1]}: {args[:-1]}")



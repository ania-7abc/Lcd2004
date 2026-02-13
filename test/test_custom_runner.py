#/ BESTF (Platformio part)

from platformio.public import TestCase, TestCaseSource, TestStatus, TestRunnerBase
from platformio.device.finder import SerialPortFinder
from platformio.exception import UserSideException
from platformio.test.runners.readers.serial import SerialTestOutputReader
import serial
import time
import click
class SerialManager(SerialTestOutputReader):
    def __init__(self, test_runner):
        self.test_runner = test_runner
        self.ser = None
    def begin(self):
        click.echo(
            "If you don't see any output for the first 10 secs, "
            "please reset board (press reset button)"
        )
        click.echo()
        try:
            self.ser = serial.serial_for_url(
                self.resolve_test_port(),
                baudrate=self.test_runner.get_test_speed(),
                timeout=600,
            )
        except serial.SerialException as exc:
            click.secho(str(exc), fg="red", err=True)
            return
        while not self.test_runner.test_suite.is_finished():
            if self.ser.in_waiting:
                data = self.ser.read(self.ser.in_waiting)
                self.test_runner.on_testing_data_output(data)
        self.ser.close()
        self.ser = None
    def send(self, data):
        if self.ser is None or not self.ser.is_open:
            raise serial.SerialException("Serial port is not open")
        if isinstance(data, str):
            data = data.encode('utf-8')
        elif not isinstance(data, bytes):
            raise TypeError("Data must be bytes or str")
        try:
            bytes_sent = self.ser.write(data)
            self.ser.flush()
            return bytes_sent
        except serial.SerialException as exc:
            click.secho(str(exc), fg="red", err=True)
            raise
class CustomTestRunner(TestRunnerBase):
    def setup(self):
        self.intro_count = 0
        self.ser_mngr = SerialManager(self)
    def stage_testing(self):
        if self.options.without_testing:
            return None
        click.secho("Testing...", bold=True)
        return self.ser_mngr.begin()
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
                click.secho(f"Running '{self.test_name}' from {self.test_file}...", bold=True, fg="cyan")
            elif cmd in ('T', 'F'):
                self.test_suite.add_case(TestCase(
                    self.test_name,
                    TestStatus.PASSED if cmd == 'T' else TestStatus.FAILED,
                    None if cmd == 'T' else args[0],
                    f"Test '{self.test_name}' in {self.test_file}:{args[-1]} {"\033[1;32mpassed" if cmd == 'T' else f"\033[1;31mfailed: {args[0]}"}\033[0m",
                    TestCaseSource(self.test_file, args[-1]),
                    time.time() - self.test_timestamp
                ))
            elif cmd == 'D':
                cmd = args[0]
                args = args[1:]
                line = args[-1]
                if (cmd == 'T'):
                    click.secho(f"From test '{self.test_name}' in {self.test_file}:{line}: {args[0]}", fg='yellow')
                elif (cmd == 'C'):
                    cont = click.confirm(click.style(f"Continue running the '{self.test_name}' test in {self.test_file}:{line}?", fg='green', bold=True), default=True)
                    self.ser_mngr.send("y" if cont else "n")
                elif cmd == 'S':
                    self.test_suite.add_case(TestCase(
                        self.test_name,
                        TestStatus.SKIPPED,
                        None,
                        f"Test '{self.test_name}' in {self.test_file}:{args[-1]} \033[1;36mskipped\033[0m",
                        TestCaseSource(self.test_file, args[-1]),
                        time.time() - self.test_timestamp
                    ))

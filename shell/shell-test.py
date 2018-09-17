import os
import re
import unittest
from signal import SIGINT, SIGTSTP

from shell.proto.shell_info_pb2 import ShellInfo
from shell.proto.job_pb2 import Job
from subprocess import Popen, PIPE


class ShellTest(unittest.TestCase):
    def test_success_commands(self):
        with open('shell/success_commands.sh', 'r') as command_file:
            proc = Popen(['shell/shell-exec'], stdin=PIPE, stdout=PIPE, stderr=PIPE)
            out, err =  proc.communicate(command_file.read())

            self.assertEqual(err.find('Error'), -1)

            shell_info_data = out[out.find('SHELL|')+6:out.rfind('|SHELL')]

            shell_info = ShellInfo()
            shell_info.ParseFromString(shell_info_data)

            for job in shell_info.job:
                print(job)
                self.assertEqual(job.status, Job.UNDEFINED)
            self.assertEqual(proc.returncode, 0)

    def test_failure_commands(self):
        with open('shell/failure_commands.sh', 'r') as command_file:
            for line in command_file.readlines():
                proc = Popen(['shell/shell-exec'], stdin=PIPE, stdout=PIPE, stderr=PIPE)
                out, err = proc.communicate(line)

                self.assertNotEqual(err.find('Error'), -1)


if __name__ == "__main__":
    unittest.main()

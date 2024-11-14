#!/usr/bin/env python

# Copyright JS Foundation and other contributors, http://js.foundation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
import subprocess
import sys
import time

import util
TempFile = __import__("test262-harness").TempFile # pylint: disable=invalid-name

class DebuggerArgs:
    def __init__(self):
        self.jerry = sys.argv[1]
        self.channel = sys.argv[2]
        self.debugger_client = sys.argv[3]
        self.test_case = sys.argv[4]


def check_output(command_args, stdin=None, encoding=None):
    try:
        out = subprocess.check_output(command_args, stdin=stdin, shell=False, stderr=subprocess.STDOUT)
    except subprocess.CalledProcessError as check_error:
        out = check_error.output
    return out.decode(encoding or 'utf-8', 'ignore')


def execute_debug_client(out_tmp, cmd_file_name, debug_client_args):
    print(f'input debug cmd: {cmd_file_name}')
    with open(cmd_file_name, 'rb') as cmd_file:
        out = check_output(debug_client_args, cmd_file)
        out_tmp.write(out)


def main(args):
    util.setup_stdio()
    jerry_debug_server_cmd = [args.jerry]
    client_args = []
    if 'client_source' in args.test_case:
        jerry_debug_server_cmd += ['--start-debug-server', '--debug-channel',
                                   args.channel, '--debugger-wait-source']
        client_args += ['--client-source']
        if 'client_source_multiple' in args.test_case:
            client_args += [args.test_case + '_2.js', args.test_case + '_1.js']
        else:
            client_args += [args.test_case + '.js']
    else:
        jerry_debug_server_cmd += [args.test_case + '.js', '--start-debug-server', '--debug-channel', args.channel]
    print(f'run debug server: {jerry_debug_server_cmd}')
    with subprocess.Popen(jerry_debug_server_cmd, stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT) as proc:
        time.sleep(1)

        out_tmp = TempFile(prefix=os.path.basename(args.test_case), suffix='out')
        git_failed = False
        try:
            debug_client_args = util.get_python_cmd_prefix()
            debug_client_args += [args.debugger_client, '--channel', args.channel, '--non-interactive']
            debug_client_args += client_args
            print(f"run debug client: {' '.join(debug_client_args)}")
            execute_debug_client(out_tmp, args.test_case + '.cmd', debug_client_args)
            if 'restart' in args.test_case:
                continue_case = args.test_case.replace('restart', 'continue')
                execute_debug_client(out_tmp, continue_case + '.cmd', debug_client_args)
            out_tmp.close()
            git_diff_cmd = ['git', '--no-pager', 'diff', '--ignore-space-at-eol',
                            '--no-index', args.test_case + '.expected', out_tmp.name]
            git_out = check_output(git_diff_cmd)
            if '@@' in git_out:
                git_failed = True
        finally:
            proc.wait()
            print(f"jerry out:\n{proc.stdout.read().decode('utf-8')}\nEOF")
            print(f"git diff cmd: {' '.join(git_diff_cmd)}")
            if git_failed:
                print(f'git diff result:\n{git_out}\nEOF')
                print(f'{util.TERM_RED}FAIL: {args.test_case}{util.TERM_NORMAL}')
                sys.exit(1)
            else:
                out_tmp.dispose()
                print(f'{util.TERM_GREEN}PASS: {args.test_case}{util.TERM_NORMAL}')
                sys.exit(0)


if __name__ == "__main__":
    sys.exit(main(DebuggerArgs()))

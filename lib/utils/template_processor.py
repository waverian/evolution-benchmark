#
# Copyright (c) 2022 waverian. All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
#                                                             * "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

import re
import subprocess
import sys

FILE_TEMPLATE = '''
  {print_lines}
'''


class PrintLinesManager:
    def __init__(self):
        self.return_type = "void"
        self.keep_whitespaces = False
        self.include = []
        self.param_name = None
        self.param_type = None
        self.name = None
        self.print_lines = ""
        self.alias = []

    def find_next_template_tag(self, input_string: str, search_tag: str = ''):
        # Regular expression to match <!--ANYTHING--> and capture ANYTHING
        match = re.search(fr'<!--%{search_tag}(.*?)-->', input_string)
        if match is None:
            return None, input_string, ''

        before_comment = input_string[:match.start()]
        after_comment = input_string[match.end():]

        comment_content = match.group(1)
        result_dict = {}

        pairs = comment_content.split(';')
        for pair in pairs:
            try:
                key, value = pair.split("=", 1)
                result_dict[key.strip()] = value.strip()
            except ValueError:
                result_dict[pair] = None

        return result_dict, before_comment, after_comment

    def generate_printf(self, input_str: str):
        # if string consists only of whitespaces - skip it
        if re.match(r'^[\s\n]*$', input_str):
            return

        if self.keep_whitespaces:
            input_str = re.sub(r'\n\s*$', '', input_str)
            # input_str = re.sub(r'^\s*\n', '', input_str)
            # input_str = re.sub(r'>\s*$', '>', input_str)
            # input_str = re.sub(r'^\s*\n', '', input_str)
            input_str = input_str.replace("\n", "\\n")
        else:
            input_str.replace("\n", "")
            input_str = re.sub(r'>\s*<', '><', input_str)
            input_str = re.sub(r'>\s*$', '>', input_str)
            input_str = re.sub(r'^\s*<', '<', input_str)
            input_str = re.sub(r'\s+<', '<', input_str)

        input_str = input_str.replace("%", "%%")
        input_str = input_str.replace("\"", "\\\"")
        input_str = input_str.replace("\'", "\\\'")

        if not self.keep_whitespaces:
            input_str = " ".join(input_str.split())
        if input_str == "":
            return
        self.print_lines += f"fprintf(file_handler,\"{input_str}\");"

    def generate_file(self, file_path):
        includes = "".join(["#include <" + include + ">\n" for include in self.include])
        with open(file_path, "w") as file:
            file.writelines(FILE_TEMPLATE.format(return_type=self.return_type, include=includes, name=self.name,
                                                 parameter=self.param_type,
                                                 param_name=self.param_name,
                                                 print_lines=self.print_lines,
                                                 alias=";\n".join(self.alias) + ";\n"))

        subprocess.run(["clang-format", "-i", file_path], check=True)

    def process_file(self, file_path: str):
        with open(file_path, 'r') as file:
            html = file.read()
            self.loop(html)

    def loop(self, text: str):
        arguments, processed_text, remain_text = self.find_next_template_tag(text)
        if arguments:
            if not "WPP_IGNORE" in arguments.keys():
                self.generate_printf(processed_text)
            if "WPP_KEEP_WHITESPACES" in arguments.keys():
                self.keep_whitespaces = True
            if "WPP_ALIAS" in arguments.keys():
                self.alias.append(arguments['WPP_ALIAS'])
            if "WPP_FOR" in arguments.keys():
                self.print_lines += f"for({arguments['WPP_FOR']} = {arguments['WPP_FOR_BEGIN']}; {arguments['WPP_FOR']} < {arguments['WPP_FOR_END']}; {arguments['WPP_FOR']}++){{"
                _, loop_body, remain_text = self.find_next_template_tag(remain_text,
                                                                        f"WPP_ENDFOR={arguments['WPP_FOR']}")
                self.loop(loop_body)
                self.print_lines += "}"
            if "WPP_STR_PARAM" in arguments.keys():
                self.print_lines += f"fprintf(file_handler, \"%s\", {arguments['WPP_STR_PARAM']});"
            if "WPP_INT_PARAM" in arguments.keys():
                self.print_lines += f"fprintf(file_handler, \"%i\", {arguments['WPP_INT_PARAM']});"
            if "WPP_IF" in arguments.keys():
                self.print_lines += f"if({arguments['WPP_IF']}){{"
                _, if_body, remain_text = self.find_next_template_tag(remain_text, "WPP_ENDIF")

                self.loop(if_body)
                self.print_lines += "}"
            if "WPP_ELSE" in arguments.keys():
                self.print_lines += "}else{"
            if "WPP_NUM_PARAM" in arguments.keys():
                length = ""
                after_dot = "0"
                if "LENGTH" in arguments.keys():
                    length = arguments["LENGTH"]
                if "AFTER_DOT" in arguments.keys():
                    after_dot = arguments["AFTER_DOT"]
                if after_dot == "AUTO":
                    self.print_lines += f"fprintf(file_handler, \"%{length}.*f\", AUTO_PRECISION({arguments['WPP_NUM_PARAM']}));"
                else:
                    self.print_lines += f"fprintf(file_handler, \"%{length}.{after_dot}f\", {arguments['WPP_NUM_PARAM']});"
            if "WPP_NAME" in arguments.keys():
                self.name = arguments["WPP_NAME"]
            if "WPP_RETURN_TYPE" in arguments.keys():
                self.return_type = arguments["WPP_RETURN_TYPE"]
            if "WPP_PARAM_TYPE" in arguments.keys():
                self.param_type = arguments["WPP_PARAM_TYPE"]
            if "WPP_PARAM_NAME" in arguments.keys():
                self.param_name = arguments["WPP_PARAM_NAME"]
            if "WPP_INCLUDE" in arguments.keys():
                self.include.append(arguments["WPP_INCLUDE"])
            if remain_text:
                self.loop(remain_text)
        else:
            self.generate_printf(processed_text)


if __name__ == "__main__":
    print_lines_manager = PrintLinesManager()
    print_lines_manager.process_file(sys.argv[1])
    print_lines_manager.generate_file(sys.argv[2])

#!/usr/bin/env python3

################################################################################
# Copyright (c) 2010-2019 Robert Bosch GmbH
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#   Robert Bosch GmbH - initial contribution
#
################################################################################

import argparse
import os.path
import sys
import glob
import datetime
import xml.dom.minidom
import xml.sax.saxutils
import logging

module = sys.modules['__main__'].__file__
log = logging.getLogger(module)

NUM_CONTEXT_LINES = 5

if sys.version_info[0] * 10 + sys.version_info[1] < 36:
    raise Exception("Must be using Python >=3.6")

def find_replacement_context(replacement:xml.dom.Node, source_file:str = None) -> (str, int):
    if not source_file:
        return replacement.toxml(), 0

    offset = int(replacement.attributes['offset'].nodeValue)
    length = int(replacement.attributes['length'].nodeValue)
    cur_offset = 0
    context = ""
    line_num = 0
    with open(source_file, 'r') as fs:
        lines = fs.readlines()
        for i, line in enumerate(lines):
            if cur_offset + len(line) >= offset:
                pre_context_lines = lines[i-NUM_CONTEXT_LINES:i]
                post_context_lines = lines[i+1:i+NUM_CONTEXT_LINES]
                linnum_width = len(str(len(post_context_lines) + i + 1))

                num_pre_lines = len(pre_context_lines)
                pre_context_lines += [line]
                context_lines_w_numbers = list(map(lambda e: f"%{linnum_width}d:%s" % (e[0], e[1]), enumerate(pre_context_lines, start=i-num_pre_lines+1)))
                fix_line = '❌'
                fix_line += '·' * (offset - cur_offset + linnum_width)
                fix_line += '˄' * length
                fix_line += '\n'
                context_lines_w_numbers += [fix_line]
                context_lines_w_numbers += list(map(lambda e: f"%{linnum_width}d:%s" % (e[0], e[1]), enumerate(post_context_lines, start=i+2)))
                context += ''.join(context_lines_w_numbers)
                line_num = i
                break
            cur_offset += len(line)

    return context, line_num


def find_source_file(report:str, prefix:str, suffix:str, file_types:tuple = tuple()) -> (str, str):
    prefix = os.path.abspath(prefix)
    _source_file = os.path.abspath(report)[len(prefix) + 1:]
    _source_file = os.path.join(os.path.dirname(_source_file), os.path.basename(report)[:-len(suffix)])
    matches = []
    if file_types:
        for file_type in file_types:
            matches += glob.glob(_source_file + file_type)
    else:
        matches += [_source_file]
    return (matches[0], None) if len(matches) > 0 else (None, _source_file)

def main() -> int:
    logging.basicConfig(level=logging.WARNING, format='%(levelname)s: %(message)s')

    parser = argparse.ArgumentParser()
    parser.add_argument('report_files', metavar='FILES', type=str, nargs='+', help="list of xml files containing the clang-format report")
    parser.add_argument('-p', '--prefix', default='build', type=str, nargs='?', help="prefix to be removed from FILES to find the equivalent source file")
    parser.add_argument('-s', '--suffix', default='_format.xml', type=str, nargs='?', help="suffix to be removed from FILES to find the equivalent source file")
    parser.add_argument('-o', '--output', metavar='OUTPUT', type=str, default='clang-format-to-junit.xml', help="path to the output file in which to write the resulting junit xml")
    parser.add_argument('-t', '--types', metavar='TYPE', type=str, nargs='+', help="list of file type extensions that should be added to the globbing expression when searching for the respective source file")
    parser.add_argument("-v", "--verbose", dest="verbose_count", action="count", default=0, help="increases log verbosity for each occurrence (e.g -v -vv).")
    args = parser.parse_args()

    # Sets log level to WARNING going more verbose for each new -v.
    log.setLevel(max(3 - args.verbose_count, 0) * 10)

    missing_files = list(filter(lambda p: not os.path.exists(p), args.report_files))
    if len(missing_files) > 0:
        log.error(f"Missing input files: {','.join(missing_files)}")
        return 1

    with xml.dom.minidom.getDOMImplementation().createDocument(None, 'testsuites', None) as output_dom:
        testsuites = output_dom.documentElement
        testsuites.setAttribute('id', 'clang-format-to-junit-' + datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f'))
        testsuites.setAttribute('name', "clang-format checks")
        num_tests = 0
        num_total_failures = 0

        for report in args.report_files:
            logging.info(f"Parsing \"{report}\"")
            source_file, best_guess = find_source_file(report, args.prefix, args.suffix)
            if not source_file:
                logging.warn(f"Could not determine source file location for \"{report}\" (best guess \"{best_guess}\").")
            elif not os.path.exists(source_file):
                logging.warn(f"Could not find source file for \"{report}\" (guessed path \"{source_file}\").")
            elif not os.path.isfile(source_file):
                logging.warn(f"Determined source path for \"{report}\", does not point to a file (\"{source_file}\").")

            testsuite = output_dom.createElement('testsuite')
            testsuites.appendChild(testsuite)
            testsuite.setAttribute('id', 'clang-format')
            testsuite.setAttribute('name', source_file if source_file else "unable_to_determine_source_" + report)

            testcase_counter = 0
            testcase = output_dom.createElement('testcase')
            testsuite.appendChild(testcase)
            testcase.setAttribute('name', 'Formatter scanned through file')

            if os.stat(report).st_size != 0:
                # If the original source file is empty clang-format will output
                # an empty xml as well.
                with xml.dom.minidom.parse(report) as input_dom:
                    replacements = input_dom.getElementsByTagName('replacements')[0].getElementsByTagName('replacement')
                    if replacements:
                        warn_msg = f"Replacements found in file \"{report}\", on line(s): "
                        linenums = []
                        for replacement in replacements:
                            testcase_counter += 1
                            context, linenum = find_replacement_context(replacement, source_file)
                            linenums += [linenum]
                            testcase = output_dom.createElement('testcase')
                            testsuite.appendChild(testcase)
                            testcase.setAttribute('name', 'format_check')
                            failure = output_dom.createElement('failure')
                            testcase.appendChild(failure)
                            failure.setAttribute('type', "format_violation")
                            failure.appendChild(output_dom.createTextNode(xml.sax.saxutils.escape(context)))
                        warn_msg += ', '.join(str(x) for x in linenums)
                        log.warn(warn_msg)

        with open(args.output, 'wb') as fs:
            encoding = 'UTF-8'
            prettyxml = output_dom.toprettyxml('  ', '\n', encoding)
            fs.write(prettyxml)
            log.debug(f"XML output:\n{prettyxml.decode(encoding)}")

if __name__ == '__main__':
    sys.exit(main())

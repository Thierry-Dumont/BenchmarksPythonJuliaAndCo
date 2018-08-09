#!/usr/bin/env python

import sys

###############################################################################
class HTMLTable:
    def header(self):
        return "<table>\n"

    def footer(self):
        return "</table>\n"

    def begin_row(self):
        return "<tr>"

    def end_row(self):
        return "</tr>\n"

    def _cell(self, tag, content='', colspan=1):
        if colspan > 1:
            return "<{0} colspan='{1}'>{2}</{0}>".format(tag, colspan, content)
        else:
            return "<{0}>{1}</{0}>".format(tag, content)

    def th(self, content='', colspan=1):
        return self._cell('th', content, colspan)

    def td(self, content='', colspan=1):
        return self._cell('td', content, colspan)


###############################################################################
class MarkdownTable:
    def __init__(self):
        self.first_row = True
        self.col_cnt = 0

    def header(self):
        return ""

    def footer(self):
        return "\n"

    def begin_row(self):
        return "|"

    def end_row(self):
        if self.first_row:
            self.first_row = False
            return "\n|" + "-|" * self.col_cnt + "\n"
        else:
            return "\n"

    def _cell(self, content='', colspan=1):
        if self.first_row:
            self.col_cnt += colspan

        return " {} |".format(content) + " |" * (colspan-1)

    def th(self, content='', colspan=1):
        if content:
            return self._cell('**{}**'.format(content), colspan)
        else:
            return self._cell('', colspan)

    def td(self, content='', colspan=1):
        return self._cell(content, colspan)


###############################################################################
def read_results(input_file, comp_filter, test_filter):

    with open(input_file, 'r') as fh:

        # Reading file
        compilers = fh.readline().split()[1:]
        results = dict()
        test_id_list = list() # Test order

        for line in fh:
            fields = line.split()
            results[int(fields[0])] = [float(v) for v in fields[1:]]
            test_id_list.append(int(fields[0]))

        import re

        # Test id list
        if args.test:
            test_id_list = list()
            for id_or_range in test_filter.split(','):
                range_id = re.match('(\d+)-(\d+)', id_or_range)
                if range_id:
                    test_id_list.extend(range(int(range_id.group(1)), int(range_id.group(2))+1))
                else:
                    test_id_list.append(int(id_or_range))

        # Index of filtered compilers
        compilers_id = [i for i,comp_name in enumerate(compilers) if re.match(comp_filter, comp_name)]

        # Returning filtered results
        return (
            [compilers[i] for i in compilers_id],
            [(test_id, [results[test_id][i] for i in compilers_id]) for test_id in test_id_list]
        )


###############################################################################
def format_table(compilers, results, table_gen, full_comp=False, write=sys.stdout.write):
    write(table_gen.header())

    # Compiler name and version
    if full_comp:
        write(table_gen.begin_row())
        write(table_gen.th())
        for comp in compilers:
            write(table_gen.th(comp))
        write(table_gen.end_row())

    else:
        write(table_gen.begin_row())
        write(table_gen.th())

        prev_comp_name = compilers[0].split('/')[0]
        colspan = 0
        for comp in compilers:
            comp_name = comp.split('/')[0]
            if comp_name == prev_comp_name:
                colspan += 1
            else:
                write(table_gen.th(prev_comp_name, colspan))
                colspan = 1
                prev_comp_name = comp_name

        write(table_gen.th(prev_comp_name, colspan))
        write(table_gen.end_row())

        write(table_gen.begin_row())
        write(table_gen.th())
        for comp in compilers:
            write(table_gen.th(comp.split('/')[1]))
        write(table_gen.end_row())

    # Results
    for test_id, values in results:
        write(table_gen.begin_row())
        write(table_gen.th(test_id))
        for v in values:
            write(table_gen.td(v))

        write(table_gen.end_row())

    write(table_gen.footer())


###############################################################################
if __name__ == '__main__':

    # Command-line arguments
    import argparse
    parser = argparse.ArgumentParser(description='Bench results to table')
    parser.add_argument('input', metavar='INPUT', help='Bench results')
    parser.add_argument('format', metavar='FORMAT', help='Output format (html or markdown)')
    parser.add_argument('--filter', '-f', default='', help='Filter the compiler name/version by the given regexp')
    parser.add_argument('--test', '-t', default='', help='Comma separated list of test id or range')
    parser.add_argument('--full_comp', '-C', default=False, action='store_true', help='Display compiler full name & version')
    args = parser.parse_args()

    # Reading results
    compilers, results = read_results(args.input, args.filter, args.test)

    # Displaying table
    if args.format.lower() == "html":
        table_gen = HTMLTable()
    else:
        table_gen = MarkdownTable()

    format_table(compilers, results, table_gen, full_comp=args.full_comp)

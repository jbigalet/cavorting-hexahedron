#!/usr/bin/env python2.7

import os
import itertools


def _pseudo_partitions(n):
    if n <= 0:
        return [[]]
    res = []
    for i in range(1, n+1):
        res += [[i] + p for p in _pseudo_partitions(n-i)]
    return res


SBEGIN = '{| '
SEND =   ' |}'
PBEGIN = '{# '
PEND =   ' #}'

INF = float('inf')

def outofscope_exec(s):
    _str = ""
    exec(s)
    return _str

def smic_to_cpp(_from, to):
    with open(_from, 'r') as f:
        smic = f.read()

    to_exec = ""
    indent = 0

    while True:
        smic_sbegin = smic.find(SBEGIN)
        smic_pbegin = smic.find(PBEGIN)

        smic_begin = min((sb if sb != -1 else INF) for sb in [smic_sbegin, smic_pbegin])

        if smic_begin == INF:
            to_exec += '\n' + (' '*indent) + '_str += """%s"""' % smic
            break

        if smic_begin == smic_sbegin:
            end_symbol = SEND
            block_match = True
        else:
            end_symbol = PEND
            block_match = False

        to_exec += '\n' + (' '*indent) + '_str += """%s"""' % smic[:smic_begin]

        smic = smic[smic_begin+len(end_symbol):]
        smic_end = smic.find(end_symbol)
        if smic_end == '-1':
            raise RuntimeError('Unmatched smic symbols')  # TODO at line, etc..

        smic_stmt = smic[:smic_end]
        if not block_match:  # 'toprint' match
            smic_stmt = '_str += """%s""" % ' + smic_stmt

        if block_match and smic_stmt == 'end':
            indent -= 4
            if indent < 0:
                raise RuntimeError('Unmatched end symbol')  # TODO at line, etc...

        else:  # 'end' is the only ignored symbol
            to_exec += '\n' + (' '*indent) + smic_stmt
            if block_match and smic_stmt.endswith(':'):
                indent += 4

        smic = smic[smic_end+len(end_symbol):]

    if indent != 0:
        raise RuntimeError('Unmatched indent, an end symbol is missing')  # TODO at line etc


    # if to.endswith('h'):
    #     print to_exec

    out = outofscope_exec(to_exec)
    if not os.path.exists(os.path.dirname(to)):
        os.mkdir(os.path.dirname(to))

    with open(to, 'w') as f:
        f.write(out)


if __name__ == '__main__':
    root = os.path.dirname(os.path.realpath(__file__))
    for r, dirs, files in os.walk(root, topdown=True):
        dirs[:] = [d for d in dirs if d != '.git']  # inplace dirs modifications allowed in topdown traversal
        for f in files:
            tmpname, ext = os.path.splitext(f)
            name, smicext = os.path.splitext(tmpname)
            if smicext == '.smic':
                smic_to_cpp(os.path.join(r, f), os.path.join(r, 'autogen', '%s%s' % (name, ext)))

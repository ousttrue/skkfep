#!/usr/bin/python3
from typing import List
import sys
import pathlib
import re
import struct


PAT=re.compile(r'\\(\d+)')


def to_utf8(vals: List[str]):
    nums = [int(v, 8) for v in vals]
    print(nums)
    byte_array = struct.pack(f'{len(vals)}B', *nums)
    return byte_array.decode('euc-jp')


def main(path: pathlib.Path):
    content = path.read_text()
    for l in content.splitlines():
        m = PAT.findall(l)
        if m:
            print(to_utf8(m))


if __name__ == '__main__':
    main(pathlib.Path(sys.argv[1]))


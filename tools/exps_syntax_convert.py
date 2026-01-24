'''stupid regex code lappy wants to convert new explorerscript syntax to old explorerscript syntax >:('''

import regex
import os
from sys import argv, stderr
from os.path import exists, isdir

twoline = regex.compile(r"(?P<s> *?)([^ \"'\n][^\"'\n]*?)[\"']{3}\n(?P=s)    (.*?)\n(?:(?P=s)    (.*?)\n)(?P=s)[\"']{3}")
threeline = regex.compile(r"(?P<s> *?)([^ \"'\n][^\"'\n]*?)[\"']{3}\n(?P=s)    (.*?)\n(?:(?P=s)    (.*?)\n)(?:(?P=s)    (.*?)\n)(?P=s)[\"']{3}")
def multiline_to_singleline(src: str) -> str:
    return threeline.sub(r'\1\2"\3\\n\4\\n\5"', twoline.sub(r'\1\2"\3\\n\4"', src))

oldlabels = regex.compile(r"( {2,})@([^ ;]*?;)")
def newlabels_to_oldlabels(src: str) -> str:
    return oldlabels.sub(r'\1§\2', src)

inlinewiths = regex.compile(r'( *?)([^` <;{]*?)<([^>,]*?)>(\([^;]*?\);)')
def remove_inline_withs(src: str) -> str:
    return inlinewiths.sub(r'\1with(\3) {\n    \1\2\4\n\1}', src)

def new_to_old(src: str) -> str:
    return newlabels_to_oldlabels(remove_inline_withs(multiline_to_singleline(src)))

def process_single(path: str):
    with open(path, "r") as f:
        src = f.read()
    old_syntax = new_to_old(src)
    with open(path, "w") as f:
        f.write(old_syntax)

def process_folder(path: str):
    for root, _, files in os.walk(path):
        for file in files:
            if file.endswith('.exps'):
                full_path = os.path.join(path, file)
                process_single(full_path)

if __name__ == "__main__":
    if len(argv) <= 1:
        print("Please provide a path to either a file or a directory", file=stderr)
        exit(1)
    path = argv[1]
    if not exists(path):
        print(f"{path} not found.", file=stderr)
        exit(1)
    if isdir(path):
        process_folder(path)
    else:
        process_single(path)

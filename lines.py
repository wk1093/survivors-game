# count lines of .cpp/.h in src/ directory

import os

def count_lines(path):
    total = 0
    for root, dirs, files in os.walk(path):
        for f in files:
            if f.endswith('.cpp') or f.endswith('.h'):
                lines = sum(1 for line in open(os.path.join(root, f)))
                print(lines, f)
                total += lines
    return total

a = count_lines('src')
print('total:', a)
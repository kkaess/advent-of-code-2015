'''
Created on Feb 23, 2021

@author: kaess
'''

import sys

if __name__ == '__main__':
    s = sys.argv[1]
    total = 0
    for c in s:
        if c == '(':
            total += 1
        elif c == ')':
            total -= 1
    print('Final Destination: {}'.format(total))
    floor = 0
    for i,c in enumerate(s):
        if c == '(':
            floor += 1
        elif c == ')':
            floor -= 1
        if floor == -1:
            print('Index of first visit to basement: {}'.format(i + 1))
            break
    pass

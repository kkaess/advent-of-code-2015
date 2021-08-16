'''
Created on Feb 24, 2021

@author: kaess
'''
import sys


if __name__ == '__main__':
    f = open(sys.argv[1])
    wrapping_paper = 0
    ribbon = 0
    for present in f:
        dims = [int(num) for num in present.split('x')]
        dims.sort()
        wrapping_paper += 3 * dims[0] * dims[1] + 2 * (dims[1] * dims[2] + dims[0] * dims[2])
        ribbon += (2 * (dims[0] + dims[1]) + dims[0] * dims[1] * dims[2])
    print('Size of Wrapping Paper: {}'.format(wrapping_paper))
    print('Length of Ribbon: {}'.format(ribbon))
    f.close()
        
    pass

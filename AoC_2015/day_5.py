'''
Created on Mar 1, 2021

@author: kaess
'''
import regex as re

if __name__ == '__main__':
    three_vowels = re.compile('([aeiou][^aeiou]*){2}[aeiou]')
    doubles = re.compile('(.)\\1')
    bad_combos = re.compile('ab|cd|pq|xy')
    
    def is_good(s):
        if three_vowels.search(s) is not None and doubles.search(s) is not None and bad_combos.search(s) is None:
            return 1
        else:
            return 0
        
    double_doubles = re.compile('(.{2}).*\\1')
    double_single = re.compile('(.).\\1')
        
    def new_is_good(s):
        if double_doubles.search(s) is not None and double_single.search(s) is not None:
            return 1
        else:
            return 0
    
    
    
    
    f = open('input_day5_2015.txt')
    strings = f.readlines()
    f.close()
    
    print("Number of good strings: {}".format(sum([is_good(s) for s in strings])))
    
    print("Number of good strings under new rules: {}".format(sum([new_is_good(s) for s in strings])))
    
    pass
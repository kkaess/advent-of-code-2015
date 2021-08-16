'''
Created on Feb 26, 2021

@author: kaess
'''
from hashlib import md5
import sys
if __name__ == '__main__':
    
    value = sys.argv[1]
    
    i = 1
    
    new_value = value + str(i)
    m = md5(bytes(new_value,'utf-8'))
    
    while not m.hexdigest()[:5] == '00000':
        i += 1
        new_value = value + str(i)
        m = md5(bytes(new_value,'utf-8'))
        
    print('Secret key five zeros: {}'.format(i))
    
    while not m.hexdigest()[:6] == '000000':
        i += 1
        new_value = value + str(i)
        m = md5(bytes(new_value,'utf-8'))
    
    print('Secret key six zeros: {}'.format(i))
    pass

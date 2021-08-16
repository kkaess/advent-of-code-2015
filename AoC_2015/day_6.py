'''
Created on Mar 4, 2021

@author: kaess
'''
import regex as re
import numpy as np

if __name__ == '__main__':
    f = open('input_day6_2015.txt')
    strings = f.readlines()
    f.close()
    
    grid = np.zeros((1000,1000),dtype='bool')
    
    def turn_on(vals):
        grid[vals[0]:vals[2]+1,vals[1]:vals[3]+1] = True
    def turn_off(vals):
        grid[vals[0]:vals[2]+1,vals[1]:vals[3]+1] = False
    def toggle(vals):
        grid[vals[0]:vals[2]+1,vals[1]:vals[3]+1] = np.logical_not(grid[vals[0]:vals[2]+1,vals[1]:vals[3]+1])
        
    for s in strings:
        if s.startswith('turn on'):
            vals = [int(x) for x in re.split(' through |turn on | |,|\n',s)[1:5]]
            turn_on(vals)
        elif s.startswith('turn off'):
            vals = [int(x) for x in re.split(' through |turn off | |,|\n',s)[1:5]]
            turn_off(vals)
        elif s.startswith('toggle'):
            vals = [int(x) for x in re.split(' through |toggle | |,|\n',s)[1:5]]
            toggle(vals)
    
    print('Number of lights lit: {}'.format(np.sum(grid))) 
    
    new_grid = np.zeros((1000,1000),dtype='int16')
    
    def new_turn_on(vals):
        shape = [vals[2]+1-vals[0],vals[3]+1-vals[1]]
        new_grid[vals[0]:vals[2]+1,vals[1]:vals[3]+1] += np.ones(shape,dtype='int16')
    def new_turn_off(vals):

        new_grid[vals[0]:vals[2]+1,vals[1]:vals[3]+1] -= 1
        new_grid[vals[0]:vals[2]+1,vals[1]:vals[3]+1][new_grid[vals[0]:vals[2]+1,vals[1]:vals[3]+1]<0] = 0
    def new_toggle(vals):
        shape = [vals[2]+1-vals[0],vals[3]+1-vals[1]]
        new_grid[vals[0]:vals[2]+1,vals[1]:vals[3]+1] += 2*np.ones(shape,dtype='int16')
        
    for s in strings:
        if s.startswith('turn on'):
            vals = [int(x) for x in re.split(' through |turn on | |,|\n',s)[1:5]]
            new_turn_on(vals)
        elif s.startswith('turn off'):
            vals = [int(x) for x in re.split(' through |turn off | |,|\n',s)[1:5]]
            new_turn_off(vals)
        elif s.startswith('toggle'):
            vals = [int(x) for x in re.split(' through |toggle | |,|\n',s)[1:5]]
            new_toggle(vals)
    
    print("Total brightness with correct interpretation: {}".format(np.sum(new_grid)))
    pass
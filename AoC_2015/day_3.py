'''
Created on Feb 25, 2021

@author: kaess
'''
import numpy as np

if __name__ == '__main__':
    
    f = open('input_day3_2015.txt')
    directions = f.readline()
    f.close()
    
    min_x = 0
    min_y = 0
    max_x = 0
    max_y = 0
    x = 0
    y = 0
    
    for direction in directions:
        if direction == '>':
            x += 1
            if x > max_x:
                max_x = x
        elif direction == '<':
            x -= 1
            if x < min_x:
                min_x = x
        elif direction == '^':
            y += 1
            if y > max_y:
                max_y = y
        elif direction == 'v':
            y -= 1
            if y < min_y:
                min_y = y
    
    len_x = max_x - min_x + 1
    len_y = max_y - min_y + 1
    
    grid = np.zeros((len_x,len_y), dtype='int64')
    
    
    x = -min_x
    y = -min_y
    
    grid[x][y] += 1
    
    for direction in directions:
        if direction == '>':
            x += 1
        elif direction == '<':
            x -= 1
        elif direction == '^':
            y += 1
        elif direction == 'v':
            y -= 1
        grid[x][y] += 1
    
    houses = 0
    for i in range(len(grid)):
        for j in range(len(grid[0])):
            if grid[i][j] > 0:
                houses += 1
    
    
    print('Number of houses visited: {}'.format(houses))
    
    santas_d = directions[::2]
    robo_santas_d = directions[1::2]
    
    min_x = 0
    min_y = 0
    max_x = 0
    max_y = 0
    x = 0
    y = 0
    
    for direction in santas_d:
        if direction == '>':
            x += 1
            if x > max_x:
                max_x = x
        elif direction == '<':
            x -= 1
            if x < min_x:
                min_x = x
        elif direction == '^':
            y += 1
            if y > max_y:
                max_y = y
        elif direction == 'v':
            y -= 1
            if y < min_y:
                min_y = y
                
    x = 0
    y = 0
    
    for direction in robo_santas_d:
        if direction == '>':
            x += 1
            if x > max_x:
                max_x = x
        elif direction == '<':
            x -= 1
            if x < min_x:
                min_x = x
        elif direction == '^':
            y += 1
            if y > max_y:
                max_y = y
        elif direction == 'v':
            y -= 1
            if y < min_y:
                min_y = y
    
    len_x = max_x - min_x + 1
    len_y = max_y - min_y + 1
    
    grid = np.zeros((len_x,len_y), dtype='int64')
    
    
    x = -min_x
    y = -min_y
    
    grid[x][y] += 1
    
    for direction in santas_d:
        if direction == '>':
            x += 1
        elif direction == '<':
            x -= 1
        elif direction == '^':
            y += 1
        elif direction == 'v':
            y -= 1
        grid[x][y] += 1
        
    x = -min_x
    y = -min_y
    
    grid[x][y] += 1
    
    for direction in robo_santas_d:
        if direction == '>':
            x += 1
        elif direction == '<':
            x -= 1
        elif direction == '^':
            y += 1
        elif direction == 'v':
            y -= 1
        grid[x][y] += 1
    
    houses = 0
    for i in range(len(grid)):
        for j in range(len(grid[0])):
            if grid[i][j] > 0:
                houses += 1
    
    
    print('Number of houses visited next year: {}'.format(houses))
    
    
    pass

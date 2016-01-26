import random

LONGEST_PATH = 20

maze =  [
        ['*',8,'-',1],
        [4,'*',11,'*'],
        ['+',4,'-',18],
        [22,'-',9,'*'],
        ]

def randomDirection(x,y):
    directions = []
    if x > 0 and (x,y) != (1,3): directions.append('w')
    if x < 3: directions.append('e')
    if y > 0: directions.append('n')
    if y < 3 and (x,y) != (0,2): directions.append('s')
    return random.choice(directions)

def randomPath():
    path = []
    x,y = 0,3
    while (x,y) != (3,0):
        if len(path) > LONGEST_PATH: return []
        op_direction = randomDirection(x,y)
        x,y = takeStep(x,y,op_direction)

        num_direction = randomDirection(x,y)
        x,y = takeStep(x,y,num_direction)

        path.append((op_direction, num_direction))
    return path

def takeStep(x,y,direction):
    if direction == 'n': y -= 1
    elif direction == 'e': x += 1
    elif direction == 's': y += 1
    elif direction == 'w': x -= 1
    else:
        raise Exception("Unknown direction '%s'" % direction)

    return x,y

def evalPath(path):
    x,y,value = 0,3,22
    for op_step, num_step in path:
        x,y = takeStep(x,y,op_step)
        op = maze[y][x]
        x,y = takeStep(x,y,num_step)
        num = maze[y][x]
        if op == '+':
            value += num
        elif op == '-':
            value -= num
        elif op == '*':
            value *= num
        else:
            raise Exception("Unknown op '%s'" % op)

    return value

def validPath():
    value = 0
    while value != 30:
        path = randomPath()
        value = evalPath(path)

    return path

def tryPaths():
    shortest = '.' * 40
    while True:
        path = validPath()
        if len(path) < len(shortest):
            shortest = path
            print len(path), ''.join([d for pair in path for d in pair])

tryPaths()

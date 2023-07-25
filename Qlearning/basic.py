import pygame
import numpy as np
import random
pygame.init()

class Grid:
    def __init__(self,row,column,start_x,start_y,reward_x,reward_y):
        self.row=row
        self.column = column
        self.grid = np.zeros((column,row))
        self.startX = start_x
        self.startY = start_y
        self.posX = start_x
        self.posY = start_y
        self.grid[reward_x,reward_y] = 1
        
    def addQTable(self,table):
        self.qtable = table
    def setBlock(self,x,y):
        self.grid[x,y] = -1
    def reset(self):
        while True:
            self.posX = (int)((random.random()*1000)//1)%self.column
            self.posY = (int)((random.random()*1000)//1)%self.row
            if self.grid[self.posX,self.posY] != -1 and self.grid[self.posX,self.posY] != 1:
                break
        self.qtable.successtimes += 1
        self.qtable.expl = max(1-self.qtable.successtimes/100,0.05)
        print(self.qtable.expl)
    def goLeft(self):
        if self.posX==0:
            return -1
        elif self.grid[self.posX-1,self.posY] == -1 :
            return -1
        elif self.grid[self.posX-1,self.posY] == 1:
            self.reset()
            return 100
        else:
            self.posX-=1
            return -0.1

    def goRight(self):
        if self.posX+1==self.column:
            return -1
        elif self.grid[self.posX+1,self.posY] == -1 :
            return -1
        elif self.grid[self.posX+1,self.posY] == 1:
            self.reset()
            return 100
        else:
            self.posX+=1
            return -0.1

    def goUp(self):
        if self.posY==0:
            return -1
        elif self.grid[self.posX,self.posY-1] == -1:
            return -1
        elif self.grid[self.posX,self.posY-1] == 1:
            self.reset()
            return 100
        else:
            self.posY-=1
            return -0.1

    def goDown(self):
        if self.posY+1==self.row:
            return -1
        if self.grid[self.posX,self.posY+1] == -1:
            return -1
        elif self.grid[self.posX,self.posY+1] == 1:
            self.reset()
            return 100
        else:
            self.posY+=1
            return -0.1


class Action:
    UP = 0
    RIGHT = 1
    DOWN = 2
    LEFT = 3



class Square(pygame.sprite.Sprite):
    def __init__(self,size,color):
        super(Square, self).__init__()
        self.surf = pygame.Surface((size, size))
        self.surf.fill(color)
        self.rect = self.surf.get_rect()
 
class QTable:
    def __init__(self,grid,alpha,gamma):
        self.alpha = alpha
        self.gamma = gamma
        self.table = []
        self.state = []
        self.noOfState = 0
        self.grid = grid
        self.expl = 1
        self.successtimes = 0
    def addState(self,state):
        self.state.append(state)
        self.table.append([0,0,0,0])
        self.noOfState+=1
    def getQPos(self,state):
        for i in range(self.noOfState):
            if(self.state[i][0]==state[0] and self.state[i][1]==state[1]):
                return i
        return -1
    def getQ(self,state,action):
        temp = self.getQPos(state)
        if temp==-1:
            return 0
        else:
            return temp[action]
    def setQ(self,action):
        state = (self.grid.posX,self.grid.posY)
        Qpos = self.getQPos(state)
        if Qpos==-1:
            Qpos = self.noOfState
            self.addState(state)
        
        if action==Action.UP:
            reward = self.grid.goUp()
            nextState = (state[0],state[1]-1)
        elif action==Action.DOWN:
            reward = self.grid.goDown()
            nextState = (state[0],state[1]+1)
        elif action==Action.RIGHT:
            reward = self.grid.goRight()
            nextState = (state[0]+1,state[1])
        elif action==Action.LEFT:
            reward = self.grid.goLeft()
            nextState = (state[0]-1,state[1])

        nextQPos = self.getQPos(nextState)
        if nextQPos==-1:
            maxNext = 0
        else:
            maxNext = max(self.table[nextQPos])
        
        self.table[Qpos][action] += self.alpha*(reward-self.table[Qpos][action]+self.gamma*maxNext)
    def makeDecision(self):
        rand = random.random()
        if rand<self.expl:
            return random.choices([0,1,2,3])[0]
        else:
            temp = self.getQPos((self.grid.posX,self.grid.posY))
            if temp==-1:
                self.addState((self.grid.posX,self.grid.posY))
                return self.table[self.noOfState-1].index(max(self.table[self.noOfState-1]))
            return self.table[temp].index(max(self.table[temp]))

gameOn = True

gridColumn = 9
gridRow = 9

grid = Grid(gridRow,gridColumn,5,0,0,5)
qtable = QTable(grid,0.1,0.9)
grid.addQTable(qtable)


grid.setBlock(3,3)
grid.setBlock(1,5)
grid.setBlock(1,4)
grid.setBlock(1,3)
grid.setBlock(0,1)
grid.setBlock(1,1)
grid.setBlock(2,1)
grid.setBlock(7,6)
grid.setBlock(7,7)
grid.setBlock(7,8)
grid.setBlock(4,3)



maxWidth = 600
maxHeight = 600

if gridColumn>gridRow:
    squareSize = maxWidth//gridColumn
else:
    squareSize = maxHeight//gridRow
screenWidth = squareSize*gridColumn
screenHeight = squareSize*gridRow
pygame.display.set_caption("Q-Learning")
screen = pygame.display.set_mode((screenWidth,screenHeight))


clock = pygame.time.Clock()

while gameOn:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            gameOn = False
        elif event.type == pygame.KEYDOWN:
            if event.key==pygame.K_UP:
                qtable.setQ(Action.UP)
            elif event.key==pygame.K_DOWN:
                qtable.setQ(Action.DOWN)
            elif event.key==pygame.K_LEFT:
                qtable.setQ(Action.LEFT)
            elif event.key==pygame.K_RIGHT:
                qtable.setQ(Action.RIGHT)
    d = qtable.makeDecision()
    qtable.setQ(d)

    screen.fill((0,0,0))



    blocks = np.argwhere(grid.grid==-1)
    for i in blocks:
        screen.blit(Square(squareSize,(0,0,255)).surf,(i[0]*squareSize,i[1]*squareSize))

    
    final = np.argwhere(grid.grid==1)
    for i in final:
        screen.blit(Square(squareSize,(255,255,255)).surf,(i[0]*squareSize,i[1]*squareSize))

    screen.blit(Square(squareSize,(0,255,0)).surf,(grid.posX*squareSize,grid.posY*squareSize))

    pygame.display.flip()
    
    clock.tick(60)

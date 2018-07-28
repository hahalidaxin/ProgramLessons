from Tkinter import *
import random


class SnakeGame:


    def __init__(self):
        # moving step for snake and food
        self.step=15
        # game score
        self.gamescore=-10
        # score per food
        self.scoreperfood = 10
        
        # to initialize the snake in the range of (x1,y1,x2,y1)
        r = random.randrange(191, 191 + 15 * 10, self.step)
        self.snakeX=[r,r+self.step,r+self.step*2]
        self.snakeY=[r,r,r]
        
        # to initialize the moving direction
        self.snakeDirection = 'left'
        self.snakeMove = [-1,0]
        # to draw the game frame 
        window = Tk()
        window.geometry("600x600+10+10")
        window.maxsize(600,400)
        window.minsize(600,400)
        window.title("Snake game")
        self.frame1=Frame(window,height=400,width=600)
        self.frame2=Frame(self.frame1)
        self.canvas=Canvas(self.frame1,width=600,height=400,bg="yellow")
        self.score_label=Label(self.frame2)

        self.frame1.pack(fill=BOTH)
        self.frame2.pack(fill=BOTH)
        self.score_label.pack(side=LEFT)
        self.canvas.pack(fill=BOTH)
        self.draw_wall()
        self.draw_score()
        self.draw_food()
        self.draw_snake()

        self.play()
        window.mainloop()

    "=== View Part ==="
    def draw_wall(self):
        self.canvas.create_line(10,10,582,10,fill='blue',width=5)
        self.canvas.create_line(10,359,582,359,fill='blue',width=5)
        self.canvas.create_line(10,10,10,359,fill='blue',width=5)
        self.canvas.create_line(582,10,582,359,fill='blue',width=5)

    def draw_score(self):
        self.score()                        # score model
        self.score_label.config(self.score_label,text="Score: "+str(self.gamescore))
        
    def draw_food(self):
        self.canvas.delete("food")
        self.foodx,self.foody=self.random_food()    #food model
        self.canvas.create_rectangle(self.foodx,self.foody,self.foodx+self.step,self.foody+self.step,
                                     fill="red" ,tags="food")     #food view

    def draw_snake(self):
        self.canvas.delete("snake")
        x,y=self.snake()                    # snake model
        for i in range(len(x)):             # snake view
            self.canvas.create_rectangle(x[i],y[i],x[i]+self.step,y[i]+self.step\
            , fill="green",tags='snake')

    "=== Model Part ==="
    # food model
    def random_food(self):      
        return random.randrange(11,570,self.step),random.randrange(11,340,self.step)
    
    # snake model
    def snake(self):
        for i in range(len(self.snakeX)-1,0,-1):
            self.snakeX[i] = self.snakeX[i-1]
            self.snakeY[i] = self.snakeY[i-1]
        self.snakeX[0] += self.snakeMove[0]*self.step
        self.snakeY[0] += self.snakeMove[1]*self.step
        return(self.snakeX,self.snakeY)
        
    #score model    
    def score(self):
        self.gamescore = (len(self.snakeX)-3)*self.scoreperfood
        
    
    "=== Control Part ==="     
    def iseated(self):
            if self.snakeX[0]==self.foodx and self.snakeY[0]==self.foody:
                return True
            else:
                return False
    
    def isdead(self):
        if self.snakeX[0]<8 or self.snakeX[0] >580 or\
        self.snakeY[0]<8 or self.snakeY[0]>350 :
            return True
        
        for i in range(1,len(self.snakeX)):
            if self.snakeX[0]==self.snakeX[i] and self.snakeY[0]==self.snakeY[i] :
                return True
        else:
            return False
    
    def move(self,event):
    # left:[-1,0],right:[1,0],up:[0,1],down:[0,-1] 
    
        if (event.keycode == 39 or event.keycode == 68) and self.snakeDirection != 'left':
            self.snakeMove = [1,0]
            self.snakeDirection = "right"
        elif (event.keycode == 38 or event.keycode == 87) and self.snakeDirection != 'down':
            self.snakeMove = [0,-1]
            self.snakeDirection = "up"
        elif (event.keycode == 37 or event.keycode == 65) and self.snakeDirection != 'right':
            self.snakeMove = [-1,0]
            self.snakeDirection = "left"
        elif (event.keycode == 40 or event.keycode == 83) and self.snakeDirection != 'up':
            self.snakeMove = [0,1]
            self.snakeDirection = "down"
        else:
            pass

#       above codes can be insteaded by the following codes 
        
#        if (event.keysym == 'Right' or event.keysym == 'd') and self.snakeDirection != 'left':
#            self.snakeMove = [1,0]
#            self.snakeDirection = "right"
#        elif (event.keysym == 'Up' or event.keysym == 'w') and self.snakeDirection != 'down':
#            self.snakeMove = [0,-1]
#            self.snakeDirection = "up"
#        elif (event.keysym == 'Left' or event.keysym == 'a') and self.snakeDirection != 'right':
#            self.snakeMove = [-1,0]
#            self.snakeDirection = "left"
#        elif (event.keysym == 'Down' or event.keysym == 's') and self.snakeDirection != 'up':
#            self.snakeMove = [0,1]
#            self.snakeDirection = "down"
#        else:
#            pass
             
    def play(self):
        self.canvas.bind('<Key>',self.move)
        self.canvas.focus_set()

        while True:
            if self.isdead():
                self.gameover()
                break
            elif self.iseated():
                self.snakeX[0] += self.snakeMove[0]*self.step
                self.snakeY[0] += self.snakeMove[1]*self.step
                self.snakeX.insert(1,self.foodx)
                self.snakeY.insert(1,self.foody)

                self.draw_score()
                self.draw_food()
                self.draw_snake()
            else:
                self.draw_snake() 
                self.canvas.after(200)
                self.canvas.update()
        
    def gameover(self):
        # self.canvas.delete("food","snake")
        self.canvas.unbind('<Key>')
        self.canvas.bind("<Key>",self.restart)
        self.canvas.create_text(270,180,text="                   Game Over!\n \
        Press any key to continue",font='Helvetica -30 bold',tags='text')

    def restart(self,event):
        self.canvas.delete("food","snake","text")
        self.canvas.unbind('<Key>')

        # to initialize the snake in the range of (191,191,341,341)                
        r=random.randrange(191,191+15*10,self.step)
        self.snakeX=[r,r+self.step,r+self.step*2]
        self.snakeY=[r,r,r]
        
        # to initialize the moving direction
        self.snakeDirection = 'left'
        self.snakeMove = [-1,0]
        
        # reset the score to zero 
        self.gamescore=-10
        self.draw_score() 
        
        # to initialize the game (food and snake)
        self.draw_food()
        self.draw_snake()
        
        # to play the game
        self.play()

SnakeGame()
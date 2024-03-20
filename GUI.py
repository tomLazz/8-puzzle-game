import pygame 

from Gameboard import Gameboard
from Gameboard import solve_with

class GameGUI:
    def __init__(self):
        pygame.init()
        self.screen = pygame.display.set_mode((450,650))
        pygame.display.set_caption('8 puzzle')
        self.clock = pygame.time.Clock()
        self.playing_game = False
        self.all_sprites = pygame.sprite.Group()
        self.tiles = Gameboard()

    def draw_board(self):

        self.screen.fill((255, 255, 255))
        self.all_sprites.draw(self.screen)


        for x in range(4):
            pygame.draw.line(self.screen, (100,100,100),(x*100 + 75, 50),(x*100 + 75, 350), 3)
            #75 < x < 375
            #75, 175, 275, 375 

        for y in range(4):
            pygame.draw.line(self.screen, (100,100,100),(75, y*100 + 50),(375, y*100 + 50), 3)
            #50 < y < 350
            #50, 150, 250, 350

        for x in range(2):
            pygame.draw.line(self.screen, (100,100,100),(125 + 200 * x, 400),(125 + 200 * x, 450), 3)



        for y in range(2):
            pygame.draw.line(self.screen, (100,100,100),(125, 400 + 50*y),(325, 400+ 50*y), 3)
                


        font1 = pygame.font.Font('freesansbold.ttf',24)
        text1 = font1.render("Solve with CNN", True, (40,55,50), (255,255,255))
        textRect1 = text1.get_rect()
        textRect1.center = (225, 425)
        self.screen.blit(text1, textRect1)

        pygame.display.flip()


    def run(self):
        self.draw_board()
        self.playing_game = True
        
        while self.playing_game:
            self.update_board()
            self.events()

    def get_move(self, mouseX, mouseY):

        origX,origY = -1,-1
        
        for x in range(3):
            for y in range(3):
                if 75 + 100*x < mouseX < 75 + 100*(x+1) and 50 + 100*y < mouseY < 50 + 100*(y+1):
                    origX = x
                    origY = y

        return origX, origY
    
    def set_tile_to(self, x_pos, y_pos):
        
        while True:
            for event in pygame.event.get():
                if event.type == pygame.KEYDOWN:
                    keystroke = chr(event.key)
                    if not self.tiles.valid_entry(keystroke):
                        return

                    self.tiles.gameboard[y_pos][x_pos] = keystroke
                    return
                elif event.type == pygame.MOUSEBUTTONDOWN:
                    mouseX, mouseY = pygame.mouse.get_pos()

                    if 75 < mouseX < 375 and 50 < mouseY < 350:
                        x, y = self.get_move(mouseX, mouseY)
                        self.set_tile_to(x, y)
                    return
                elif event.type == pygame.QUIT:
                    pygame.quit()
                    quit(0)

    def events(self):
        for event in pygame.event.get():    
            if event.type == pygame.QUIT:
                pygame.quit()
                quit(0)

            if event.type == pygame.MOUSEBUTTONDOWN:
                mouseX, mouseY = pygame.mouse.get_pos()
                if 125 < mouseX < 325 and 400 < mouseY < 450:
                    moves = solve_with(self.tiles)
                    if self.animate_moves(moves) != None:
                        print("not solvable")
                elif 75 < mouseX < 375 and 50 < mouseY < 350:
                    x, y = self.get_move(mouseX, mouseY)
                    self.set_tile_to(x, y)

    def animate_moves(self, moves):
        if moves == None:
            return None
        time = 300
        for move in moves:
            pygame.time.wait(time)
            self.tiles.switch(move)
            self.update_board()


    def update_board(self):
        font = pygame.font.Font('freesansbold.ttf',32)

        for x in range(3): 
            for y in range(3):
                if self.tiles.gameboard[y][x] == 0:
                    text = font.render("   ", True, (255,255,255), (255,255,255))
                else:
                    text = font.render(str(self.tiles.gameboard[y][x]), True, (155,100,100), (255,255,255))


                textRect = text.get_rect()
                textRect.center = (x * 100 + 125, y*100 + 100)
                self.screen.blit(text, textRect)

        pygame.display.flip()

    
board = GameGUI()
board.draw_board()
board.run()


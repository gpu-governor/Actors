import pygame
import sys
from scripts.Actors import *
# GLOBAL VAR
SCREEN_WIDTH, SCREEN_HEIGHT = 1080, 720
GAME_CAPTION = "GAME"
# INIT PYGAME
pygame.init()
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption(GAME_CAPTION)
clock = pygame.time.Clock()

player = AnimatedActor(["Data/img/player/Idle/0.png", "Data/img/player/Idle/1.png", "Data/img/player/Idle/2.png", "Data/img/player/Idle/3.png"],100, 100,2.0,3)

# EVENT LOOP
while True:
    player.render(screen,50)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_UP:
                pass          
            if event.key == pygame.K_DOWN:
                pass

    pygame.display.update()
    clock.tick(60)
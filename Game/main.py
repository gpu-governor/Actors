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

player_idle =["Data/img/player/Idle/0.png", "Data/img/player/Idle/1.png", "Data/img/player/Idle/2.png", "Data/img/player/Idle/3.png", "Data/img/player/Idle/4.png"]
player_walk = ["Data/img/player/Run/0.png","Data/img/player/Run/1.png","Data/img/player/Run/2.png","Data/img/player/Run/3.png","Data/img/player/Run/4.png","Data/img/player/Run/5.png"]
player = AnimatedActor(player_idle,300, 300, 2.0, 0.1)
# EVENT LOOP
while True:
    delta_time = clock.tick(60) / 1000.0  # Time in seconds
    screen.fill((128,128,128))
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_RIGHT:
                player.file_path = player_walk
                player.x = player.x+2       
            if event.key == pygame.K_LEFT:
                player.x = player.x-2

    player.render(screen,delta_time)
    pygame.display.update()
    clock.tick(60)
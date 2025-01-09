import pygame
import sys
# GLOBAL VAR
SCREEN_WIDTH, SCREEN_HEIGHT = 1080, 720
GAME_CAPTION = "GAME"
# INIT PYGAME
pygame.init()
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption(GAME_CAPTION)
clock = pygame.time.Clock()

# EVENT LOOP
while True:
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
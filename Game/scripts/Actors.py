import pygame
import time

class Actor:
    def __init__(self, file_path, xpos, ypos, scale_value):
        self.image = pygame.image.load(file_path).convert_alpha()  # Load and handle transparency
        self.x = xpos
        self.y = ypos
        self.scale = scale_value

        # Scale the image
        self.image = pygame.transform.scale(
            self.image,
            (int(self.image.get_width() * self.scale), int(self.image.get_height() * self.scale))
        )

    def render(self, screen):
        screen.blit(self.image, (self.x, self.y))

    # this function is to adjust the parameters of actors, for instance after an actor is initialize and you want to modify the img to show maybe after a key press
    def adjust(self, file_path, xpos, ypos, scale_value):
        # Load the new image
        self.image = pygame.image.load(file_path).convert_alpha()  # Handle transparency
        
        # Update position and scale
        self.x = xpos
        self.y = ypos
        self.scale = scale_value

        # Scale the image
        self.image = pygame.transform.scale(
            self.image,
            (int(self.image.get_width() * self.scale), int(self.image.get_height() * self.scale))
        )


class AnimatedActor(Actor):
    def __init__(self, file_paths, xpos, ypos, scale_value, frame_duration):
        super().__init__(file_paths[0], xpos, ypos, scale_value)  # Load the first frame
        self.frames = [pygame.image.load(path).convert_alpha() for path in file_paths]
        self.frames = [
            pygame.transform.scale(frame, (int(frame.get_width() * self.scale), int(frame.get_height() * self.scale)))
            for frame in self.frames
        ]
        self.frame_duration = frame_duration
        self.current_frame = 0
        self.elapsed_time = 0.0

    def render(self, screen, delta_time):
        # Update frame based on elapsed time
        self.elapsed_time += delta_time
        if self.elapsed_time >= self.frame_duration:
            self.current_frame = (self.current_frame + 1) % len(self.frames)
            self.elapsed_time = 0.0

        # Render current frame
        screen.blit(self.frames[self.current_frame], (self.x, self.y))

    def adjust(self, file_paths, xpos, ypos, scale_value, frame_duration):
        # Update position and scale
        self.x = xpos
        self.y = ypos
        self.scale = scale_value
        self.frame_duration = frame_duration

        # Reload frames
        self.frames = [pygame.image.load(path).convert_alpha() for path in file_paths]
        self.frames = [
            pygame.transform.scale(frame, (int(frame.get_width() * self.scale), int(frame.get_height() * self.scale)))
            for frame in self.frames
        ]

        # Reset animation state
        self.current_frame = 0
        self.elapsed_time = 0.0


class SpriteSheetActor(Actor):
    def __init__(self, file_path, xpos, ypos, scale_value, tile_width, tile_height, frame_count=1, frame_duration=1.0, row_offset=0):
        super().__init__(file_path, xpos, ypos, scale_value)
        self.tile_width = tile_width
        self.tile_height = tile_height
        self.frame_count = frame_count
        self.frame_duration = frame_duration
        self.row_offset = row_offset
        self.current_frame = 0
        self.elapsed_time = 0.0

        # Precompute frame rectangles
        self.image = pygame.image.load(file_path).convert_alpha()
        self.frames = []
        image_width = self.image.get_width()
        for i in range(frame_count):
            tile_x = i % (image_width // tile_width)
            tile_y = (i // (image_width // tile_width)) + row_offset
            rect = pygame.Rect(tile_x * tile_width, tile_y * tile_height, tile_width, tile_height)
            self.frames.append(rect)

    def render(self, screen, delta_time):
        # Update animation frame
        self.elapsed_time += delta_time
        if self.elapsed_time >= self.frame_duration:
            self.current_frame = (self.current_frame + 1) % self.frame_count
            self.elapsed_time = 0.0

        # Render the current frame
        src_rect = self.frames[self.current_frame]
        frame_image = self.image.subsurface(src_rect)
        scaled_frame = pygame.transform.scale(
            frame_image,
            (int(self.tile_width * self.scale), int(self.tile_height * self.scale))
        )
        screen.blit(scaled_frame, (self.x, self.y))
        
    def adjust(self, file_path, xpos, ypos, scale_value, tile_width, tile_height, frame_count, frame_duration, row_offset):
        # Update parameters
        self.x = xpos
        self.y = ypos
        self.scale = scale_value
        self.tile_width = tile_width
        self.tile_height = tile_height
        self.frame_count = frame_count
        self.frame_duration = frame_duration
        self.row_offset = row_offset

        # Reload image and recompute frames
        self.image = pygame.image.load(file_path).convert_alpha()
        self.frames = []
        image_width = self.image.get_width()
        for i in range(frame_count):
            tile_x = i % (image_width // tile_width)
            tile_y = (i // (image_width // tile_width)) + row_offset
            rect = pygame.Rect(tile_x * tile_width, tile_y * tile_height, tile_width, tile_height)
            self.frames.append(rect)

        # Reset animation state
        self.current_frame = 0
        self.elapsed_time = 0.0




pygame.init()

# Screen setup
screen = pygame.display.set_mode((800, 600))
clock = pygame.time.Clock()

# Actors
static_sprite = Actor("img/player/Idle/0.png", 100, 100, 2.0)
animated_sprite = AnimatedActor(
    ["img/player/Idle/0.png", "img/player/Idle/1.png", "img/player/Idle/2.png", "img/player/Idle/3.png"],
    300, 300, 2.0, 0.2
)
sprite_sheet_actor = SpriteSheetActor("img/Attack1.png", 500, 100, 2.0, 126, 126, 7, 0.1)

# Game loop
running = True
while running:
    delta_time = clock.tick(60) / 1000.0  # Time in seconds

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Rendering
    screen.fill((255, 255, 255))
    static_sprite.render(screen)
    animated_sprite.render(screen, delta_time)
    sprite_sheet_actor.render(screen, delta_time)
    pygame.display.flip()

pygame.quit()

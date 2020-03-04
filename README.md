# SuperPong
A small C++ game dev project using SDL2

## Version history:

### P 0.1
As of time of creation, the program just generates a back window where two white rectangles can be moved using the arrow keys.

### P 0.2
- Added ball and collision system

## Bugs:
- ~~The ball can get through a wall if it hits just in the corner~~ P 0.3: Ball position capped to game area
- ~~Moving the paddles into the ball traps the ball inside the paddle~~ P 0.3: Fixed thanks to the interaction observer
- ~~When the ball hits the left paddle's top corners it can get trapped inside the paddle (failure to detect corner collision)~~ P 0.3: Fixed correcting collision calculation 

## P 0.3:
- [x] Add timers to cap fps
- [x] Add text and score
- [x] Use normals for automatic reflection calculation in collisions
- [x] Use the whole ball for collision instead of just the center for extra accurate (and satisfying) collisions (should fix collision bugs)
- [x] Fix wonky paddle collision

## P 0.3 other changes:
- Added an interaction observer (following design pattern) to handle game object interactions
- Added Quad class as a float analog to SDL_Rect with expanded functionality
- Fixed collision bugs

## P 0.4 ToDo list:
- [x] Switch to hardware accelerated rendering
- [x] Allow vertical text alignment
- [x] Add support for different game states
- [ ] Add start screen
- [ ] Add pause screen (with info)
- [ ] Add end confirmation screen
- [ ] Change paddle collision angle in relation to hit distance from center
- [ ] Fix wonky paddle collision again

## P 0.4 other changes:
- Added some internal code for things
- Added 3s countdown before the game begins, with graphics!

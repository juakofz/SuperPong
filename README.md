# SuperPong
A small C++ game dev project using SDL2

## Version history:

### P 0.1
As of time of creation, the program just generates a back window where two white rectangles can be moved using the arrow keys.

### P 0.2
- Added ball and collision system

### P 0.3:
- [x] Add timers to cap fps
- [x] Add text and score
- [x] Use normals for automatic reflection calculation in collisions
- [x] Use the whole ball for collision instead of just the center for extra accurate (and satisfying) collisions (should fix collision bugs)
- [x] Fix wonky paddle collision

#### P 0.3 other changes:
- Added an interaction observer (following design pattern) to handle game object interactions
- Added Quad class as a float analog to SDL_Rect with expanded functionality
- Fixed collision bugs

### P 0.4 ToDo list:
- [x] Switch to hardware accelerated rendering
- [x] Allow vertical text alignment
- [x] Add support for different game states
- [x] Add pause screen
- [x] Add score limit
- [x] Add game end screen and reset
- [x] Change change ball speed in relation to distance from center
- [ ] Fix wonky paddle collision again

#### P 0.4 other changes:
- Added some internal code for things
- Added 3s countdown before the game begins, with graphics!
- Fixed document structure :p
- Added score limit per game
- Ball speed goes up as the game goes on

### P 0.5 ToDo list:
- [ ] Add start screen
- [ ] Add info and options to screens
- [ ] Add simple AI
- [ ] Add taunts
- [ ] Make font properly centered

## Bugs:
- Moving the paddles into the ball traps the ball inside the paddle
- Countdown text is slightly off center
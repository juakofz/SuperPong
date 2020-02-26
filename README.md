# SuperPong
A small C++ game dev project using SDL2

## Version history:

### P 0.1
As of time of creation, the program just generates a back window where two white rectangles can be moved using the arrow keys.

### P 0.2
- Added ball and collision system

## Bugs:
- The ball can get through a wall if it hits just in the corner
- Moving the paddles into the ball traps the ball inside the paddle

## P 0.3 ToDo list:
- [x] Add timers to cap fps
- [x] Add text and score
- [ ] Use normals for automatic reflection calculation in collisions
- [ ] Use the whole ball for collision instead of just the center for extra accurate collisions (should fix collision bugs)

## P 0.4 ToDO list:
- [ ] Add different game states
- [ ] Add start screen
- [ ] Add pause screen (with info)
- [ ] Add end confirmation screen

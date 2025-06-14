#include"player.h"
#include"entity.h"
#include"stack.h"
#include"dllist.h"
#include"room.h"
#include"maze.h"
#include<iostream>

//  Player( ... )
//		Constructs and initializes the Player/Entity and its member variables
//		Remembers and discovers the starting room.
Player::Player(Maze *maze, Room p, std::string name, char sprite, const bool tbacktrack)
    : Entity(maze, p, name, sprite),
      m_lookingPaper(),
      m_discoveredRooms(),
      m_btStack(),
      BACKTRACKENABLED(tbacktrack) {
    // Discover the starting room
    m_discoveredRooms.add_front(p);
    m_lookingPaper.push(p);
}

//  stuck()
//		See if the player is stuck in the maze (no solution)
bool Player::stuck() const {
    return Entity::state() == State::NOEXIT;
}

//  foundExit()
//		See if the player has found the exit
bool Player::foundExit() const {
    return Entity::state() == State::EXIT;
}

//  getTargetRoom()
//		Get the room the player wants to look around next.
//      If m_lookingPaper is empty then return an invalid room.
Room Player::getTargetRoom() const {
    if (m_lookingPaper.empty()) return Room(-1, -1);
    return m_lookingPaper.peek();
}


// say()
//		What does the player say?
//		Already Implemented, nothing to do here
void Player::say() {
    // Freedom supercedes being eaten
    if (state() == State::EXIT) {
        std::cout << name() << ": WEEEEEEEEE!";
        return;
    }

    // Being eaten supercedes being lost
    switch (interact()) {
        case Interact::ATTACK:
            std::cout << name() << ": OUCH!";
            break;
        case Interact::GREET:
            break;
        case Interact::ALONE:
        default:
            switch (state()) {
                case State::LOOK:
                    std::cout << name() << ": Where is the exit?";
                    break;
                case State::NOEXIT:
                    std::cout << name() << ": Oh no! I am Trapped!";
                    break;
                case State::BACKTRACK:
                    std::cout << name() << ": Got to backtrack...";
                    break;
                default:
                    std::cout << name() << ": A - hee - ahee ha - hee!";
                    break;
            }

            break;
    }
}


//  update()
//		This function implements an algorithm to look through the maze
//      for places to move (STATE::LOOK). update also handles moving the
//		player as well, if there is an open undiscovered cell to move to the
//		player just moves there.  However, there will be cases in the next
//		desired room to look around is not adjacent (more than one cell away),
//		peek this room the player must backtrack (State::BACKTRACK) to a room
//		that is adjacent to the next room the player was planning to visit
//
//		The player can only do one thing each call, they can either LOOK or
//		BACKTRACK not both.  Nor should the player move more than one cell per
//		call to update.  If you examine the function calls for the entire game
//		you should observe that this is already be called within a loop. Your
//		Implementation should NOT have any loops that pertain to actual movement
//		of the player.
//
//		Backtracking is challenging, save it for the very very very last thing.
//		Make sure the STATE::LOOK aspect compiles and works first.
// Update function
void Player::update() {

    switch (state()) {
        case State::EXIT:
            return;
        case State::NOEXIT:
            return;
        case State::BACKTRACK: {
            // Backtrack to the previous room
            Room previous = m_btStack.peek();
            m_btStack.pop();
            move(previous);
            // If the backtrack stack is empty, or  then we are done backtracking
            bool adjacent = room().adjacent(m_lookingPaper.peek());
            if (m_btStack.empty() || adjacent) {
                state(State::LOOK);
            }
            break;
        }
        case State::LOOK: {
            Room target = getTargetRoom();
            m_lookingPaper.pop();
            if (BACKTRACKENABLED)
                // Save current room to backtrack later
                m_btStack.push(room());
            move(target);
            //check if current room is the exit
            if (maze()->foundExit(target)) {
                state(State::EXIT);
                return;
            }
            //initialize the directions to check
            Room direction[] = {
                target + Room(-1, 0), target + Room(1, 0),
                target + Room(0, -1), target + Room(0, 1)
            };
            // Check all four directions for open rooms
            for (int i = 0; i < 4; i++) {
                if (maze()->open(direction[i]) &&
                    m_discoveredRooms.search(direction[i]) == -42) {
                    m_lookingPaper.push(direction[i]);
                    m_discoveredRooms.add_front(direction[i]);
                }
            }
            //checks if player is stuck
            if (m_lookingPaper.empty()) {
                state(State::NOEXIT);
                return;
            }
            if (BACKTRACKENABLED) {
                // check if the next target is not adjacent
                bool adjacent = room().adjacent(m_lookingPaper.peek());
                if (!adjacent)
                    state(State::BACKTRACK);
            }
            break;
        }
    }
}

#ifndef DIRECTION_HPP
#define DIRECTION_HPP

enum Direction {NORTH, SOUTH, EAST, WEST, UP, DOWN};

static inline glm::vec3 getDirectionVector(enum Direction direction){
    glm::vec3 directionVector[] = {
        glm::vec3( 0.0f,  0.0f, -1.0f), // north (-z)
        glm::vec3( 0.0f,  0.0f,  1.0f), // south (+z)
        glm::vec3( 1.0f,  0.0f,  0.0f), // east (+x)
        glm::vec3(-1.0f,  0.0f,  0.0f), // west (-x)
        glm::vec3( 0.0f,  1.0f,  0.0f), // up (+y)
        glm::vec3( 0.0f, -1.0f,  0.0f), // down (-y)
    };
    return directionVector[direction];

}


#endif

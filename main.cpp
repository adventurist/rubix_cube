#include <iostream>
#include <variant>
#include <vector>
#include <map>

enum Colour { Red, Green, Blue, Yellow, White, Orange };
enum Axis { x, y };
enum Direction { FORWARD, BACKWARD };
enum Layer { FIRST, MIDDLE, OUTER };

/**
 * Side
 *
 * A side has a starting colour with coordinates intended to initially match that colour.
 * A side is solved whenever all of the coordinates have the same colour
 */
struct Side {
  Colour colour;
  bool solved;
  std::vector<Colour> coordinates;
};

/**
 * createSides
 *
 * @helper
 * @returns {std::vector<Side} A vector of 9-coordinate sides of distinct colour, expressing the
 * initial aesthetic state of a solved RubixCube
 */
std::vector<Side> createSides() {
  std::vector<Side> sides{};
  std::vector<int> num_of_coordinates {0, 1, 2, 3, 4, 5, 6, 7, 8};

  for (auto colour : { Red, Green, Blue, Yellow, White, Orange }) {
    std::vector<Colour> coordinates{};
    for (auto i : num_of_coordinates) {
      coordinates.push_back(colour);
    }
    sides.push_back(Side{colour, false, coordinates});
  }
  return sides;
}

/**
 * RubixTurnInterface
 *
 * The turn interface of Rubik's cube is distinguished as the turning of one of 3 possible layers of
 * sequential faces in 3 dimensional space in a chosen direction
 *
 * @interface
 * @abstract
 *
 */
class RubixTurnInterface {
  public:
    virtual void turn(Axis axis, Layer layer, Direction direction) = 0;
};

/**
 * RubixCube
 *
 * @class
 * @implements RubixTurnInterface
 *
 */
class RubixCube : public RubixTurnInterface {
 public:
  RubixCube() {
    m_sides = createSides();
    solved = false;
    facing_side = 0;
  }

  bool isSolved() { return solved; }

  void turn(Axis axis, Layer layer, Direction direction) {
      if (axis == Axis::x) {
        std::cout << "Horizontal turn" << std::endl;
        rotateX(layer, direction);
      } else {
        std::cout << "Vertical turn" << std::endl;
        rotateY(layer, direction);
      }
  }

  std::vector<int> getInitialIndexesX(int face = 0) {
    return std::vector<int>{0, 1, 2, 3};
  }

  std::vector<int> getInitialIndexesY(int face = 0) {
    return std::vector<int>{0, 4, 2, 5};
  }

  /**
   * RubixCube::rotateX
   *
   * @helper
   * @parameter {Layer} layer The layer to rotate
   * @parameter {Direction} direction The direction in which to elicit the rotation
   */
  void rotateX(Layer layer, Direction direction) {
    // TODO: Convert to rotateY's implementation
    Colour initial_side_colours[4]{};
    std::vector<int> initial_side_indexes = getInitialIndexesX();
    for (int i : initial_side_indexes) {
      initial_side_colours[i] = m_sides[i].colour;
    }
    int coordinates[3]{};

    switch (layer) {
      case Layer::FIRST: {
        coordinates[0] = 0;
        coordinates[1] = 1;
        coordinates[2] = 2;
        break;
      }
      case Layer::MIDDLE: {
        coordinates[0] = 3;
        coordinates[1] = 4;
        coordinates[2] = 5;
        break;
      }
      case Layer::OUTER: {
        coordinates[0] = 6;
        coordinates[1] = 7;
        coordinates[2] = 8;
      }
    }

    for (int i : initial_side_indexes) {
      for (int j : coordinates) {
        if (direction == Direction::FORWARD) {
          m_sides[i].coordinates.at(j) = initial_side_colours[j == 0 ? 2 : j - 1];
        } else {
          m_sides[i].coordinates.at(j) = initial_side_colours[j == 2 ? 0 : j + 1];
        }
      }
    }
  }

  std::map<int, int> getVerticalTransformationIndexForSide(int side = 0) {
    switch (side) {
      case 0: {
        return std::map<int, int> {
        {0, 5},
        {4, 0},
        {2, 4},
        {5, 2}
        };
        break;
      }
      case 1: {
        return std::map<int, int> {
        {1, 5},
        {4, 1},
        {3, 4},
        {5, 3}
        };
        break;
      }
      case 2: {
        return std::map<int, int> {
        {2, 5},
        {4, 2},
        {0, 4},
        {5, 0}
        };
        break;
      }
      case 3: {
        return std::map<int, int> {
        {3, 5},
        {4, 3},
        {1, 4},
        {5, 1}
        };
        break;
      }
    }
  }
  /**
   * RubixCube::rotateY
   *
   * @helper
   * @parameter {Layer} layer The layer to rotate
   * @parameter {Direction} direction The direction in which to elicit the rotation
   */
  void rotateY(Layer layer, Direction direction) {
    int coordinates[3]{};

    switch (layer) {
      case Layer::FIRST: {
        coordinates[0] = 0;
        coordinates[1] = 3;
        coordinates[2] = 6;
        break;
      }
      case Layer::MIDDLE: {
        coordinates[0] = 1;
        coordinates[1] = 4;
        coordinates[2] = 7;
        break;
      }
      case Layer::OUTER: {
        coordinates[0] = 2;
        coordinates[1] = 5;
        coordinates[2] = 8;
      }
    }

    std::map<int, int> coordinate_map = getVerticalTransformationIndexForSide(0);
    std::map<int, std::vector<Colour> > initial_side_colours;

    for (auto& kv : coordinate_map) {
      initial_side_colours.insert({
        kv.first, std::vector<Colour>{
          m_sides[kv.first].coordinates.at(coordinates[0]),
          m_sides[kv.first].coordinates.at(coordinates[1]),
          m_sides[kv.first].coordinates.at(coordinates[2])}
        });
    }

    for (auto& transformation_pair : coordinate_map) {
      int coordinate_index = 0;

      int source = direction == Direction::FORWARD ? transformation_pair.first : transformation_pair.second;
      int destination = direction == Direction::FORWARD ? transformation_pair.second : transformation_pair.first;
      for (int coordinate_value : coordinates) {
        m_sides.at(destination).coordinates.at(coordinate_value) = initial_side_colours[source].at(coordinate_index);
        coordinate_index++;
      }
    }
  }

  /**
   * Scramble
   *
   * Shuffles the colours of the coordinates in the RubixCube
   */
  void scramble() {
    std::cout << "Shuffling and randomizing this cube's configuration" << std::endl;
  }

 private:
  std::vector<Side> m_sides;
  int facing_side;
  bool solved;
};

/**
 * solveRubixCube
 *
 * @helper
 * @parameter[in/out] {RubixCube} cube A RubixCube upon which to perform actions as dictated by inputted instructions
 */
void solveRubixCube (RubixCube &cube) {
  int axis{}, layer{}, direction{};
  std::cout << "1. Vertical Turn" << "\n" << "2.  Horizontal Turn" << "\n\n" << std::endl;

  while (std::cin >> axis) {

    if (axis < 1 || axis > 2) {
      std::cout << "Invalid turn" << std::endl;
      break;
    }

    std::cout << "Choose layer: 1   2   3" << std::endl;
    std::cin >> layer;

    if (layer < 1 || layer > 3) {
      std::cout << "You did not enter a valid layer" << std::endl;
      break;
    }

    std::cout << "Choose direction: 1 (left)  2 (right)" << std::endl;
    std::cin >> direction;

    if (direction < 1 || direction > 2) {
      std::cout << "Invalid direction" << std::endl;
      break;
    }

    Axis axis_value{};
    Layer layer_value{};
    Direction direction_value{};

    axis_value = axis == 1 ? Axis::x : Axis::y;

    direction_value = direction == 1 ? Direction::FORWARD : Direction::BACKWARD;

    layer_value = layer == 1 ? Layer::FIRST :
                  layer == 2 ? Layer::MIDDLE :
                               Layer::OUTER;

    cube.turn(axis_value, layer_value, direction_value);
  }
}

int main() {
  RubixCube rubix_cube{};

  solveRubixCube(rubix_cube);

  auto solved = rubix_cube.isSolved();

  if (solved) {
    std::cout << "Your cube is solved!" << std::endl;
  } else {
    std::cout << "Your cube is not solved, bitch" << std::endl;
  }

  return 0;
}

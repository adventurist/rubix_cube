#include <iostream>
#include <variant>
#include <vector>
#include <map>
#include <random>

// Enums
enum class Colour : int {
  Red = 0,
  Green = 1,
  Blue = 2,
  Yellow = 3,
  White = 4,
  Orange= 5
};
// Override insertion operator for easy printing
std::ostream& operator<< (std::ostream& os, Colour colour) {
  switch (colour) {
    case Colour::Red : return os << "Red";
    case Colour::Green : return os << "Green";
    case Colour::Blue : return os << "Blue";
    case Colour::Yellow : return os << "Yellow";
    case Colour::White : return os << "White";
    case Colour::Orange : return os << "Orange";
    default : return os << "UNKNOWN COLOUR";
  }
}

enum Axis { x, y };
enum Direction { FORWARD, BACKWARD };
enum Layer { FIRST, MIDDLE, LAST };

/**
 * Side
 *
 * A side is solved whenever all of the coordinates have the same colour
 */
struct Side {
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

  for (auto colour : { Colour::Red, Colour::Green, Colour::Blue, Colour::Yellow, Colour::White, Colour::Orange }) {
    std::vector<Colour> coordinates{};
    for (auto i : num_of_coordinates) {
      coordinates.push_back(colour);
    }
    sides.push_back(Side{true, coordinates});
  }
  return sides;
}

/**
 * getCoordinates
 *
 * Returns a set of 3 indexes representing coordinates on a RubixCube
 *
 * @helper
 * @param {Axis} axis The axis along which to draw coordinates
 * @param {Layer} layer The layer from which to draw coordinates
 * @returns {std::vector<int} A vector integers representing a set of 3 coordinates out of 9 on
 * the side of a RubixCube
 */
std::vector<int> getCoordinates(Axis axis, Layer layer) {
  std::vector<int> coordinates{0, 0, 0};

  if (axis == Axis::y) {
    switch (layer) {
      case Layer::FIRST: {
        coordinates.at(0) = 0;
        coordinates.at(1) = 3;
        coordinates.at(2) = 6;
        break;
      }
      case Layer::MIDDLE: {
        coordinates.at(0) = 1;
        coordinates.at(1) = 4;
        coordinates.at(2) = 7;
        break;
      }
      case Layer::LAST: {
        coordinates.at(0) = 2;
        coordinates.at(1) = 5;
        coordinates.at(2) = 8;
      }
    }
  } else {
    switch (layer) {
      case Layer::FIRST: {
        coordinates.at(0) = 0;
        coordinates.at(1) = 1;
        coordinates.at(2) = 2;
        break;
      }
      case Layer::MIDDLE: {
        coordinates.at(0) = 3;
        coordinates.at(1) = 4;
        coordinates.at(2) = 5;
        break;
      }
      case Layer::LAST: {
        coordinates.at(0) = 6;
        coordinates.at(1) = 7;
        coordinates.at(2) = 8;
      }
    }
  }
  return coordinates;
}

/**
 * getTransformationMap
 *
 * Returns a map of integers representing sides of a RubixCube that are related through
 * axis of perception.
 *
 * @helper
 * @param <Axis> axis The axis upon which to perceive the cube
 * @param <int> side The front-facing side of the cube
 * @returns <std::map<int, int>> A map of integers representing sides of a RubixCube that have been related
 * for use in a transformation appropriate to the axis provided.
 */

std::map<int, int> getTransformationMap(Axis axis, int side = 0) {
  if (axis == Axis::x) {
    switch (side) {
      case 0: {
        return std::map<int, int> {
        {0, 4},
        {4, 2},
        {2, 5},
        {5, 0}
        };
        break;
      }
      case 1: {
        return std::map<int, int> {
        {1, 4},
        {4, 3},
        {3, 5},
        {5, 1}
        };
        break;
      }
      case 2: {
        return std::map<int, int> {
        {2, 4},
        {4, 0},
        {0, 5},
        {5, 2}
        };
        break;
      }
      case 3: {
        return std::map<int, int> {
        {3, 4},
        {4, 1},
        {1, 5},
        {5, 3}
        };
        break;
      }
    }
  } else {
    return std::map<int, int> {
      {0, 1}, {1, 2}, {2, 3}, {3, 0}
    };
  }
  return std::map<int, int> {};
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
        std::cout << "Rotating vertically" << std::endl;
        rotate(Axis::x, layer, direction);
      } else {
        std::cout << "Rotating horizontally" << std::endl;
        rotate(Axis::y, layer, direction);
      }
  }

  void render() {
    std::cout << "\n_______CUBE_______\n" << std::endl;
    int index = 1;
    for (const auto& side : m_sides) {
      std::cout << "## Side " << index << " ##" << std::endl;
      int coordinateIndex = 1;
      for (const auto& coordinate : side.coordinates) {
        std::cout << coordinate << " ";
        if (coordinateIndex % 3 == 0) {
          std::cout << "\n";
        }
        coordinateIndex++;
      }
      std::cout << std::endl;
      index++;
    }
    std::cout << "_____________________\n\n" << std::endl;
  }

  /**
   * RubixCube::rotateY
   *
   * @helper
   * @parameter {Axis} axis The axis upon which to rotate
   * @parameter {Layer} layer The layer to rotate
   * @parameter {Direction} direction The direction in which to elicit the rotation
   */
  void rotate(Axis axis, Layer layer, Direction direction) {
    std::vector<int> coordinates = getCoordinates(axis, layer);
    std::map<int, int> coordinate_map = getTransformationMap(axis, 0);
    std::map<int, std::vector<Colour> > initial_side_colours;
    // Take a snapshot of the sides that are going to be transformed
    for (const auto& kv : coordinate_map) {
      initial_side_colours.insert({
        kv.first, std::vector<Colour>{
          m_sides[kv.first].coordinates.at(coordinates[0]),
          m_sides[kv.first].coordinates.at(coordinates[1]),
          m_sides[kv.first].coordinates.at(coordinates[2])}
        });
    }
    // Perform the transformation
    for (const auto& transformation_pair : coordinate_map) {
      int coordinate_index = 0;
      // Determine the source and destination sides based on direction of rotation
      int source = direction == Direction::FORWARD ? transformation_pair.first : transformation_pair.second;
      int destination = direction == Direction::FORWARD ? transformation_pair.second : transformation_pair.first;
      // Iterate the coordinates and replace their colours in tandem with the desired rotation
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
    std::random_device rd{}; // obtain a random number from hardware
    std::mt19937 mte(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 1); // define the range

    Axis axis_value{};
    Layer layer_value{};
    Direction direction_value{};

    for (int i=0; i < 4000; i++) {
      int axis = distr(mte);
      int direction = distr(mte);
      int layer = distr(mte);

      axis_value = axis == 0 ? Axis::x : Axis::y;
      direction_value = direction == 0 ? Direction::FORWARD : Direction::BACKWARD;
      if (i % 3 == 0) {
        layer_value = Layer::LAST;
      } else {
        layer_value = layer == 0 ? Layer::FIRST :
                      layer == 1 ? Layer::MIDDLE :
                                  Layer::LAST;
      }
      turn(axis_value, layer_value, direction_value);
    }
  }

  void assess() {
    Colour colour{};
    bool assessment = true;
    for (const auto& side : m_sides) {
      for (const auto& coordinate : side.coordinates) {
        if (coordinate == *side.coordinates.begin()) {
          colour = coordinate;
        } else {
          if (coordinate != colour) {
            assessment = false;
            break;
          }
        }
        if (assessment == false) {
          solved = false;
          break;
        }
      }
    }
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
  std::cout << "1. Vertical Turn" << "\n" << "2. Horizontal Turn" << "\n\n" << std::endl;

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
                               Layer::LAST;

    cube.turn(axis_value, layer_value, direction_value);
  }
}

int main() {
  RubixCube rubix_cube{};

  std::cout << "Original state:" << std::endl;
  rubix_cube.render();
  rubix_cube.scramble();
  rubix_cube.render();

  solveRubixCube(rubix_cube);

  rubix_cube.render();
  rubix_cube.assess();

  auto solved = rubix_cube.isSolved();

  if (solved) {
    std::cout << "Your cube is solved!" << std::endl;
  } else {
    std::cout << "Your cube is not solved, you filthy animal" << std::endl;
  }

  return 0;
}

#include <iostream>
#include <variant>
#include <vector>

enum Colour { Red, Green, Blue, Yellow, White, Orange };
enum Axis { x, y };
enum Direction { FORWARD, BACKWARD };
enum Layer { FIRST, MIDDLE, OUTER };

struct Side {
  Colour colour;
  bool solved;
  std::vector<Colour> coordinates;
};

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


class RubixTurnInterface {
  public:
    virtual void turn(Axis axis, Layer layer, Direction direction) = 0;
};

class RubixCube : public RubixTurnInterface {
 public:
  RubixCube() { m_sides = createSides(); }
  bool isSolved() { return solved; }

  void turn() { }
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
    return std::vector<int>{0, 4, 2, 6};
  }

  void rotateX(Layer layer, Direction direction) {
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
    if (layer == Layer::FIRST) {
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
  }

  void rotateY(Layer layer, Direction direction) {
    std::vector<int> initial_side_indexes = getInitialIndexesY();
  }

  void scramble() {
    std::cout << "Shuffling and randomizing this cube's configuration" << std::endl;
  }

 private:
  std::vector<Side> m_sides;
  bool solved = false;
};

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

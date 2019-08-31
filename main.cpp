#include <iostream>
#include <variant>
#include <vector>

enum Colour { Red, Green, Blue, Yellow, White, Orange };
enum Axis { x, y };
enum Layer { FIRST, MIDDLE, OUTER };

struct Side {
  Colour colour;
  bool solved;
  std::vector<Colour> coordinates;
};

std::vector<Side> createSides() {
  return std::vector<Side>{Side{Colour::Red, false, std::vector<Colour>{}},
                           Side{Colour::Blue, false, std::vector<Colour>{}},
                           Side{Colour::Green, false, std::vector<Colour>{}},
                           Side{Colour::Yellow, false, std::vector<Colour>{}},
                           Side{Colour::White, false, std::vector<Colour>{}},
                           Side{Colour::Orange, false, std::vector<Colour>{}}};
}

class RubixTurnInterface {
  public:
    virtual void turn(Axis axis, Layer layer) = 0;
};

class RubixCube : public RubixTurnInterface {
 public:
  RubixCube() { m_sides = createSides(); }
  bool isSolved() { return solved; }

  void turn() { }
  void turn(Axis axis, Layer layer) {
      if (axis == Axis::x) {
        std::cout << "Horizontal turn" << std::endl;
      } else {
        std::cout << "Vertical turn" << std::endl;
      }
    }

 private:
  std::vector<Side> m_sides;
  bool solved = false;
};

int main() {
  RubixCube rubix_cube{};
  int choice{};
  int layer{};
  std::cout << "1. Vertical Turn" << "\n" << "2.  Horizontal Turn" << "\n\n" << std::endl;

  while (std::cin >> choice) {
    std::cout << "Choose layer: 1   2   3" << std::endl;
    std::cin >> layer;
    if (choice ==  1) {
      if (layer == 1)
        rubix_cube.turn(Axis::y, Layer::FIRST);
      else if (layer == 2)
        rubix_cube.turn(Axis::y, Layer::MIDDLE);
      else if (layer == 3)
        rubix_cube.turn(Axis::y,  Layer::OUTER);
      else
        std::cout << "You did not enter a valid layer" << std::endl;
    } else if (choice == 2) {
      if (layer == 1)
        rubix_cube.turn(Axis::x, Layer::FIRST);
      else if (layer == 2)
        rubix_cube.turn(Axis::x, Layer::MIDDLE);
      else if (layer == 3)
        rubix_cube.turn(Axis::x,  Layer::OUTER);
      else
        std::cout << "You did not enter a valid layer" << std::endl;
    }
  }
  auto solved = rubix_cube.isSolved();

  if (solved) {
    std::cout << "Your cube is solved!" << std::endl;
  } else {
    std::cout << "Your cube is not solved, bitch" << std::endl;
  }

  return 0;
}

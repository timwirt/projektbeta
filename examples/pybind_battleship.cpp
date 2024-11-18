#include <../src/battleship_board.cpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace BattleShip;

PYBIND11_MODULE(battleship, m) {
  m.doc() = "Battleship";
  
  py::class_<BattleShipBoard>(m, "BattleShipBoard")
    .def(py::init<>())
    .def("create_ship", &BattleShipBoard::create_ship)
    .def("shoot", &BattleShipBoard::shoot)
    .def("get_board_as_string", &BattleShipBoard::get_board_as_string)
    .def("get_board_as_string_without_ships", &BattleShipBoard::get_board_as_string_without_ships)
    .def("all_ships_sunken", &BattleShipBoard::all_ships_sunken);
}

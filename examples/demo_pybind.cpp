#include <hangman/hangman_board.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace Hangman;


PYBIND11_MODULE(hangman, m) {
  m.doc() = "Hangman Example";
  
  py::class_<HangmanBoard>(m, "HangmanBoard")
    .def(py::init<>())
    .def(py::init<const std::string&, const len_t>())
    .def("guess_letter", &HangmanBoard::guess_letter)
    .def("guess_secret_word", &HangmanBoard::guess_secret_word)
    .def("secret_length", &HangmanBoard::secret_length)
    .def("get_num_wrong_guesses", &HangmanBoard::get_num_wrong_guesses)
    .def("get_current_guess", &HangmanBoard::get_current_guess);
}

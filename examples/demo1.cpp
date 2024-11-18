#include <iostream>

#include <hangman/hangman_board.hpp>

int main() {
  std::cout << "Hi" << std::endl;
  Hangman::HangmanBoard h("Geheimeswort", 5);
  std::cout << h.secret_length() << std::endl;
  std::cout << h << std::endl;
  
  h.guess_letter('G');
  std::cout << h.get_num_wrong_guesses() << std::endl;
  std::cout << h << std::endl;
  
  h.guess_letter('e');
  std::cout << h.get_num_wrong_guesses() << std::endl;
  std::cout << h << std::endl;
  
  h.guess_letter('h');
  std::cout << h.get_num_wrong_guesses() << std::endl;
  std::cout << h << std::endl;
  
  h.guess_letter('h');
  std::cout << h.get_num_wrong_guesses() << std::endl;
  std::cout << h << std::endl;
  
  h.guess_letter('u');
  std::cout << h.get_num_wrong_guesses() << std::endl;
  std::cout << h << std::endl;
  
}

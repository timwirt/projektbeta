#pragma once

#include <string>

namespace Hangman {

using len_t = size_t;
using letter_t = std::string::value_type;
  
class HangmanBoard {
public:
  HangmanBoard();
  HangmanBoard(const std::string& secret_word, const len_t max_num_wrong_guesses);
  bool guess_letter(const letter_t& guess);
  bool guess_secret_word(const std::string& guess);
  size_t secret_length() const;
  len_t get_num_wrong_guesses() const;
  std::string get_current_guess() const;
  

private:
  const std::string secret_word;
  std::string current_guess;
  const len_t max_num_wrong_guesses;
  len_t current_num_wrong_guesses;

  friend std::ostream& operator<<(std::ostream&, const HangmanBoard&);
};



}

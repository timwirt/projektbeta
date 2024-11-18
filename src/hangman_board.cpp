#include <algorithm>
#include <iostream>
#include <hangman/hangman_board.hpp>

namespace Hangman{

HangmanBoard::HangmanBoard() :
  HangmanBoard("Hi", 3)
{
  
}

HangmanBoard::HangmanBoard(const std::string& secret_word, const len_t max_num_wrong_guesses) :
  secret_word(secret_word),
  current_guess(secret_word.length(), '?'),
  max_num_wrong_guesses(max_num_wrong_guesses),
  current_num_wrong_guesses(0)
{
  
}

bool HangmanBoard::guess_letter(const letter_t& guess) {
  if (std::find(begin(current_guess), end(current_guess), guess) != end(current_guess) or std::find(begin(secret_word), end(secret_word), guess) == end(secret_word)) {
    current_num_wrong_guesses += 1;
    return false;
  } else {
    for (size_t i = 0; i < current_guess.length(); ++i) {
      if (secret_word[i] == guess) {
        current_guess[i] = guess;
      }
    }
    return true;
  }
}


bool HangmanBoard::guess_secret_word(const std::string& guess) {
  if (secret_word == guess) {
    return true;
  } else {
    current_num_wrong_guesses += 1;
    return false;
  }
}


size_t HangmanBoard::secret_length() const {
  return secret_word.length();
}


len_t HangmanBoard::get_num_wrong_guesses() const {
  return current_num_wrong_guesses;
}


std::string HangmanBoard::get_current_guess() const {
  return current_guess;
}


std::ostream& operator<<(std::ostream& os, const HangmanBoard& h) {
  return (os << h.secret_word << std::string(" ") << h.current_guess);
}

}

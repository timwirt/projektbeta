# Import magic
try:
  import hangman
except ImportError as e:
  print(f"Importing the shared library 'hangman' did not work.")
  print(f"Is (a link to) the shared library 'hangman.____.so' in the same directory as this python script?")
  print(f"The import caused the following exception '{e}'")
  print(f"Exiting")
  exit(1)
else:
  print(f"Importing the shared library 'hangman' did work.")


from hangman import HangmanBoard
import pydoc


def main():
  print(f"Module name: '{hangman.__doc__}'")
  print(f"HangmanBoard doc: {pydoc.render_doc(HangmanBoard)}")
  
  h = HangmanBoard("Secret", 5)
  print(f"Current guess: '{h.get_current_guess()}'   Number of wrong guesses: {h.get_num_wrong_guesses()}'")
  h.guess_letter('v')
  print(f"Current guess: '{h.get_current_guess()}'   Number of wrong guesses: {h.get_num_wrong_guesses()}'")
  h.guess_letter('e')
  print(f"Current guess: '{h.get_current_guess()}'   Number of wrong guesses: {h.get_num_wrong_guesses()}'")
  h.guess_letter('G')
  print(f"Current guess: '{h.get_current_guess()}'   Number of wrong guesses: {h.get_num_wrong_guesses()}'")
  h.guess_letter('S')
  print(f"Current guess: '{h.get_current_guess()}'   Number of wrong guesses: {h.get_num_wrong_guesses()}'")
  h.guess_letter('e')
  print(f"Current guess: '{h.get_current_guess()}'   Number of wrong guesses: {h.get_num_wrong_guesses()}'")


if __name__ == '__main__':
  main()

# Start demo_fastapi_2 in background first

import requests
from pprint import pprint # Bei größeren Dicts sollte man pprint(...) statt print(...) verwenden


def main():
  base_api_url = "http://127.0.0.1:8000"
  
  response = requests.get(base_api_url)
  response_json = response.json()
  current_guess = response_json['Current guess']
  print(f"Current guess: '{current_guess}'")
  
  while current_guess.find("?") != -1:
    letter = input(f"Buchstabe raten:")
    response_json = requests.get(f"{base_api_url}/guess/{letter}").json()
    current_guess = response_json['Current guess']
    print(f"Current guess: '{current_guess}'")
  

if __name__ == '__main__':
  main()

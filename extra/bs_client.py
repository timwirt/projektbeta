# Start bs_server in background first

try:
  import battleship
except ImportError as e:
  print(f"Importing the shared library 'battleship' did not work.")
  print(f"Is (a link to) the shared library 'battleship.____.so' in the same directory as this python script?")
  print(f"The import caused the following exception '{e}'")
  print(f"Exiting")
  exit(1)

import time
from battleship import BattleShipBoard
import requests
from pprint import pprint 

#connect to game while its running
def connect():

    '''Spiel beitreten'''
    global cname
    global cid
    global host_status
    global lobby
    # get cname from player by console
    cname = input("Spielername: ")

    # generate Client_ID by cname
    cid = hash(cname)

    # Join the game
    join = requests.get(f"{base_api_url}/join/{cid}/{cname}")
    join_json = join.json()

    # Get host_status from lobby
    lobby = requests.get(f"{base_api_url}/lobby")
    lobby_json = lobby.json()

    host_status = lobby_json[str(cid)]['host']

def wait_for_start():
    global host_status
    global lobby

    lobby = requests.get(f"{base_api_url}/lobby").json()
    '''wait for the start from host'''
    while (lobby['started'] == False):
        try:
            lobby = requests.get(f"{base_api_url}/lobby").json()

            # tes if host changed
            if(host_status != lobby[str(cid)]['host']):
                host_status = True
                print("Du bist nun der Host!")

            # only the host can do inputs
            if(host_status):
                action = input()
                lobby = requests.get(f"{base_api_url}/lobby").json()
                if(action == 'start'):
                    if (lobby['player'] >= 2):
                        #set 'started' = True
                        lobby = requests.get(f"{base_api_url}/lobby/start/True").json()

                        # Set intern_id for host
                        response = requests.get(f"{base_api_url}/lobby/intern_id/{cid}")
                        return
                    else:
                        print(f"Es gibt nicht genug Spieler. Aktuell sind {lobby['player']} Spieler in der Lobby!")
            
        # Except that a player leaves by ctrl + c
        except (KeyboardInterrupt):
        # Leave the game
            response = requests.get(f"{base_api_url}/leave/{cid}")
            print("Du hast das Spiel verlassen")

            if host_status:
                response = requests.get(f"{base_api_url}/stats/host_end/bool/True")
                response = requests.get(f"{base_api_url}/lobby/reset_host")

            exit()

    # Set intern_id for non-host
    response = requests.get(f"{base_api_url}/lobby/intern_id/{cid}")

    print('Das Spiel startet nun.')
    return

def place_ships():
    global intern_id
    global bsb
    global other_player

    bsb = BattleShipBoard()

    player_infos = requests.get(f"{base_api_url}/lobby").json()[str(cid)]
    ships_dict = player_infos['ships']
    intern_id = player_infos['intern_id']

    other_player = (intern_id + 1) % 2

    print('Platziere nun deine Schiffe: \n Bsp.: Schlachtschiff West D:9')

    '''spielfeld muss in player_json gespeichert werden'''
    
    # Test if all ships are placed
    while (sum(ships_dict.values()) != 0):
        try:
            print(bsb.get_board_as_string(int(intern_id)))

            if (requests.get(f"{base_api_url}/stats").json()['host_end']):
                print("Zu wenig Spieler! Das Spiel wurde beendet!")
                response = requests.get(f"{base_api_url}/lobby/reset_lobby")
                response = requests.get(f"{base_api_url}/stats/reset_stats")

                exit()

            place_input = input().split()

            #except not working input
            try:
                # Test if ships from this type are left
                if(ships_dict[place_input[0]] == 0):
                    print('Du hast von dem Schiffstypen keine Schiffe mehr übrig!')
                
                # Try to place the ship
                elif(bsb.create_ship(intern_id, place_input[0], place_input[1], place_input[2])):
                    ships_dict = requests.get(f"{base_api_url}/lobby/{cid}/ships/{place_input[0]}").json()[str(cid)]['ships']
                else:
                    print('Dein Schiff darf dort nicht platziert werden. Probiere eine andere Stelle!')
            except:
                print("Das war eine ungültige Eingabe. Probiere diese Form: Schlachtschiff West D:9")        
        
        # Except that a player leaves by ctrl + c
        except (KeyboardInterrupt):
        # Leave the game
            leave_question = input("\n Sicher, dass du das Spiel verlassen möchtest? J/N \n")
            if (leave_question == "J"):

                response = requests.get(f"{base_api_url}/leave/{cid}")
                print("Du hast das Spiel verlassen")

                response = requests.get(f"{base_api_url}/stats/host_end/bool/True")

                exit()

    # Wait for other player placed the shipsa
    lobby = requests.get(f"{base_api_url}/lobby").json()
    player_list = lobby['player_list']
    other_player_infos = lobby[str(player_list[other_player])]
    other_ships_dict = other_player_infos['ships']

    while((sum(ships_dict.values()) != 0) or (sum(other_ships_dict.values()) != 0)):
        print("Warte bis alle Schiffe platziert wurden!\n")
        time.sleep(1)

        # Update informations
        lobby = requests.get(f"{base_api_url}/lobby").json()
        player_list = lobby['player_list']
        other_player_infos = lobby[str(player_list[other_player])]
        other_ships_dict = other_player_infos['ships']

    # Write board without ships to Server

    print('Alle Schiffe sind nun platziert. Los geht\'s!')
    time.sleep(1)

    return

def shooting():
    game_stats = requests.get(f"{base_api_url}/stats").json()
    lobby = requests.get(f"{base_api_url}/lobby").json()
    guess_other_player = lobby[str(lobby['player_list'][other_player])]['guess']

    # loop while game not ended by itself
    while (not game_stats['game_end']):
        
        try:
            # If host ended the game, clear the lobby and stats
            if (requests.get(f"{base_api_url}/stats").json()['host_end']):
                print("Zu wenig Spieler! Das Spiel wurde beendet!")
                response = requests.get(f"{base_api_url}/lobby/reset_lobby")
                response = requests.get(f"{base_api_url}/stats/reset_stats")

                exit()

            # Client is current player and can shoot
            if (intern_id == game_stats['current_player']):
                print("Du bist an der Reihe! \n Wohin möchtest du schießen? \n")
                guess = input()
                response = requests.get(f"{base_api_url}/lobby/{cid}/guess/{guess}")

                # get hit answer
                time.sleep(1)
                lobby = requests.get(f"{base_api_url}/lobby").json()

                # Client can shoot while hitting a ship
                while(lobby[str(lobby['player_list'][other_player])]['hit'] == "ship_hit" or lobby[str(lobby['player_list'][other_player])]['hit'] == "exception"):
                    # Print game boards
                    print("Dein Spielfeld: \n")
                    print(bsb.get_board_as_string(int(intern_id)))
                    print("Das Spielfeld deines Gegners: \n")
                    print(lobby[str(lobby['player_list'][other_player])]['board_without_ships'])

                    # Test if Client already won
                    if(lobby[str(lobby['player_list'][other_player])]['all_ships_sunken']):
                        response = requests.get(f"{base_api_url}/stats/game_end/bool/True")
                        print("Gewonnen!")
                        exit()
                    
                    # Shoot again
                    else:
                        if(lobby[str(lobby['player_list'][other_player])]['hit'] == "ship_hit"):
                            print("Getroffen! Du bist nochmal: \n")
                        else:
                            print("Außerhalb des Spielfelds! Probier es nochmal: \n")

                        guess = input()
                        response = requests.get(f"{base_api_url}/lobby/{cid}/guess/{guess}")

                        # get hit answer
                        time.sleep(1)
                        lobby = requests.get(f"{base_api_url}/lobby").json()
                    

                print("Daneben! Der Gegner ist nun an der Reihe!\n")

                response = requests.get(f"{base_api_url}/stats/current_player/int/{other_player}")

                #Shoot

            # Waiting
            print("Warte bis dein Gegner fertig ist!")
            game_stats = requests.get(f"{base_api_url}/stats").json()
            # Test if current player changed
            while(intern_id != game_stats['current_player'] and not game_stats['game_end']):
                    game_stats = requests.get(f"{base_api_url}/stats").json()
                    lobby = requests.get(f"{base_api_url}/lobby").json()

                    # If guess changed
                    if(lobby[str(lobby['player_list'][other_player])]['guess'] != guess_other_player):
                        guess_other_player = lobby[str(lobby['player_list'][other_player])]['guess']

                        hit = bsb.shoot(int(intern_id), guess_other_player) 
                        all_ships_sunken = bsb.all_ships_sunken(intern_id)
                        board_without_ships = bsb.get_board_as_string_without_ships(intern_id)

                        print("!Debug:")
                        print(board_without_ships)

                        response = requests.get(f"{base_api_url}/lobby/{cid}/board_without_ships/{board_without_ships}")
                        response = requests.get(f"{base_api_url}/lobby/{cid}/all_ships_sunken/{all_ships_sunken}")
                        response = requests.get(f"{base_api_url}/lobby/{cid}/hit/{hit}")

                        # Print game boards
                        print("Dein Spielfeld: \n")
                        print(bsb.get_board_as_string(int(intern_id)))
                        print("Das Spielfeld deines Gegners: \n")
                        print(lobby[str(lobby['player_list'][other_player])]['board_without_ships'])

                    # Client lost the game
                    if (game_stats['game_end']):
                        print("Du hast verloren!")
                        response = requests.get(f"{base_api_url}/lobby/reset_lobby")
                        response = requests.get(f"{base_api_url}/stats/reset_stats")
                        return

        # Except that a player leaves by ctrl + c
        except (KeyboardInterrupt):
        # Leave the game
            leave_question = input("\n Sicher, dass du das Spiel verlassen möchtest? J/N \n")
            if (leave_question == "J"):

                response = requests.get(f"{base_api_url}/leave/{cid}")
                print("Du hast das Spiel verlassen")

                response = requests.get(f"{base_api_url}/stats/host_end/bool/True")

                exit()

    return

def main():
    global base_api_url

    base_api_url = "http://127.0.0.1:8000"
    connect()
    wait_for_start()
    place_ships()
    shooting()
    #end()
        
if __name__ == '__main__':
    main()
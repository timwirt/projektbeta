# Run with
#   uvicorn bs_server:bs_server --port 8000 --reload

import os
from fastapi import FastAPI
from fastapi.responses import RedirectResponse
import uvicorn

# Initialize the game data
json_player = {
                'started': False,
                'player': 0,
                'player_list': []
                }

# Game stats
json_stats = {
                'game_end': False, 
                'host_end': False, 
                'current_player': 0
                }

# Object to config the webservice
bs_server = FastAPI()

'''Lobby paths'''
# Add path to lobby '/lobby/'
@bs_server.get("/lobby")
async def lobby():
    return json_player

# Add path for clients '/lobby/{cid}/{cname}'
@bs_server.get("/join/{cid}/{cname}")
async def lobby(cid: int, cname: str):
    # First player is gamehost
    host = (True if json_player['player'] == 0 else False)

    json_player[cid] = {
                        "host": host,
                        "cid": cid,
                        "intern_id": -1,
                        "cname": cname,
                        "ships": {
                            "Schlachtschiff": 1,
                            "Kreuzer": 1,
                            "Zerstörer": 0,
                            "U-Boot": 0
                        },
                        "board_without_ships": "",
                        "guess": "",
                        "hit": "",
                        "all_ships_sunken": False
                    }
    json_player['player'] += 1
    json_player['player_list'].append(cid)

    # Update Game stats
    json_stats['game_end'] = False
    json_stats['host_end'] = False
    json_stats['current_player'] = 0

    return RedirectResponse(url="/lobby")

# Add path to set board without ships '/lobby/{cid}/board_without_ships/{board_without_ships}'
@bs_server.get("/lobby/{cid}/board_without_ships/{board_without_ships}")
async def set_board_without_ships(cid: int, board_without_ships: str):
    json_player[cid]['board_without_ships'] = board_without_ships

    return RedirectResponse(url="/lobby")

# Add path to set guess '/lobby/{cid}/guess/{guess}'
@bs_server.get("/lobby/{cid}/guess/{guess}")
async def set_guess(cid: int, guess: str):
    json_player[cid]['guess'] = guess

    return RedirectResponse(url="/lobby")

# Add path to set hit '/lobby/{cid}/hit/{hit}'
@bs_server.get("/lobby/{cid}/hit/{hit}")
async def set_hit(cid: int, hit: str):
    json_player[cid]['hit'] = hit

    return RedirectResponse(url="/lobby")

# Add path to set hit '/lobby/{cid}/all_ships_sunken/{hit}'
@bs_server.get("/lobby/{cid}/all_ships_sunken/{all_ships_sunken}")
async def set_all_ships_sunken(cid: int, all_ships_sunken: bool):
    json_player[cid]['all_ships_sunken'] = all_ships_sunken

    return RedirectResponse(url="/lobby")

# Add path to set intern_id after gamestart '/lobby/intern_id/{cid}'
@bs_server.get("/lobby/intern_id/{cid}")
async def set_intern_id(cid: int):
    json_player[cid]['intern_id'] = json_player['player_list'].index(cid)

    return RedirectResponse(url="/lobby")

# Add path to reset host '/lobby/reset_host'
@bs_server.get("/lobby/reset_host")
async def reset_host():
    player_list = json_player['player_list']
    if (len(player_list) > 0):
        new_host_id = player_list[0]
        json_player[new_host_id]['host'] = True

    return RedirectResponse(url="/lobby")

# Add path to set start 'lobby/start/{value}'
@bs_server.get("/lobby/start/{value}")
async def set_start(value: bool):
    json_player['started'] = value
    return RedirectResponse(url="/lobby")

# Add path to set ships 'lobby/{cid}/ships/{ship}'
@bs_server.get("/lobby/{cid}/ships/{ship}")
async def modify_ships(cid: int, ship: str):
    # Decrease the number of the ship by 1
    json_player[cid]['ships'][ship] = json_player[cid]['ships'][ship] - 1

    return RedirectResponse(url="/lobby")

# Add path to reset lobby 'lobby/reset_lobby'
@bs_server.get("/lobby/reset_lobby")
async def reset_lobby():
    global json_player
    json_player = {
                    'started': False,
                    'player': 0,
                    'player_list': []
                    }
    return RedirectResponse(url="/lobby")

'''leave paths'''
# Add path for clients '/leave/{cid}'
@bs_server.get("/leave/{cid}")
async def lobby(cid: int):
    del json_player[cid]
    json_player['player'] -= 1
    json_player['player_list'].remove(cid)

    return RedirectResponse(url="/lobby")

'''stats paths'''
# Add path for stats
@bs_server.get("/stats")
async def stats(): 
    global json_stats
    return json_stats

# Add path for update game stats 
@bs_server.get("/stats/{stat}/{type}/{value}")
async def update_stats(stat: str, type: str, value): 
    # Only on host_end json_player has to be modified
    if (stat == 'host_end'):
        json_stats['host_end'] = True
        json_player = {'player': 0} #reset lobby

    else:
        # Change value to the given type
        match type:
            case "str":
                json_stats[stat] = value
            case "int":
                json_stats[stat] = int(value)
            case "bool":
                json_stats[stat] = bool(value)

    return RedirectResponse(url="/stats")

# Add path for reset game stats 
@bs_server.get("/stats/reset_stats")
async def reset_stats(): 
    global json_stats
    json_stats = {
                    'game_end': False, 
                    'host_end': False, 
                    'current_player': 0
                    }
    return RedirectResponse(url="/stats")



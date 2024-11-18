#include "../include/battleship/battleship_board.hpp"
#include <exception>
#include <iostream>
#include <algorithm>
#include <vector>

namespace BattleShip {

    class BattleShipBoard : public AbstractBattleShipBoard {
    public:
        BattleShipBoard() {
            for(int sign = 0; sign < 10; sign++) {
                for(int number = 0; number < 10; number++) {
                    battleship_board_players[0][sign][number] = 0;
                    battleship_board_players[1][sign][number] = 0;
                }
            }
        }
        bool create_ship(int player_id, std::string ship_type, std::string ship_rotation, std::string ship_head_location) override {
            if(!isValidShipType(ship_type)) return false;
            if(!isValidRotation(ship_rotation)) return false;
            if(!isValidLocation(ship_head_location)) return false;

            int size = ship_type == "U-Boot" ? 2 : ship_type == "Zerstörer" ? 3 : ship_type == "Kreuzer" ? 4 : 5;//Schlachtschiff
            std::vector<int> start_location = get_numbers_from_location(ship_head_location);

            //std::cout << ship_head_location << "=>" << start_location[0] << ":" << start_location[1] << std::endl;//Test
            
            int x_direction = ship_rotation == "South" ? 1 : ship_rotation == "North" ? -1 : 0;
            int y_direction = ship_rotation == "East" ? 1 : ship_rotation == "West" ? -1 : 0;

            if(start_location[0]+(size-1)*x_direction>9 || start_location[0]+(size-1)*x_direction<0
                || start_location[1]+(size-1)*y_direction>9 || start_location[1]+(size-1)*y_direction<0) return false;//outside the board

            // Tests for surrounding Ships
            int x_min = x_direction == -1 ? start_location[0]+(size+1)*x_direction : start_location[0]+(-1);
            int x_max = x_direction == -1 ? start_location[0]+(-1)*x_direction : x_direction == 0 ? x_min+2 : start_location[0]+(size+1)*x_direction;
            int y_min = y_direction == -1 ? start_location[1]+(size+1)*y_direction : start_location[1]+(-1);
            int y_max = y_direction == -1 ? start_location[1]+(-1)*y_direction : y_direction == 0 ? y_min+2 : start_location[1]+(size+1)*y_direction;
            
            //std::cout << x_min << x_max << y_min << y_max << std::endl;//Von bis

            for(int x = x_min; x < x_max; x+=1) {
                for(int y = y_min; y < y_max; y+=1) {
                    if(get_board_info(player_id, x, y) != 0) {
                        return false;
                    }
                }
            }

            // Place the ship
            for(int i = 0; i < size; i++) {
                int x = start_location[0] + i*x_direction;
                int y = start_location[1] + i*y_direction;
                battleship_board_players[player_id][x][y] = 1;//Sets the ShipTyle
            }
            return true;
        }

        std::string shoot(int player_id, std::string location) override {
            if(!isValidLocation(location)) return "exception";

            std::vector<int> loc = get_numbers_from_location(location);
            
            auto hitLocObj = battleship_board_players[player_id][loc[0]][loc[1]];
            switch(hitLocObj) {
                case 0:
                    battleship_board_players[player_id][loc[0]][loc[1]] = 4;
                    return "water_hit";
                case 1:
                    battleship_board_players[player_id][loc[0]][loc[1]] = 2;
                    test_if_ship_is_now_sunken(player_id, loc);
                    return "ship_hit";
                default:
                    return "exception";
            }
        }

        void test_if_ship_is_now_sunken(int player_id, std::vector<int> location) {
            auto left = get_board_info(player_id, location[0], location[1]-1);
            auto right = get_board_info(player_id, location[0], location[1]+1);
            auto up = get_board_info(player_id, location[0]-1, location[1]);
            auto down = get_board_info(player_id, location[0]+1, location[1]);
            bool everyTileHit = true;
            if(isWater(left) && isWater(right)) {// The ship's rotation is north or south
                int y = location[1];
                // Norden testen
                for(int x = location[0]; x >= 0; x--) {
                    if(get_board_info(player_id, x, y) == 0 || get_board_info(player_id, x, y) == 4) {
                        break;
                    } else if(get_board_info(player_id, x, y) == 1) {
                        everyTileHit = false;
                        break;
                    }
                }
                // Süden testen
                for(int x = location[0]; x < 10; x++) {
                    if(get_board_info(player_id, x, y) == 0 || get_board_info(player_id, x, y) == 4) {
                        break;
                    } else if(get_board_info(player_id, x, y) == 1) {
                        everyTileHit = false;
                        break;
                    }
                }

            } else {// The ship's rotation is east or west
                int x = location[0];
                // West testen
                for(int y = location[1]; y >= 0; y--) {
                    if(get_board_info(player_id, x, y) == 0 || get_board_info(player_id, x, y) == 4) {
                        break;
                    } else if(get_board_info(player_id, x, y) == 1) {
                        everyTileHit = false;
                        break;
                    }
                }
                // East testen
                for(int y = location[1]; y < 10; y++) {
                    if(get_board_info(player_id, x, y) == 0 || get_board_info(player_id, x, y) == 4) {
                        break;
                    } else if(get_board_info(player_id, x, y) == 1) {
                        everyTileHit = false;
                        break;
                    }
                }
            }

            if(everyTileHit) {// let's sink the ship :D
                if(isWater(left) && isWater(right)) {// The ship's rotation is north or south
                    int y = location[1];
                    // Norden sinken
                    for(int x = location[0]; x >= 0; x--) {
                        if(get_board_info(player_id, x, y) == 0 || get_board_info(player_id, x, y) == 4) {
                            break;
                        } else if(get_board_info(player_id, x, y) == 2) {
                            battleship_board_players[player_id][x][y] = 3;
                        }
                    }
                    // Süden sinken
                    for(int x = location[0]; x < 10; x++) {
                       if(get_board_info(player_id, x, y) == 0 || get_board_info(player_id, x, y) == 4) {
                            break;
                        } else if(get_board_info(player_id, x, y) == 1) {
                            battleship_board_players[player_id][x][y] = 3;
                        }
                    }

                } else {// The ship's rotation is east or west
                    int x = location[0];
                    // West sinken
                    for(int y = location[1]; y >= 0; y--) {
                        if(get_board_info(player_id, x, y) == 0 || get_board_info(player_id, x, y) == 4) {
                            break;
                        } else if(get_board_info(player_id, x, y) == 2) {
                            battleship_board_players[player_id][x][y] = 3;
                        }
                    }
                    // East sinken
                    for(int y = location[1]; y < 10; y++) {
                        if(get_board_info(player_id, x, y) == 0 || get_board_info(player_id, x, y) == 4) {
                            break;
                        } else if(get_board_info(player_id, x, y) == 2) {
                            battleship_board_players[player_id][x][y] = 3;
                        }
                    }
                }
            }
        }
        /// Helper-Function
        bool isWater(int board_result) {
            return (board_result == 0 || board_result == 4);
        }
        /// Helper-Function to not get Errors if I try to get the Info of fields ouside the board...
        int get_board_info(int player_id, int x, int y) {
            if(x < 0 || x > 9) return 0;
            if(y <0 || y > 9) return 0;
            return battleship_board_players[player_id][x][y];
        }
        std::string get_board_as_string(int player_id) const override {

            std::string board;

            // Add the integers for the board (1-10) for representation of the column
            board += "   ";
            for (int number = 1; number <= 10; number++) {
                board += std::to_string(number) + " ";
            }
            board += "\n";

            // Add the indices for the board (A-J) for representation of the row
            std::string list[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J" };
            for (int sign = 0; sign < 10; sign++) {
                board += " " + list[sign] + " ";
                // Traverse through all board fields and edit their representation by their current state
                for (int number = 0; number < 10; number++) {
                    // The different states of the board customized by their represented unicode character for the user interface
                    char symbol;
                    switch (battleship_board_players[player_id][sign][number]) {
                        case 0:
                            symbol = ' ';
                            break;
                        case 1:
                            symbol = 'X';  // Use 'X' to represent a ship
                            break;
                        case 2:
                            symbol = 'O';  // Use 'O' to represent a hit ship
                            break;
                        case 3:
                            symbol = 'V';  // Use 'V' to represent a sunken ship
                            break;
                        case 4:
                            symbol = 'M';  // Use 'M' to represent a missed shot
                            break;
                            }
                    board += symbol;
                    board += " ";
                }
                board += "\n";
            }

            return board;
        }

        std::string get_board_as_string_without_ships(int player_id) const override {
            
            std::string board;

            // Add the integers for the board (1-10) for representation of the column
            board += "   ";
            for (int number = 1; number <= 10; number++) {
                board += std::to_string(number) + " ";
            }
            board += "\n";

            // Add the indices for the board (A-J) for representation of the row
            std::string list[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J" };
            for (int sign = 0; sign < 10; sign++) {
                board += " " + list[sign] + " ";
                // Traverse through all board fields and edit their representation by their current state
                for (int number = 0; number < 10; number++) {
                    // The different states of the board customized by their represented unicode character for the user interface
                    char symbol;
                    switch (battleship_board_players[player_id][sign][number]) {
                        case 0:
                            symbol = ' ';  // Use ' ' to represent water
                            break;
                        case 1:
                            symbol = ' ';  // Use ' ' to represent a ship
                            break;
                        case 2:
                            symbol = 'O';  // Use 'O' to represent a hit ship
                            break;
                        case 3:
                            symbol = 'V';  // Use 'V' to represent a sunken ship
                            break;
                        case 4:
                            symbol = 'M';  // Use 'M' to represent a missed shot
                            break;
                    }
                    board += symbol;
                    board += " ";
                }
                board += "\n";
            }

            return board;
        }

        bool all_ships_sunken(int player_id) const override {
            for(int x = 0; x < 10; x++) {
                for(int y = 0; y < 10; y++) {
                    auto value = battleship_board_players[player_id][x][y];
                    if(value == 1 || value == 2) {
                        return false;
                    }
                }
            }
            return true;
        }

    private:
        std::vector<int> get_numbers_from_location(std::string location) {
            std::string letter;
            letter.push_back(location[0]);

            std::string number;
            number.push_back(location[2]);
            if(location.length() == 4) {
                number.push_back(location[3]);
            }

            std::vector<int> result = { -1, -1 };
            if(letter == "A") result[0] = 0;
            if(letter == "B") result[0] = 1;
            if(letter == "C") result[0] = 2;
            if(letter == "D") result[0] = 3;
            if(letter == "E") result[0] = 4;
            if(letter == "F") result[0] = 5;
            if(letter == "G") result[0] = 6;
            if(letter == "H") result[0] = 7;
            if(letter == "I") result[0] = 8;
            if(letter == "J") result[0] = 9;

            result[1] = stoi(number)-1;

            return result;
        }
        bool isValidLocation(std::string location) const override {
            if(location.length() != 3 && location.length() != 4) return false;
            std::vector<std::string> list = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J" };
            if((std::find(list.begin(), list.end(), std::to_string(location[0])) != list.end())) return false;//Is the first Element a valid Key?!
            if(location[1] != ':') return false;
            std::string number;
            number.push_back(location[2]);
            if(location.length() == 4) {
                number.push_back(location[3]);
            }
            try {
                int num = stoi(number);
                if(num >= 1 && num <= 10) return true;
            } catch(std::exception e) {
                std::cout << "exception " << std::endl;
                return false;
            };
        }
        bool isValidRotation(std::string rotation) const override {
            if(rotation == "North" || rotation == "East" || rotation == "South" || rotation == "West") return true;
            return false;
        }
        bool isValidShipType(std::string ship_type) const override {
            if(ship_type == "Schlachtschiff" || ship_type == "Kreuzer" || ship_type == "Zerstörer" || ship_type == "U-Boot") return true;
            return false;
        }
    };
}
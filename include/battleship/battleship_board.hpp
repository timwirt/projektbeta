#pragma once

#include <string>

namespace BattleShip
{
    /// @brief An AbstractClass with the methods and attributes needed for a BattleShip-Game.
    class AbstractBattleShipBoard {
    public:
        /// @brief Creates a ship on the board for the given player, ship_type, ship_rotation and ship_head_location.
        /// @param player_id The player_id or rather the client_id as a string. (0,1)
        /// @param ship_type The ship_type as a string. (Schalchtschif, ...)
        /// @param ship_rotation The ship_rotation as a string. (North, East, South, West)
        /// @param ship_head_location The ship_head_location as a string. (A:1 bis J:10)
        /// @return Returns wether the Ship has been created succesfully.
        virtual bool create_ship(int player_id, std::string ship_type, std::string ship_rotation, std::string ship_head_location) = 0;

        /// @brief Shoots at the given location on the board of the given player.
        /// @param player_id The player_id or rather the client_id as a string.
        /// @param location The location as a string.
        /// @return Returns wether the shot has been fired or not.
        virtual std::string shoot(int player_id, std::string location) = 0;
        
        /// @brief Gets the BattleShip-Board whilst only showing the ships of the given player.
        /// @param player_id The player_id or rather the client_id as a string.
        /// @return Returns the Battleship-Board as a string.
        virtual std::string get_board_as_string(int player_id) const = 0;

        /// @brief Gets the BattleShip-Board whilst hiding the ships of the given player.
        /// @param player_id The player_id or rather the client_id as a string.
        /// @return Returns the Battleship-Board without the ships as a string.
        virtual std::string get_board_as_string_without_ships(int player_id) const = 0;

        /// @brief Gets wether all ships are sunken on the board of the given player.
        /// @param player_id The player_id or rather the client_id as a string.
        /// @return Returns the truth value wether all ships are sunken.
        virtual bool all_ships_sunken(int player_id) const = 0;
        
    protected:
        /// @brief Validates wether the input is a valid location.
        /// @param location The location as a string.
        /// @return Returns wether the input is a valid location.
        virtual bool isValidLocation(std::string location) const = 0;
        /// @brief Validates wether the input is a valid rotation.
        /// @param rotation The rotation as a string.
        /// @return Returns wether the input is a valid rotation.
        virtual bool isValidRotation(std::string rotation) const = 0;
        /// @brief Validates wether the input is a valid ship_type.
        /// @param ship_type The ship_type as a string.
        /// @return Returns wether the input is a valid ship_type.
        virtual bool isValidShipType(std::string ship_type) const = 0;

        /// The Board for the two players: 0 means Water, 1 means ShipTile, 2 means a hit ShipTile, 3 means a sunken ShipTile, and 4 means a shot in the Water
        int battleship_board_players[2][10][10];
    };
}

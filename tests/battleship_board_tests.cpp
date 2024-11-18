#include <gtest/gtest.h>
#include "../src/battleship_board.cpp"
#include <iostream>

TEST(ProjektBeta, create_ship_test) {
    BattleShip::BattleShipBoard board;

    bool placed = board.create_ship(0, "U-Boot", "East", "D:4");
    ASSERT_TRUE(placed);

    placed = board.create_ship(0, "Kreuzer", "East", "D:4");//inside the other ship
    ASSERT_FALSE(placed);

    placed = board.create_ship(0, "Kreuzer", "West", "A:10");
    ASSERT_TRUE(placed);
    std::cout << board.get_board_as_string(0) << std::endl;

    placed = board.create_ship(0, "Zerstörer", "South", "A:1");
    ASSERT_TRUE(placed);

    placed = board.create_ship(0, "Schlachtschiff", "North", "H:9");
    ASSERT_TRUE(placed);


    placed = board.create_ship(0, "Kreuzer", "West", "J:2");//outside the board
    ASSERT_FALSE(placed);

    std::cout << board.get_board_as_string(0) << std::endl;
}

TEST(ProjektBeta, shoot_test) {
    BattleShip::BattleShipBoard board;

    bool placed = board.create_ship(0, "U-Boot", "East", "D:4");
    ASSERT_TRUE(placed);
    std::cout << board.get_board_as_string(0) << std::endl;

    std::string shot = board.shoot(0, "D:4");
    ASSERT_EQ(shot, "ship_hit");
    std::cout << board.get_board_as_string(0) << std::endl;

    shot = board.shoot(0, "D:5");
    ASSERT_EQ(shot, "ship_hit");
    std::cout << board.get_board_as_string(0) << std::endl;

    shot = board.shoot(0, "B:3");//WASSERTREFFER
    ASSERT_EQ(shot, "water_hit");

    shot = board.shoot(0, "A:19");
    ASSERT_EQ(shot, "exception");

    std::cout << board.get_board_as_string(0) << std::endl;
}

TEST(ProjektBeta, get_board_as_string_without_ships_test) {
    BattleShip::BattleShipBoard board;
    board.create_ship(0, "U-Boot", "East", "D:4");
    board.create_ship(0, "Kreuzer", "West", "A:10");
    board.shoot(0, "D:4");
    board.shoot(0, "A:10");

    std::cout << board.get_board_as_string_without_ships(0) << std::endl;
}

TEST(ProjektBeta, all_ships_sunken_test) {
    BattleShip::BattleShipBoard board;
    board.create_ship(0, "U-Boot", "East", "D:4");
    board.shoot(0, "D:4");

    ASSERT_FALSE(board.all_ships_sunken(0));

    board.shoot(0, "D:5");

    ASSERT_TRUE(board.all_ships_sunken(0));
}
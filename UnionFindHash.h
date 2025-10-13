#ifndef UnionFindHash_H
#define UnionFindHash_H

#include <cstdlib>
#include <iostream>
#include "HashTable.h"
#include "Team.h"
#include "Player.h"
#include "wet2util.h"

class Player;
class PlayerNode;
class Team;

class UnionFindHash{
private:
    HashTable<Player> players_table;
    int num_of_players;

public:

    UnionFindHash(){}
    ~UnionFindHash();
    void addPlayer(const Player& player, Team* team);
    Player* findPlayer(int playerId);
    Team* findUF(int playerId);
    PlayerNode* findPlayerNode(int playerId);
    int findGamesPlayed(int playerId);
    permutation_t findPartialSpirit(int playerId);
    void unionTeams(PlayerNode* root_player_of_buyer,PlayerNode* root_player_of_bought);
    int getNumGamesOfPlayerCalculated (int player_id);
    void buyTeam(Team* buyer, Team* bought);

    permutation_t get_partial_spirit(int playerId);
    int getGamesForPlayer(Player* player);
    bool checkIfExist(int teamId);

    class Exception : public std::exception {};
    class NotActiveTeam : public Exception {};
};
#endif
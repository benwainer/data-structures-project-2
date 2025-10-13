//
// Created by Ben on 21/12/2022.
//

#ifndef HW2_PLAYER_H
#define HW2_PLAYER_H
#include "Team.h"
#include "UnionFindHash.h"

class Team;
class PlayerNode
{
private:
    int player_id;
    PlayerNode* parent;
    Team* team;

    int games_played;
    permutation_t partial_spirit;
public:
    PlayerNode(int player_id,PlayerNode* parent, int gamesPlayed,const permutation_t& partial);
    PlayerNode(const PlayerNode& player);//copy constructor
    PlayerNode& operator=(const PlayerNode& player);
    virtual ~PlayerNode();
    int getPlayerId() const;
    void setParent(PlayerNode* parent);
    void setPartialSpirit(const permutation_t& additional_spirit);
    permutation_t getPartialSpirit() const;
    PlayerNode* getParent() const;
    Team* getTeam() const;
    void setGamesPlayed(int games);
    int getGamesPlayed() const;
    Team* getPlayerNodeTeam() const;
    void setPlayerNodeTeam(Team* team);

};

class Player {
private:
    int player_id;
    int team_id;
    permutation_t spirit;
    int games_played;
    int ability;
    int cards;
    bool goalkeeper;
    PlayerNode* himself;
public:
    Player();
    Player(int playerId);
    Player(int playerId, int teamId,const permutation_t& spirit,int gamesPlayed,
           int ability,  int cards,bool goalKeeper
           );
    Player(const Player& player);//copy constructor
    ~Player();
    Player& operator=(const Player& player);
    bool operator==(const Player& player) const;
    bool operator!=(const Player& player) const;
    bool operator<(const Player& player) const;
    bool operator>(const Player& player) const;
    int getCards() const;
    int getPlayerId() const;
    int getTeamId() const;
    permutation_t getSpirit() const;
    bool isGoalkeeper() const;
    int getNumPlayedGames() const;
    int getAbility() const;
    void addCards(int cards);
    int operator% (int n) const;
    PlayerNode* getPlayerNode() const;
    int getGamesOfPlayerCalculated(PlayerNode* root_player);
    void setHimselfPointer(PlayerNode* himself);

};




#endif //HW2_PLAYER_H

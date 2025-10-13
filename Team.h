//
// Created by Ben on 21/12/2022.
//

#ifndef HW2_TEAM_H
#define HW2_TEAM_H
#include "wet2util.h"
#include "Player.h"

class Player;
class PlayerNode;

class Team {
private:
    int team_id;
    int points;
    int num_of_matches;
    int num_of_players;
    int num_of_goal_keepers;
    permutation_t team_spirit;
    int players_abilities;
    bool byId;
    PlayerNode* players_root;
public:

    Team();
    Team(int team_id);
    Team(const Team &other);
    Team& operator=(const Team& other);
    ~Team();
    int getTeamId() const ;
    int getNumOfPlayers() const;
    int getTeamValue() const;
    permutation_t getTeamSpirit() const;
    int getTeamStrength() const;
    int getTeamPoints() const;
    int getNumOfMatches() const;
    int getNumOfGoalKeepers() const;
    int getPlayersAbilities() const;
    PlayerNode* getPlayersRoot() const;
    void setTeamId(int id);
    void setByAbilities();
    void setPlayersRoot(PlayerNode* player_node);
    void setNullForPlayersRoot();


    bool operator<(const Team &other) const;

    bool operator>(const Team &other) const;

    bool operator==(const Team &other) const;
    void addPoints(int num);

    void addPlayer(Player* player);

    void addMatches(int num);

    void updateTeamSpirit(const Team &team);
    void setTeamSpirit(const permutation_t& p);

    void uniteValues(Team* team);

};
#endif //HW2_TEAM_H

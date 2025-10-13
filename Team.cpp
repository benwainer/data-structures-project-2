//
// Created by Naomi on 12/21/2022.
//
#include "Team.h"

Team::Team() : team_id(-1), points(0) ,num_of_matches(0), num_of_players(0), num_of_goal_keepers(0),
        team_spirit(permutation_t().neutral()), players_abilities(0),  byId(true), players_root(nullptr){}

Team::Team(int team_id) : team_id(team_id), points(0) ,num_of_matches(0), num_of_players(0),
num_of_goal_keepers(0), team_spirit(permutation_t().neutral()), players_abilities(0), byId(true), players_root(nullptr){}

Team::Team(const Team &other) :team_id(other.team_id), points(other.points),num_of_matches(other.num_of_matches),
num_of_players(other.num_of_players), num_of_goal_keepers(other.num_of_goal_keepers),
team_spirit(other.team_spirit),players_abilities(other.players_abilities), byId(other.byId), players_root(other.players_root){}

Team& Team::operator=(const Team& other)
{
    if(&other == this){
        return *this;
    }
    team_id = other.team_id;
    points = other.points;
    num_of_matches = other.num_of_matches;
    num_of_players = other.num_of_players;
    num_of_goal_keepers = other.num_of_goal_keepers;
    team_spirit = other.team_spirit;
    players_abilities = other.players_abilities;
    byId = other.byId;
    players_root = other.players_root;
    return *this;
}
Team::~Team()
{
    team_id = -1;
    points = -1;
    num_of_matches = -1;
    num_of_players = -1;
    num_of_goal_keepers = -1;
    players_abilities = -1;
    team_spirit.invalid();
    players_root = nullptr;
}
bool Team::operator<(const Team &other) const
{
    if(!byId){
        if(players_abilities < other.players_abilities){
            return true;
        }
        else if(players_abilities > other.players_abilities){
            return false;
        }
    }
    return team_id < other.team_id;
}
bool Team::operator>(const Team &other) const
{
    if(!byId){
        if(players_abilities > other.players_abilities){
            return true;
        }
        else if(players_abilities < other.players_abilities){
            return false;
        }
    }
    return team_id > other.team_id;
}
bool Team::operator==(const Team &other) const
{
    return team_id == other.team_id;
}

int Team::getTeamId() const
{
    return team_id;
}
int Team::getNumOfPlayers() const
{
    return num_of_players;
}
int Team::getTeamValue() const
{
    return points + players_abilities;
}
permutation_t Team::getTeamSpirit() const
{
    return team_spirit;
}
int Team::getTeamStrength() const
{
    return team_spirit.strength();
}
int Team::getTeamPoints() const
{
    return points;
}
int Team::getNumOfMatches() const
{
    return num_of_matches;
}
int Team::getNumOfGoalKeepers() const
{
    return num_of_goal_keepers;
}

void Team::setTeamId(int id)
{
    team_id = id;
}
void Team::setByAbilities()
{
    byId = false;
    players_root = nullptr;
}

void Team::addPoints(int num)
{
    points+=num;
}
void Team::addMatches(int num)
{
    num_of_matches+=num;
    if(players_root!= nullptr)
    {
        players_root->setGamesPlayed(players_root->getGamesPlayed()+num);
    }
}
void Team::addPlayer(Player* player)
{
    if(player->isGoalkeeper()){
        num_of_goal_keepers++;
    }
    num_of_players++;
    players_abilities += player->getAbility();
    team_spirit = team_spirit*player->getSpirit();
}
void Team::uniteValues(Team* team)
{
    points+=team->points;
    num_of_players+=team->num_of_players;
    num_of_goal_keepers+=team->num_of_goal_keepers;
    players_abilities+=team->players_abilities;
}
void Team::updateTeamSpirit(const Team &team)
{
    team_spirit = team_spirit*(team.team_spirit);
}

void Team::setNullForPlayersRoot()
{
    if(players_root != nullptr){
        players_root->setPlayerNodeTeam(nullptr);
    }
}

void Team::setPlayersRoot(PlayerNode *player_node)
{
    players_root = player_node;
}
int Team::getPlayersAbilities() const
{
    return players_abilities;
}
PlayerNode* Team:: getPlayersRoot() const
{
    return players_root;
}

void Team::setTeamSpirit(const permutation_t& p) {
    team_spirit = p;
}




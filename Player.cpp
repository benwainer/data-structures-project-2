//
// Created by Ben on 21/12/2022.
//
#include "Player.h"

Player::Player() :player_id(-1), team_id(-1), spirit(), games_played(-1),
                  ability(-1), cards(-1), goalkeeper(false),  himself(nullptr){}
Player:: Player(int playerId):
        player_id(playerId), team_id(-1), spirit(), games_played(-1),
        ability(-1), cards(-1), goalkeeper(false),  himself(nullptr)
{}
Player::Player(int playerId, int teamId,const permutation_t& spirit,int gamesPlayed,
              int ability,  int cards,bool goalKeeper
               ):
        player_id(playerId), team_id(teamId), spirit(spirit), games_played(gamesPlayed),
       ability(ability),
        cards(cards), goalkeeper(goalKeeper),  himself(nullptr)
{}
Player::Player(const Player& player):
        player_id(player.player_id), team_id(player.team_id), spirit(player.spirit), games_played(player.games_played),
        ability(player.ability),
        cards(player.cards), goalkeeper(player.goalkeeper),  himself(player.himself)
{}
Player& Player::operator=(const Player& player)
{
    if(&player == this){
        return *this;
    }
    player_id = player.player_id;
    team_id = player.team_id;
    spirit = player.spirit;
    games_played = player.games_played;
    ability = player.ability;
    cards = player.cards;
    goalkeeper = player.goalkeeper;
    himself = player.himself;
    return *this;
}
Player::~Player()
{
    if(himself != nullptr){
        delete himself;
    }
    player_id = -1;
    himself = nullptr;
}
bool Player:: operator==(const Player& player) const
{
    return (player_id==player.player_id);
}
bool Player:: operator!=(const Player& player) const
{
    return player_id!= player.player_id;
}
bool Player:: operator<(const Player& player) const
{
    return player_id<player.player_id;
}
bool Player:: operator>(const Player& player) const
{
    return player_id>player.player_id;
}
int Player:: getCards() const
{
    return cards;
}
int Player:: getTeamId() const
{
    return team_id;
}
permutation_t Player::getSpirit() const
{
    return spirit;
}
bool Player:: isGoalkeeper() const
{
    return goalkeeper;
}
int Player:: getNumPlayedGames() const
{
    return games_played;
}
void Player:: addCards(int cards)
{
    this->cards+=cards;
}

int Player::operator% (int n) const
{
    return player_id%n;
}
PlayerNode* Player::getPlayerNode() const
{
    return himself;
}
int Player::getPlayerId() const {
    return player_id;
}
int Player::getAbility() const {
    return ability;
}

void Player::setHimselfPointer(PlayerNode *himself) {
    this->himself = himself;

}
PlayerNode:: PlayerNode(int player_id,PlayerNode* parent,int games,const permutation_t& partial):
player_id(player_id),parent(parent),team(nullptr), games_played(games), partial_spirit(partial)
{}

PlayerNode &PlayerNode::operator=(const PlayerNode &player)
{
    if(this == &player)
    {
        return *this;
    }
    player_id = player.player_id;
    parent = player.parent;
    team = player.team;
    games_played = player.games_played;
    partial_spirit = player.partial_spirit;
    return *this;
}

void PlayerNode::setParent(PlayerNode *parent)
{
    this->parent=parent;
}
void PlayerNode::setPartialSpirit(const permutation_t& partial_spirit)
{
    this->partial_spirit=partial_spirit;
}
PlayerNode* PlayerNode::getParent() const
{
    return parent;
}
permutation_t PlayerNode::getPartialSpirit() const
{
    return partial_spirit;
}

void PlayerNode::setGamesPlayed(int games)
{
    this->games_played=games;
}
int PlayerNode::getGamesPlayed() const
{
    return games_played;
}
void PlayerNode::setPlayerNodeTeam(Team* team)
{
    this->team=team;
}
Team* PlayerNode::getPlayerNodeTeam() const
{
    return team;
}

PlayerNode::~PlayerNode()
{
    parent = nullptr;
    team = nullptr;
    partial_spirit.invalid();
}

int PlayerNode::getPlayerId() const {
    return player_id;
}

Team *PlayerNode::getTeam() const {
    return team;
}

PlayerNode::PlayerNode(const PlayerNode &player) : player_id(player.player_id),
parent(player.parent), team(player.team), games_played(player.games_played),
partial_spirit(player.partial_spirit)
{}




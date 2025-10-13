//
// Created by Naomi on 12/21/2022.
//
#include "UnionFindHash.h"

void UnionFindHash::addPlayer(const Player& player, Team* team)
{
    players_table.insert(player); //exception if already exist
    Player* player_ptr = players_table.find(player);
    int player_id = player_ptr->getPlayerId();
    team->addPlayer(player_ptr); //update values
    PlayerNode* players_root = team->getPlayersRoot();
    int gamesPlayed = player_ptr->getNumPlayedGames();
    permutation_t partial = player_ptr->getSpirit();
    if(players_root != nullptr){
        gamesPlayed -= players_root->getGamesPlayed();
        partial = players_root->getPartialSpirit().inv()*team->getTeamSpirit();
    }
    PlayerNode* to_add = new PlayerNode(player_id,players_root, gamesPlayed, partial);
    if(players_root == nullptr){
        to_add->setPlayerNodeTeam(team);
        team->setPlayersRoot(to_add);
    }
    player_ptr->setHimselfPointer(to_add);
    num_of_players++;
}
Player* UnionFindHash::findPlayer(int playerId)
{
    Player tmp(playerId);
    Player* player_ptr = players_table.find(tmp);
    return player_ptr;
}
Team *UnionFindHash::findUF(int playerId)
{
    return findPlayerNode(playerId)->getTeam();
}

PlayerNode *UnionFindHash::findPlayerNode(int playerId)
{
    Player* player_ptr = findPlayer(playerId);
    PlayerNode* player_node = player_ptr->getPlayerNode();
    PlayerNode* parent = player_node->getParent();
    if(parent == nullptr){
        return player_node;
    }
    int partial_sum = 0, sum = 0;
    sum += player_node->getGamesPlayed();
    permutation_t product_of_spirits = player_node->getPartialSpirit();
    while(parent->getParent() != nullptr){
        sum += parent->getGamesPlayed();
        product_of_spirits = parent->getPartialSpirit()*product_of_spirits;
        parent = parent->getParent();
    }
    int old_games_played = 0;
    partial_sum = sum;
    PlayerNode* tmp = nullptr;
    permutation_t old_partial = permutation_t().neutral();
    while(player_node != parent){
        old_games_played = player_node->getGamesPlayed();
        player_node->setGamesPlayed(partial_sum);
        partial_sum -= old_games_played;
        old_partial = player_node->getPartialSpirit();
        player_node->setPartialSpirit(product_of_spirits);
        product_of_spirits = product_of_spirits*old_partial.inv();
        tmp = player_node->getParent();
        player_node->setParent(parent);
        //player_node->setPlayerNodeTeam(parent->getTeam());
        player_node = tmp;
    }
    //in this point player_node == parent
    return parent;
}
int UnionFindHash::findGamesPlayed(int playerId)
{
    Player* player_ptr = findPlayer(playerId);
    PlayerNode* players_root = findPlayerNode(playerId);
    PlayerNode* player_node = player_ptr->getPlayerNode();
    PlayerNode* parent = player_node->getParent();
    int sum = 0;
    sum += player_node->getGamesPlayed();
    while(parent != nullptr){
        sum += parent->getGamesPlayed();
        parent = parent->getParent();
    }
    return sum;
}
permutation_t UnionFindHash::findPartialSpirit(int playerId)
{
    Player* player_ptr = findPlayer(playerId);
    Team* players_team = findUF(playerId);
    if(players_team == nullptr){
        throw NotActiveTeam();
    }
    PlayerNode* player_node = player_ptr->getPlayerNode();
    PlayerNode* parent = player_node->getParent();
    permutation_t partial = player_node->getPartialSpirit();
    while(parent != nullptr){
        partial = parent->getPartialSpirit()*partial;
        parent = parent->getParent();
    }
    return partial;
}
void UnionFindHash::unionTeams(PlayerNode* root_player_of_buyer,PlayerNode* root_player_of_bought) {

    if (root_player_of_buyer == root_player_of_bought || root_player_of_bought == nullptr || root_player_of_buyer ==
    nullptr) {
        return;
    }
    Team *buyer = root_player_of_buyer->getPlayerNodeTeam();
    Team *bought = root_player_of_bought->getPlayerNodeTeam();

    if (buyer->getNumOfPlayers() >= bought->getNumOfPlayers() || (buyer->getNumOfPlayers() == 0 && bought->getNumOfPlayers() == 0)) {
        root_player_of_bought->setParent(root_player_of_buyer);
        root_player_of_bought->setGamesPlayed(root_player_of_bought->getGamesPlayed()-root_player_of_buyer->getGamesPlayed());
        root_player_of_bought->setPartialSpirit(root_player_of_buyer->getPartialSpirit().inv()
        *root_player_of_buyer->getTeam()->getTeamSpirit()*root_player_of_bought->getPartialSpirit());
    }
    else {
        root_player_of_buyer->setParent(root_player_of_bought);
        root_player_of_buyer->setGamesPlayed(root_player_of_buyer->getGamesPlayed()-root_player_of_bought->getGamesPlayed());
        root_player_of_bought->setPartialSpirit(root_player_of_buyer->getTeam()->getTeamSpirit()*root_player_of_bought->getPartialSpirit());
        root_player_of_buyer->setPartialSpirit(root_player_of_bought->getPartialSpirit().inv()*root_player_of_buyer->getPartialSpirit());
    }
}
int UnionFindHash::getNumGamesOfPlayerCalculated (int player_id)
{
    return findGamesPlayed(player_id);
}
permutation_t UnionFindHash::get_partial_spirit(int playerId)
{
    return findPartialSpirit(playerId);
}

void UnionFindHash::buyTeam(Team* buyer, Team* bought)
{
    PlayerNode* buyer_root = buyer->getPlayersRoot();
    PlayerNode* bought_root = bought->getPlayersRoot();
    if(buyer_root == nullptr && bought_root == nullptr){
        return;
    }
    unionTeams(buyer_root, bought_root); // uf union function
    PlayerNode* updated_players_root = nullptr;
    if(buyer->getNumOfPlayers() >= bought->getNumOfPlayers()){
        updated_players_root = buyer_root;
    }
    else {
        updated_players_root = bought_root;
    }
    buyer->setTeamSpirit(buyer->getTeamSpirit()*bought->getTeamSpirit());
    updated_players_root->setPlayerNodeTeam(buyer);
    buyer->setPlayersRoot(updated_players_root);
}

UnionFindHash::~UnionFindHash() {}




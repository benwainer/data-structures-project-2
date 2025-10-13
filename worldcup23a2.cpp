#include "worldcup23a2.h"

#define WIN_POINTS 3
#define DRAW_POINTS 1
#define FIRST_TEAM_WON_BY_ABILITY 1
#define SECOND_TEAM_WON_BY_ABILITY 3
#define FIRST_TEAM_WON_BY_SPIRIT 2
#define SECOND_TEAM_WON_BY_SPIRIT 4
#define DRAW 0

world_cup_t::world_cup_t()
{
    num_of_teams = 0;
}

world_cup_t::~world_cup_t()
{
	// TODO: Your code goes here
}

StatusType world_cup_t::add_team(int teamId)
{
    try{
        if(teamId <= 0){
            return StatusType::INVALID_INPUT;
        }
        Team to_add(teamId);
        all_teams_by_id.insert(to_add);
        Team to_add_by_abilities(to_add);
        to_add_by_abilities.setByAbilities();
        all_teams_by_abilities.insert(to_add_by_abilities);
        num_of_teams++;
    }
    catch(std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    catch(...){
        return StatusType::FAILURE;
    }
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if(teamId <= 0){
        return StatusType::INVALID_INPUT;
    }
    try{
        Team tmp(teamId);
        Team* team_ptr = all_teams_by_id.find(tmp);
        team_ptr->setNullForPlayersRoot();
        Team team_by_abilities(*team_ptr);
        team_by_abilities.setByAbilities();
        all_teams_by_abilities.remove(team_by_abilities);
        all_teams_by_id.remove(tmp);
        num_of_teams--;
    }
    catch(std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    catch(...){
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

StatusType world_cup_t::add_player(int playerId, int teamId,
                                   const permutation_t &spirit, int gamesPlayed,
                                   int ability, int cards, bool goalKeeper)
{
    if(playerId <= 0 || teamId <= 0 || !spirit.isvalid() || gamesPlayed < 0 || cards < 0){
        return StatusType::INVALID_INPUT;
    }
    try{
        Team tmp_team(teamId);
        Team* players_team = all_teams_by_id.find(tmp_team);
        Team players_team_by_abilities(*players_team);
        players_team_by_abilities.setByAbilities();
        permutation_t team_spirit = players_team->getTeamSpirit();
        Player player_to_add(playerId, teamId, spirit, gamesPlayed,
                             ability, cards, goalKeeper);
        all_players.addPlayer(player_to_add, players_team); //exception if does not exist
        Team* players_team_by_abilities_ptr = all_teams_by_abilities.find(players_team_by_abilities);
        Team copy(*players_team_by_abilities_ptr);
        copy.setByAbilities();
        all_teams_by_abilities.remove(players_team_by_abilities);
        copy.addPlayer(&player_to_add);
        all_teams_by_abilities.insert(copy);
    }
    catch(std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    catch(...){
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::play_match(int teamId1, int teamId2)
{
    if(teamId1<=0 || teamId2<=0 || teamId1==teamId2) {
        output_t<int> output1(StatusType::INVALID_INPUT);
        return output1;
    }
    try {
        Team tmp_team1(teamId1);
        Team tmp_team2(teamId2);
        Team *team1 = all_teams_by_id.find(tmp_team1);
        Team *team2 = all_teams_by_id.find(tmp_team2);
        if (team1->getNumOfGoalKeepers() < 1 || team2->getNumOfGoalKeepers() < 1) {
            output_t<int> output1(StatusType::FAILURE);
            return output1;
        }
        Team tmp_team1_by_abilities(*team1);
        tmp_team1_by_abilities.setByAbilities();
        Team* team1_by_abilities = all_teams_by_abilities.find(tmp_team1_by_abilities);
        Team tmp_team2_by_abilities(*team2);
        tmp_team2_by_abilities.setByAbilities();
        Team* team2_by_abilities = all_teams_by_abilities.find(tmp_team2_by_abilities);
        team1->addMatches(1);
        team1_by_abilities->addMatches(1);
        team2->addMatches(1);
        team2_by_abilities->addMatches(1);
        if (team1->getTeamValue() > team2->getTeamValue()) {
            team1->addPoints(WIN_POINTS);
            team1_by_abilities->addPoints(WIN_POINTS);
            output_t<int> output1(FIRST_TEAM_WON_BY_ABILITY);
            return output1;
        }
        if (team2->getTeamValue() > team1->getTeamValue()) {
            team2->addPoints(WIN_POINTS);
            team2_by_abilities->addPoints(WIN_POINTS);
            output_t<int> output1(SECOND_TEAM_WON_BY_ABILITY);
            return output1;
        }
        if (team1->getTeamStrength() > team2->getTeamStrength()) {
            team1->addPoints(WIN_POINTS);
            team1_by_abilities->addPoints(WIN_POINTS);
            output_t<int> output1(FIRST_TEAM_WON_BY_SPIRIT);
            return output1;
        }
        if (team2->getTeamStrength() > team1->getTeamStrength()) {
            team2->addPoints(WIN_POINTS);
            team2_by_abilities->addPoints(WIN_POINTS);
            output_t<int> output1(SECOND_TEAM_WON_BY_SPIRIT);
            return output1;
        }
        team1->addPoints(DRAW_POINTS);
        team1_by_abilities->addPoints(DRAW_POINTS);
        team2->addPoints(DRAW_POINTS);
        team2_by_abilities->addPoints(DRAW_POINTS);
        output_t<int> output1(DRAW);
        return output1;
    }catch(std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }catch (...)
    {
        output_t<int> output1(StatusType::FAILURE);
        return output1;
    }
}
output_t<int> world_cup_t::num_played_games_for_player(int playerId)
{
    if(playerId <= 0){
        output_t<int> output1(StatusType::INVALID_INPUT);
        return output1;
    }
    try{
        int result = all_players.getNumGamesOfPlayerCalculated(playerId);
        output_t<int> output1(result);
        return output1;
    }catch(std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }catch (...) {
        output_t<int> output1(StatusType::FAILURE);
        return output1;
    }

}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
    if(playerId <= 0 || cards < 0){
        return StatusType::INVALID_INPUT;
    }
    try{
        Player* player= all_players.findPlayer(playerId);
        Team* players_team = all_players.findUF(playerId);
        if(players_team == nullptr)
        {
            return StatusType::FAILURE;
        }
        player->addCards(cards);
    }
    catch(std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    catch(...){
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}
output_t<int> world_cup_t::get_player_cards(int playerId) {
    if (playerId <= 0) {
        output_t<int> output1(StatusType::INVALID_INPUT);
        return output1;
    }
    try
    {
        Player* player = all_players.findPlayer(playerId);
        output_t<int> output1(player->getCards());
        return output1;
    }
    catch (std::bad_alloc &e) {
        output_t<int> output1(StatusType::ALLOCATION_ERROR);
        return output1;
    }
    catch (...) {
        output_t<int> output1(StatusType::FAILURE);
        return output1;
    }
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if(teamId <= 0){
        output_t<int> output1(StatusType::INVALID_INPUT);
        return output1;
    }
    try{
        Team tmp(teamId);
        Team* team_ptr = all_teams_by_id.find(tmp);
        output_t<int> output1(team_ptr->getTeamPoints());
        return output1;

    }catch (std::bad_alloc &e) {
        output_t<int> output1(StatusType::ALLOCATION_ERROR);
        return output1;
    }
    catch (...) {
        output_t<int> output1(StatusType::FAILURE);
        return output1;
    }
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i)
{
    if(i < 0 || i >= num_of_teams){
        output_t<int> output1(StatusType::FAILURE);
        return output1;
    }
    try {
        Team *team_ptr = all_teams_by_abilities.select(i);
        output_t<int> output1(team_ptr->getTeamId());
        return output1;
    }catch (std::bad_alloc &e) {
        output_t<int> output1(StatusType::ALLOCATION_ERROR);
        return output1;
    }
    catch (...) {
        output_t<int> output1(StatusType::FAILURE);
        return output1;
    }
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId)
{
    if(playerId <= 0){
        output_t<permutation_t> output1(StatusType::INVALID_INPUT);
        return output1;
    }
    try {
        permutation_t result = all_players.get_partial_spirit(playerId);
        output_t<permutation_t>output1 (result);
        return output1;
    }
    catch (std::bad_alloc &e) {
        output_t<permutation_t> output1(StatusType::ALLOCATION_ERROR);
        return output1;
    }
    catch (...) {
        output_t<permutation_t> output1(StatusType::FAILURE);
        return output1;
    }
}
StatusType world_cup_t::buy_team(int teamId1, int teamId2)
{
    if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2){
        return StatusType::INVALID_INPUT;
    }
    try{
        Team tmp_buyer(teamId1);
        Team tmp_bought(teamId2);
        Team* buyer_ptr = all_teams_by_id.find(tmp_buyer);
        Team* bought_ptr = all_teams_by_id.find(tmp_bought);
        all_players.buyTeam(buyer_ptr,bought_ptr);
        Team tmp_buyer_by_abilities(*buyer_ptr);
        Team tmp_bought_by_abilities(*bought_ptr);
        tmp_buyer_by_abilities.setByAbilities();
        tmp_bought_by_abilities.setByAbilities();

        buyer_ptr->uniteValues(bought_ptr);
        Team updated_buyer_copy(*buyer_ptr);
        updated_buyer_copy.setByAbilities();
        all_teams_by_id.remove(*bought_ptr);
        all_teams_by_abilities.remove(tmp_bought_by_abilities);
        all_teams_by_abilities.remove(tmp_buyer_by_abilities);
        all_teams_by_abilities.insert(updated_buyer_copy);
        num_of_teams--;

    } catch(std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    catch(...){
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

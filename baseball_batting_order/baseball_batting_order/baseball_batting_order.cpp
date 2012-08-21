// baseball_batting_order.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <stdlib.h>

using namespace std;

const int arr_size=15;

void print_player_info (float player_stats[][18], string player_names[]){
	cout << "Player AB BB H 1B 2B 3B HR RBI Runs SF HBP K SB AVG OBP SLG OPS" << endl;
	for (int k=0; k<15; k++){
		int player_key;
		player_key=player_stats[k][0];
		cout << player_names[player_key] << " ";
		for (int l=0; l<18; l++){
			//if (l>0){
				cout << player_stats[k][l] << " ";
			//}
		}
		cout << endl;
	}
}

void sort(float array[][18], int stat)
{
	float max=array[0][stat];
	//int prev_max_idx=0;
    for(int i=0;i<arr_size;i++)
    {
		for(int j=0;j<arr_size;j++){
		   if(array[i][stat]>array[j][stat])
			{
				max=array[j][stat];
				//swap rows
				for (int k=0;k<18;k++){
					float temp=array[j][k];
					array[j][k]=array[i][k];
					array[i][k]=temp;
				}
				//prev_max_idx=i;
			}
		}
    }
	
}

int search_array(int batting_array[], int key){
	for (int i=0; i<arr_size; i++){
		if (batting_array[i]==key) {
			//player is already in batting order, try next value
			return 1;
			break;
		}
	}
	return 0;
}

void get_keys(float array[][18], int batting_order[], int keys[]){
	int count=0;
	int player_found;

	for (int i=0; i<arr_size; i++){
		int curr_key=array[i][0];
		player_found = search_array(batting_order, curr_key);
		if (!player_found){
			keys[count]=curr_key;
			count=count+1;
		}

		//only populating positions 1 and 2 in the batting order currently
		if (count==2){
			break;
		}
	}
}

int main() {
    
    ifstream team_data("dodgers_stats.txt", ios::in);  // declarations of streams fp_in and fp_out
	stringstream ss;
	char buff[100];
	float player_data[arr_size][18];
	string player_name[arr_size];
	int batting_order[arr_size];
	int obp_keys[2], sb_keys[2], avg_keys[2];
	int pos_set=0;
	int rem_batting_start_pos=5; //this is the batting position from which the avg sort will start filling values
    ofstream batting_order_file;   
     
	//take in team data
    for (int i=0; i<15;i++){
		team_data.getline(buff, 100);
		ss << buff;
		for (int j=0; j<18; j++){
			ss.getline( buff, 10, ',');
			if (j==0) {
				player_data[i][0]=i; //assign a key value to make indexing the names array later simple
				player_name[i]=buff;
			}
			else{
				player_data[i][j]=atof(buff);
			}
			
		}
		ss << "";
		ss.clear();
	}

	
	//set the batting order based on the players statistics	
	//list is currently sorted largest to smallest on ops, this means 1,2,3 of sorted list should be 3,4,5 in batting order
	//place player keys in batting order array first, when fully populated, use this array to select and print name
	sort(player_data, 17);
	//heart of order always has high .ops so there's no need for comparison to other stats
	batting_order[2] = player_data[0][0];
	batting_order[3] = player_data[1][0];
	batting_order[4] = player_data[2][0];

	//resort the data and get player keys
	sort(player_data, 15); //sort by obp
	get_keys (player_data, batting_order, obp_keys);
	sort(player_data, 13); //sort by sb
	get_keys (player_data, batting_order, sb_keys);
	sort(player_data, 14); //sort by avg
	get_keys (player_data, batting_order, avg_keys);

	//set position 1 in the batting order
	//need to check and see if the sb leaders appear in a high obp or avg position, if so they should lead off
	for (int i=0; i<2; i++){
		for (int j=0;j<2;j++){
			if ((sb_keys[i]==obp_keys[j]) || (sb_keys[i]==avg_keys[j])){
				batting_order[0]=sb_keys[i];
				pos_set=1;
			}
		}
		if (pos_set==1)
			break;
	}
	//if the leadoff position was not set, choose the guy who has the highest avg between the 2 SB leaders
	if (pos_set==0){
		//sort data back to original ordering
		sort(player_data, 0);
		if (player_data[sb_keys[0]][14] > player_data[sb_keys[1]][14]){
			batting_order[0]=sb_keys[0];
		}
		else{
			batting_order[0]=sb_keys[1];
		}
	}

	
	//set position 2 in the batting order
	//need to check and see if the he appears as one of the 2 remaining high obp leaders, if so they should lead be placed in the 2nd position
	pos_set=0;
	for (int i=0; i<2; i++){
		if ((avg_keys[i]==obp_keys[0]) || (avg_keys[i]==obp_keys[1])){
			batting_order[1]=avg_keys[i];
			pos_set=1;
		}
		if (pos_set==1)
			break;
	}
	//if the 2nd position was not set, choose the guy who has the highest avg
	if (pos_set==0){
		//sort data back to original ordering
		sort(player_data, 0);
		if (player_data[avg_keys[0]][14] > player_data[avg_keys[1]][14]){
			batting_order[1]=avg_keys[0];
		}
		else{
			batting_order[1]=avg_keys[1];
		}
	}

	//remainder of the batting order will be filled in by batting average, the ops algorithm could extend into the 6th position for the batting order
	//further refinement would look at the avg as well as speed and place accordingly in the remaining batting positions
	
	sort(player_data, 14); //sort by avg
	for (int i=0; i<arr_size; i++){
		int player_found;
		int curr_key=player_data[i][0];
		player_found = search_array(batting_order, curr_key);
		if (!player_found){
			batting_order[rem_batting_start_pos]=curr_key;
			rem_batting_start_pos = rem_batting_start_pos + 1;
		}
	}
		
	//now that the batting order is set, write it out to a text file
	batting_order_file.open("batting_order.txt", ios::out);

	for (int i=0; i<arr_size; i++){
		batting_order_file << i+1 << ". " << player_name[batting_order[i]] << endl;
	}
	// close the streams
	batting_order_file.close();
    team_data.close();   
    return 0;
}


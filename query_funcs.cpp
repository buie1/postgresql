#include "query_funcs.h"


void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{

	ostringstream insrt;
	string sql;
  	string sql_in = "INSERT INTO PLAYER (TEAM_ID,UNIFORM_NUM,FIRST_NAME,LAST_NAME,MPG,PPG,RPG,APG,SPG,BPG) ";
	nontransaction W(*C);
	insrt << "VALUES (" << team_id << ", " << jersey_num << ", '" \
		<< first_name << "', '" << last_name << "', " << mpg << ", " << ppg << ", " << rpg \
		<< ", " << apg << ", " << spg << ", " << bpg << " );";
		sql = sql_in + insrt.str();
		//cout << "sql cmd:"<< endl << sql << endl;
		W.exec ( sql );
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
	ostringstream insrt;
	string sql;
  	string sql_in = "INSERT INTO TEAM (NAME,STATE_ID,COLOR_ID,WINS,LOSSES) ";
  	nontransaction W(*C);
	insrt << "VALUES ('" << name << "', " << state_id << ", " \
		<< color_id << ", " << wins << ", " << losses << " );";
	sql = sql_in + insrt.str();
	//cout << "sql cmd:"<< endl << sql << endl;
	W.exec ( sql );
}


void add_state(connection *C, string name)
{
	ostringstream insrt;
	string sql;
  	string sql_in = "INSERT INTO STATE (NAME) ";
  	nontransaction W(*C);
	insrt << "VALUES ('" << name << "' );";
	sql = sql_in + insrt.str();
	//cout << "sql cmd:"<< endl << sql << endl;
	W.exec ( sql );
}


void add_color(connection *C, string name)
{
	ostringstream insrt;
	string sql;
  	string sql_in = "INSERT INTO COLOR (NAME) ";
  	nontransaction W(*C);
	insrt << "VALUES ('" << name << "' );";
	sql = sql_in + insrt.str();
	//cout << "sql cmd:"<< endl << sql << endl;
	W.exec ( sql );
}


void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
	bool useAnd = false;
	ostringstream wStr;
	wStr <<  "SELECT * from PLAYER ";
	if (use_mpg) {
		wStr << "where ( MPG between " << min_mpg << " AND " << max_mpg << ")";
		useAnd = true;
	}
	if (use_ppg) {
		if(useAnd){
			wStr << " AND ";
		}else{
			wStr << "where ";
		}
		wStr << "( PPG between " << min_ppg << " AND " << max_ppg << ")";
		useAnd = true;
	}
	if (use_rpg) {
		if(useAnd){
			wStr << " AND ";
		}else{
			wStr << "where ";
		}
		wStr << "( RPG between " << min_rpg << " AND " << max_rpg << ")";
		useAnd = true;
	}
	if (use_apg) {
		if(useAnd){
			wStr << " AND ";
		}else{
			wStr << "where ";
		}
		wStr << "( APG between " << min_apg << " AND " << max_apg << ")";
		useAnd = true;
	}
	if (use_spg) {
		if(useAnd){
			wStr << " AND ";
		}else{
			wStr << "where ";
		}
		wStr << "( SPG between " << min_spg << " AND " << max_spg << ")";
		useAnd = true;
	}
	if (use_bpg) {
		if(useAnd){
			wStr << " AND ";
		}else{
			wStr << "where ";
		}
		wStr << "( BPG between " << min_bpg << " AND " << max_bpg << ")";
		useAnd = true;
	}
	wStr << ";";
	//cout << wStr.str() << endl;
	nontransaction n(*C);
	result R ( n.exec( wStr.str()) );
	//prepare to print output
	cout << "ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG" <<endl;
	for(result::const_iterator c = R.begin(); c!= R.end(); ++c){
		cout << c[0].as<int>() << " ";
		cout << c[1].as<int>() << " ";
		cout << c[2].as<int>() << " ";
		cout << c[3].as<string>() << " ";
		cout << c[4].as<string>() << " ";
		cout << c[5].as<float>() << " ";
		cout << c[6].as<float>() << " ";
		cout << c[7].as<float>() << " ";
		cout << c[8].as<float>() << " ";
		cout << c[9].as<float>() << " ";
		cout << c[10].as<float>() << endl;
	}
}


void query2(connection *C, string team_color)
{
	ostringstream wStr;
	wStr << "SELECT TEAM.NAME from TEAM, COLOR where TEAM.COLOR_ID=COLOR.COLOR_ID AND ";
	wStr << "COLOR.NAME LIKE '" << team_color << "';";
	//cout << wStr.str() << endl;
	nontransaction n(*C);
	result R (n.exec( wStr.str()) );
	//prepare to print output
	cout << "NAME" <<endl;
	for(result::const_iterator c = R.begin(); c != R.end(); ++c){
		cout << c[0].as<string>() <<endl;
	}
}


void query3(connection *C, string team_name)
{
	ostringstream wStr;
	wStr << "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME from TEAM, PLAYER ";
	wStr << "WHERE PLAYER.TEAM_ID=TEAM.TEAM_ID AND TEAM.NAME='"<< team_name <<"' ";
	wStr << "ORDER BY PPG DESC;";
	//cout << wStr.str() << endl;
	nontransaction n(*C);
	result R (n.exec( wStr.str()) );
	//prepare to print output
	cout << "FIRST_NAME LAST_NAME" <<endl;
	for(result::const_iterator c = R.begin(); c != R.end(); ++c){
		cout << c[0].as<string>() <<" ";
		cout << c[1].as<string>() <<endl;
	}
}


void query4(connection *C, string team_state, string team_color)
{
	ostringstream wStr;
	wStr << "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, PLAYER.UNIFORM_NUM ";
	wStr << "from PLAYER, COLOR, STATE, TEAM ";
	wStr << "where PLAYER.TEAM_ID=TEAM.TEAM_ID AND TEAM.COLOR_ID=COLOR.COLOR_ID AND TEAM.STATE_ID=STATE.STATE_ID AND ";
	wStr << "STATE.NAME='"<<team_state<<"' AND COLOR.NAME='"<<team_color<<"';";
	//cout << wStr.str() << endl;
	nontransaction n(*C);
	result R (n.exec( wStr.str()) );
	//prepare to print output
	cout << "FIRST_NAME LAST_NAME UNIFORM_NUM" <<endl;
	for(result::const_iterator c = R.begin(); c != R.end(); ++c){
		cout << c[0].as<string>() << " ";
		cout << c[1].as<string>() << " ";
		cout << c[2].as<int>() <<endl;
	}
}


void query5(connection *C, int num_wins)
{
	ostringstream wStr;
	wStr << "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, TEAM.NAME, TEAM.WINS ";
	wStr << "from PLAYER, TEAM ";
	wStr << "where PLAYER.TEAM_ID=TEAM.TEAM_ID AND TEAM.WINS > " << num_wins << ";";
	//cout << wStr.str() << endl;
	nontransaction n(*C);
	result R (n.exec( wStr.str()) );
	//prepare to print output
	cout << "FIRST_NAME LAST_NAME TEAM_NAME TEAM_WINS" <<endl; 
	for(result::const_iterator c = R.begin(); c != R.end(); ++c){
		cout << c[0].as<string>() << " ";
		cout << c[1].as<string>() << " ";
		cout << c[2].as<string>() << " ";
		cout << c[3].as<int>() << endl;
	}

}

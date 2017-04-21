#include <iostream>
#include <pqxx/pqxx>
#include <fstream>

#include "exerciser.h"

using namespace std;
using namespace pqxx;


void dropTables(connection *C) {
  string drop_tables = "drop table player,team,state,color;";
  nontransaction drop_work(*C);
  try{
	  cout << "dropping old tables..." <<endl;
	  drop_work.exec(drop_tables);
  } catch(const exception& e) {
	  cout << "no tables to drop. continuing to create tables" <<endl;
  }
}

void createTables(connection *C) {
  // player sql statement

  string player_sql = "CREATE TABLE PLAYER(" \
  "player_id SERIAL PRIMARY KEY NOT NULL," \
  "team_id INT NOT NULL," \
  "uniform_num INT NOT NULL," \
  "first_name TEXT NOT NULL," \
  "last_name TEXT NOT NULL," \
  "MPG REAL NOT NULL," \
  "PPG REAL NOT NULL," \
  "RPG REAL NOT NULL," \
  "APG REAL NOT NULL," \
  "SPG REAL NOT NULL," \
  "BPG REAL NOT NULL);";
  
  string team_sql = "CREATE TABLE TEAM(" \
  "TEAM_ID SERIAL PRIMARY KEY NOT NULL," \
  "NAME TEXT NOT NULL," \
  "STATE_ID INT NOT NULL," \
  "COLOR_ID INT NOT NULL," \
  "WINS INT," \
  "LOSSES INT);";

  string state_sql = "CREATE TABLE STATE(" \
  "STATE_ID SERIAL PRIMARY KEY NOT NULL," \
  "NAME TEXT NOT NULL);";

  string color_sql = "CREATE TABLE COLOR(" \
  "COLOR_ID SERIAL PRIMARY KEY NOT NULL," \
  "NAME TEXT NOT NULL);";

  nontransaction W(*C);
  W.exec( player_sql );
  W.exec( team_sql );
  W.exec( state_sql );
  W.exec( color_sql );
  /*try{
  	W.exec( player_sql );
  	W.commit();
  	cout << "created player table" <<endl;
  } catch (const exception &e) {
	  W.abort();
	  throw;
  }
  try{
  	W.exec( team_sql );
  	W.commit();
  	cout << "created team table" <<endl;
  } catch (const exception &e){
	  W.abort();
	  throw;
  }
  try{
  	W.exec( state_sql );
  	W.commit();
  	cout << "created state table" <<endl;
  } catch (const exception &e){
	  W.abort();
	  throw;
  }
  try{
  	W.exec( color_sql );
  	W.commit();
  	cout << "Tables created successfully" << endl;
  } catch( const exception &e){
	  W.abort();
	  throw;
  }*/
  cout << "Successfully created tables." <<endl;
}

void loadPlayerFile(connection *C){
  string line;
  ifstream fs ("player.txt", ifstream::in);
  if (fs.good()){
  	while (getline(fs, line)){
  		istringstream iss(line);
		int id, team_id, uni;
		string fname, lname;
		float mpg,ppg,rpg,apg,spg,bpg;
		if(line.empty()){
			// skip line
		}else{
			if (!(iss >> id >> team_id >> uni >> fname >> lname >> mpg >> ppg >> rpg >> apg >> spg >> bpg)){
				break;
			}
			add_player(C,team_id,uni,fname,lname,mpg,ppg,rpg,apg,spg,bpg);
		}
  	}
  }
  fs.close();
}

void loadTeamFile(connection *C){
  string line;
  ifstream fs ("team.txt", ifstream::in);
  if (fs.good()){
  	while (getline(fs, line)){
  		istringstream iss(line);
		int team, state, color, win, lose; 
		string name;
		if(line.empty()){
			// skip line
		}else{
			if (!(iss >> team >> name >> state >> color >> win >> lose)){
				break;
			}
			add_team(C, name, state, color, win, lose);
		}
  	}
  }
  fs.close();
}

void loadStateFile(connection *C){
  string line;
  ifstream fs ("state.txt", ifstream::in);
  if (fs.good()){
  	while (getline(fs, line)){
  		istringstream iss(line);
		int id; 
		string name;
		if(line.empty()){
			// skip line
		}else{
			if (!(iss >> id >> name)){
				break;
			}
			add_state(C,name);
		}
  	}
  }
  fs.close();
}

void loadColorFile(connection *C){
  string line;
  ifstream fs ("color.txt", ifstream::in);
  if (fs.good()){
  	while (getline(fs, line)){
  		istringstream iss(line);
		int id; 
		string name;
		if(line.empty()){
			// skip line
		}else{
			if (!(iss >> id >> name )){
				break;
			}
			add_color(C,name);
		}
  	}
  }
  fs.close();
}

int main (int argc, char *argv[]) 
{

  //Allocate & initialize a Postgres connection object
  connection *C;
try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }
  // 1) drop all created tables
  dropTables(C);

  // 2) create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  createTables(C); 
  
  // 3) load each table with rows from the provided source txt files
  // load file data into tables
  loadPlayerFile(C);
  loadTeamFile(C);
  loadStateFile(C);
  loadColorFile(C);

  //TODO: execute queries
  exercise(C);


  //Close database connection
  C->disconnect();

  return 0;
}



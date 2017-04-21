#include "exerciser.h"

void exercise(connection *C)
{
	cout << "query1: min btw 33 & 40" <<endl;
	query1(C, 1, 33, 40,
		0,0,0,
		0,0,0,
		0,0,0,
		0,0,0,
		0,0,0);
	cout << "query1: min btw 33 & 40 AND ppg btw 10 & 20" <<endl;
	query1(C, 1, 33, 40,
		1,10,20,
		0,0,0,
		0,0,0,
		0,0,0,
		0,0,0);
	cout << "query1: min btw 33 & 40 AND ppg btw 10 & 20 AND apg btw 2 & 7" <<endl;
	query1(C, 1, 33, 40,
		1,10,20,
		0,0,0,
		1,2,7,
		0,0,0,
		0,0,0);
	cout << "query1: min btw 33 & 40 AND bpg btw 2 & 5" <<endl;
	query1(C, 0, 33, 40,
		1,10,20,
		0,0,0,
		0,0,0,
		0,0,0,
		1,2,5);

	cout << "query2: Team with color Darkblue" << endl;
	query2(C,"DarkBlue");
	cout << "query2: Team with color Gold" << endl;
	query2(C,"Gold");

	cout << "query3: Players for duke ordered by ppg" << endl;
	query3(C,"Duke");
	cout << "query3: Players for BC ordered by ppg" << endl;
	query3(C,"BostonCollege");
	cout << "query3: Players for ND ordered by ppg" << endl;
	query3(C,"NotreDame");


	cout << "query4: name & # for state NC and lightblue" <<endl;
	query4(C,"NC","LightBlue");
	cout << "query4: name & # for state MA and maroon" <<endl;
	query4(C,"MA","Maroon");

	cout << "query5: player, team and wins > 8 wins" << endl;
	query5(C,8);
	cout << "query5: player, team and wins > 11 wins" << endl;
	query5(C,11);
}

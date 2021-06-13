/*
                                ISLAND v2.6

    Folks, sorry, but I was TOO lazy to write any comments....so figure it up
yourself....


Black Gate coderz group...                         Ú- - --Ä  Ä- -¿
CyberDaemon...codez                                : CyberDaemon
Dreamcatcher..design and ideas                     À  Ä-Ä  -Ä-- ÄÙ
Chepponi......BBS                              <vkrstul@public.srce.hr>

SukhchainSingh..Ported this game to windows
gcc -o ISLAND ISLAND.C include/conio.o
*/

#include <time.h>
#include <stdio.h>
#include <direct.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "include/conio.h"

#define UP(a)   (toupper (a))
#define LOW(b)  (tolower (b))
#define NERR    0
#define ERR	1
#define OPENED  1
#define LOCKED  2
#define CLOSED  0
#define TRUE	1
#define FALSE	0

int random(int num);

char *globargv[20]; // array of words parsed from input line
char wordtype[20]; // type of word -> noun, verb ...
int globargc;
FILE *fp;

struct directions // structure of directions like north, south, east, west
{
	int n;
	int s;
	int e;
	int w;
};

struct directions room[] = { 
				0,0,0,0,    // structure with entrances to other rooms (4 directions)
				-1,-1,-1,2,
				0,3,1,0,
				2,4,0,0,
				3,0,0,5,
				7,6,4,-1,
				5,-2,-2,-2,
				8,5,-1,-1,
				9,7,-1,-1,
				11,8,-3,15,
				-4,-4,-4,9,
				12,9,-1,-1,
				13,11,-1,-1,
				14,12,-1,-1,
				-4,13,-4,-4,
				-1,-1,9,16,
				-5,-5,15,-5      
			};
char *message[]= { "",                            // Some messages
		"The forest is unpassable unless you are the coder of this game to make it \npassable...",
		"You don't know how to swim. Besides, it's full of sharks.",
		"",
		"Suddenly the CyberDaemon, coder of this game pops out from the stack and\ntells you that you can't go that way.",
		""           };

// These are arrays of nouns, verbs, ...
char *noun[]={ "","ENGINE","KEY","CAN","FOOD","SHOVEL","CUP","STICK","PAPER",
		  "CHEST","AIRPLANE","HOUSE","BOAT","SAND","FUEL","DOOR",
		  "PLANE","SIGN","GRAVE"};

char *verb[]={ "","N","S","E","W","LOOK","GET","DROP","DIG","OPEN","CLOSE",
		  "UNLOCK","READ","LOAD","SAVE","START","TURN","TAKE","LEAVE",
		  "RESTORE","?","HELP","I","INVENTORY","ENTER" };

char *clan[]={ "","THE","AN","A" };

char *adverb[]={ "","HEAVY","GOLDEN","IRON","","","PLASTIC","WOODEN","SMALL",
		  "GOLDEN","","OLD","SMALL","YELLOW","","IRON","","SMALL",
		  "ANCIENT" };

char *cerr[]={ "","I don't understand the word","What next ??","Please be more specific.","Oh really ???","I can't see THAT thing anywhere!" };

char *sandtxt[]={ "","You dig up a small hole in the sand.","You dig and the hole in the sand becomes deeper." };

// 'object_info' is an array of descriptions to each object in game
char *object_info[]= { "","It is a MERCURY 720 engine.","Look at your house key.",
		  "It is a medium size green army fuel can.","It smells like a fried chicken.",
		  "It looks like a shovel that grave-diggers use.","Just an ordinary cup.",
		  "It has perhaps fallen from a tree.","It is a computer paper. Something is written on it.",
		  "The golden chest is locked.","WOW! An YF-22 !!",
		  "The house is rusty and old. With no windows, only a door.",
		  "It is a military SCORPION speed boat.","It looks just like the sand.",
		  "It smells like kerosine, and it is blue.","Just a closed door.",
		  "WOW! An YF-22 !!","There is something written on it.","...Here lies CyberDaemon..." };

int inoun,iverb,iclan,ipron;
int n,p,err,errpos;

int collision, can=CLOSED, chest=LOCKED, pos=1, sand;
int verbs=24, nouns=18;

char *roomnme[] ={ "",	"Small clearing		",
			"Forrest		",
			"Forrest		",
			"Forrest		",
			"Sandy shore		",
			"In the boat		",
			"Forrest path		",
			"Forrest path		",
			"In the village		",
			"In the old house	",
			"Mountain side		",
			"Mountain side		",
			"Cave mouth 		",
			"In the cave		",
			"Cliff path		",
			"Rocky ledge		" };

// description is description to each room
char *description[]={ "",
			"  You are on a small clearing. Here is your crashed plane, and you can see\n"
			"woods all around. There is a small path leading into the woods to the west\n"
			"and you can see a small sign by the path.",

	      "  You are lost in a wood. There are trees all around you.",

	      "  You are lost in a wood. There are trees all around you.",

	      "  You are lost in a wood. There are trees all around you.",

			"  You are on a shore. There is a small boat in the sea to the south, and \n"
	      "there a pathway can be seen to the north. You can hear sounds of sea     \n"
	      "splashing off the shore. You stand on a yellow and hot sand that burns on\n"
	      "your feet. This reminded you on those Pirate Gold films you were watching\n"
	      "at home.",

	      "  You are in a small boat. You can see and hear a sea splashing off the shore.",

	      "  This is a small path in the wood that runs from north to south. To the\n"
	      "south  sounds of sea can be heard, and you can see some buildings to the\n"
	      "north. The wood is here less frequent. ",

	      "  You are on a a small path in the wood that continues to the north and to \n"
	      "the south. To the north you can see a small village, and to the south you  \n"
	      "can hear the sounds of sea.",

	      "  This is a native village on the small clearing sorrounded by wood. Here, \n"
	      "you can see some houses all made of bamboo wood and an old house made of   \n"
	      "solid rock to the east. Yet, although village looks habitable, there is no-\n"
	      "body here. You can see a rocky ledge to the west, and a distant mountain to\n"
	      "the north. There is a path running into the woods to the south.",

	      "  You are in an old house. From the ceiling, an oil lamp gives some light. The\n"
		"house contains only one room with no windows and one door to the west. The\n"
		"walls are old and rusty, and you can feel the moisture in the air.",

	      "  You are on a path that runs up in mountain to the north. You can see a   \n"
	      "village surrounded by woods to the south.",

	      "  This is an end of a path that leads from the village. You are high up here\n"
	      "so you can see the whole island. There is a sea surrounding the small island\n"
	      "on which you are. You can also see a small rocky ledge down there. To the   \n"
		"north you can see an entrance to the cave, and a high top of the mountain.",

	      "  You are in front of an entrance to the cave to the north. There is a path \n"
	      "going down to the south.",

	      "  You are in the small cave. Here, you can see a golden chest up to the wall\n"
	      "and an exit leading south. You can hear drops of water from somewhere.",

	      "  This is a small rocky pathway that leads to the ledge on the west. You can\n"
	      "see a village to the east.",

	      "  You are on top of the rocky ledge. Here you can see a grave that speaks :   \n"
	      "'....Here lies CyberDaemon....'. Looking around you can see woods far below,\n"
	      "and a small village to the east. To the north you can see a distant huge     \n"
	      "mountain. There is a chasm all around you except the small path to the east." };

int objpos[]={ 0,6,-2,10,10,16,1,1,-255,114,101,109,105,105,-4,109,101,101,116};

// names for objects :
char *object_name[]={ "","a heavy engine",
			 "a golden key",
			 "an iron can",
			 "some food",
			 "a shovel",
			 "a plastic cup",
			 "a wooden stick",
			 "a small paper",
			 "a golden chest",
			 "an airplane",
			 "an old house",
			 "a small boat in the sea",
			 "a yellow sand on the ground",
			 "some fuel",
			 "an iron door on the house",
			 "an airplane",
			 "a small sign",
			 "an ancient grave" };

// functions :
char *word_parse 		(char*,char*,int); // line parser
void enargv      		(char*); // line parser's help function
void doit        		(void);  // do orders
void recognize       	(void);  // see which word is which (verb, or noun ... )
void see_error   		(void);  // see if any sintax error and identify words
void describe_the_room  (void);
int  compare     		(char*,char*);  // compare two strings
void clearfields 		(void);  // clear wordtype[] and globargv[]
void event       		(void);  // events in the game
void look        		(void);  // look something
void get         		(void);  // No comment needed here
void read_       		(void);
void open_       		(void);
void drop        		(void);
void turn        		(void);
void dig         		(void);
void start       		(void);
void close_      		(void);
void unlock      		(void);
void load_       		(void); // load savegame
void save_       		(void); // save savegame
void n_          		(void);
void s_          		(void);
void e_          		(void);
void w_          		(void);
void enter	     		(void);
void opendoor    		(void);
void fall        		(void);
void endgame     		(void);
void inv	     		(void); // inventory
void help	     		(void);
void chk_adjective	 	(void); // check if right adjective used....
void definition    		(void); // check for sinonims

extern unsigned _stklen= 10000;
main()
{
	char string[300];

	// asm( "jmp start " );
	// asm( "db 'UnderWorld forever....CyberDaemon' " );
// start:
	srand(time(NULL));
	clrscr();
	printf ("%s", roomnme[pos]);
	printf ("\n--------------------------------------------------------------------------------");
	printf ("\nISLAND v2.6, is a small textual adventure coded by CyberDaemon, Black Gate and ported by Sukhchain Singh.");
	printf ("\n\n");
	do
	{
		err=0;
		errpos=0;
		printf ("\n > ");
		gets(string);
		clrscr();
		printf ("%s", roomnme[pos]);
		printf ("\n--------------------------------------------------------------------------------");
		clearfields();
		enargv(string);
		recognize();
		see_error();
		definition();
		chk_adjective();
		if (err==0)
			doit();
		if (err!=0)
			if (err==1)
				printf ("\n%s '%s'.",cerr[1],globargv[errpos]);
			else
				printf ("\n%s",cerr[err]);
		event();
	}
	while (true);
}

void help()
{
	printf ("\n\n\n  All what you have to do is to end this game.");
	printf ("\nYou have to input some orders like LOOK, OPEN, and so on.");
	printf ("\nIf you want to move through the island, just input one of four directions: ");
	printf ("\n    to go north input   >n    or    >N    ");
	printf ("\n    to go south input   >s    or    >S    ");
	printf ("\n    to go east  input   >e    or    >E    ");
	printf ("\n    to go west  input   >w    or    >W    ");
	printf ("\n\nTo see what are you carrying input >i  or  >inventory ");
	printf ("\nTo quit the game press Ctrl-Break.");
	printf ("\n\nThe rest is on program to do.");
}

void inv()
{
	int i;

	printf ("\nYou have :");
	if (collision==0)
		printf ("    nothing...Except your sanity...");
	else
		for (i=1;i<nouns;i++)
		{
			if (objpos[i]==0)
				printf ("\n%s",object_name[i]);
		}

}

int random(int num)
{
    return rand() % num + 1;
}

void event()
{
	printf ("\n");
	if (pos==5 || pos==6)
		if (random(10)==6)
			printf ("\nYou can hear a splashing sound in the sea.");
	if (pos==2 || pos==3 || pos==4)
		if (random(10)==3)
			printf ("\nYou can hear a deep moaning sound.");
	if (pos==12 || pos==13 || pos==15 || pos==16 || pos==5 || pos==6 || pos==9)
		if (random(15)==4)
			printf ("\nYou can see yourself staring back at you.");
	if (pos==11 || pos==12 || pos==13)
		if (random(10)==7)
			printf ("\nYou can see mountain bulge in and out at the top.");
	if (random(15)==2)
		printf ("\nYou turn around and see a whole bunch of daemons following you in silence.\nDon't LOOK AT DAEMONS because they are gone now...");
	if (random(20)==3)
		printf ("\nWAIT! I have to pop something from the stack...ok..continue...");
	if (pos==7 || pos==8)
		if (random (5)==2)
			printf ("\nSuddenly a bunch of daemons passes by you carrying a sign:\n'Vote for this game!'\n'We love CyberDaemon!'");

}

void definition(void)
{
	if (iverb==17)
		iverb=7;
	if (iverb==18)
		iverb=8;
	if (iverb==19)
		iverb=14;
	if (inoun==16)
		inoun=10;
}

void describe_the_room()
{
	int o;

	if (pos==1)
		objpos[17]=101;
	if (pos==16)
		objpos[17]=116;

	clrscr();
	printf ("%s", roomnme[pos]);
	printf ("\n--------------------------------------------------------------------------------");

	printf ("\n%s",description[pos]);

	for (o=1;o<nouns+1;o++)
	{
		if (objpos[o]==pos)
		{
			printf ("\n  There is %s here.",object_name[o]);
		}
	}
}

void chk_adjective (void)
{
	if (ipron == 2)
		if (inoun== 2 || inoun== 9)
			return;
	if (ipron == 3)
		if (inoun== 3 || inoun== 15)
			return;
	if (ipron == 8)
		if (inoun== 8 || inoun== 17)
			return;
	if (ipron && inoun && ipron!=inoun)
	{
		err =5;
		return;
	}
}

void doit()
{
	switch (iverb)
	{
		case 1:
		  n_();		  break;
		case 2:
		  s_();		  break;
		case 3:
		  e_();		  break;
		case 4:
		  w_();		  break;
		case 5:
		  look();	  break;
		case 6:
		  get();	  break;
		case 7:
		  drop();	  break;
		case 8:
		  dig();	  break;
		case 9:
		  open_();	  break;
		case 10:
		  close_();	  break;
		case 11:
		   unlock();  break;
		case 12:
		  read_();	  break;
		case 13:
		  load_();	  break;
		case 14:
		  save_();	  break;
		case 15:
		  start();	  break;
		case 16:
		  turn(); 	  break;
		case 20:
		case 21:
		  help(); 	  break;
		case 22:
		case 23:
		  inv();  	  break;
		case 24:
		  enter();	  break;
	}
}

void n_()
{
	int i;
	if (room[pos].n<0)
	{
		i=(room[pos].n * (-1));
                if (i==5)
		{
			fall();
			return;
		}
		printf ("%s",message[i]);
		return;
	}
	else
		if (room[pos].n==0)
		{
			describe_the_room();
			return;
		}
		else
		{
			pos=room[pos].n;
			describe_the_room();
		}
}
void s_()
{
	int i;
	if (room[pos].s<0)
	{
		i=(room[pos].s * (-1));
                if (i==5)
		{
			fall();
			return;
		}
		printf ("%s",message[i]);
		return;
	}
	else
		if (room[pos].s==0)
		{
			describe_the_room();
			return;
		}
		else
		{
			pos=room[pos].s;
			describe_the_room();
		}
}
void e_()
{
	int i;
	if (room[pos].e<0)
	{
		i=(room[pos].e * (-1));
		if (i==3)
		{
			opendoor();
			return;
		}
		printf ("%s",message[i]);
		return;
	}
	else
		if (room[pos].e==0)
		{
			describe_the_room();
			return;
		}
		else
		{
			pos=room[pos].e;
			printf ("");
			describe_the_room();
			return;
		}
}
void w_()
{
	int i;
	if (room[pos].w<0)
	{
		i=(room[pos].w * (-1));
		if (i==5)
		{
			fall();
			return;
		}
		printf ("%s",message[i]);
		return;
	}
	else
		if (room[pos].w==0)
		{
			describe_the_room();
			return;
		}
		else
		{
			pos=room[pos].w;
			describe_the_room();
		}
}

void enter()
{
	if (objpos[inoun]!=pos && objpos[inoun]!=(pos+100) )
	{
		printf ("\nWhere can you see that ?");
		return;
	}
	if (inoun==10)
	{
		printf ("\nYou can't,the airplane has crashed.");
		return;
	}
	if (inoun==11)
	{
		if (room[9].e==10)
			e_();
		else
			printf ("\nThe door is closed.");
		return;
	}
	if (inoun==12)
	{
		s_();
		return;
	}
	printf ("\nYou can't enter in that. You better visit a psychiatrist.");
}

void look()
{
	if (globargc==1)
	{
		describe_the_room();
		err=0;
		return;
	}
	else
		if (objpos[inoun]==pos || objpos[inoun]==0 || objpos[inoun]==-3 || objpos[inoun]==(pos+100) )
			printf ("\n%s",object_info[inoun]);
		else
			printf ("\nYou can't see that here!");
}
void get()
{
	if (objpos[inoun]==pos || objpos[inoun]==-3 || objpos[inoun]==(pos+100) )
	{
		if (inoun==14)
		{
			printf ("\nNo way! It will just spill around.");
			return;
		}

		if (inoun==1 || (inoun>7 && inoun<19))
		{
			printf ("It refuses to be moved !");
			return;
		}

		if (collision<4)
		{
			printf("\nOk. Now you got it.");
			objpos[inoun]=0;
			collision++;
			return;
		}
		else
		{
			printf ("\nYou are carying too much.");
			return;
		}
	}
	else
		if (objpos[inoun]==0)
			printf ("\nYou already have that.");
		else
			printf ("\nYou can't see that here.");
}
void drop()
{
	if (objpos[inoun]==0)
	{
		printf("\nOk. Now you droped it.");
		objpos[inoun]=pos;
		collision--;
		return;
	}
	else
		printf ("\nHow can you drop something you don't even have ?");
}
void dig()
{
	if (objpos[inoun]!=pos && objpos[inoun]!=(pos+100))
	{
		printf ("\nWhere can you see that ?");
		return;
	}
	if (inoun !=13 && inoun !=18)
	{
		printf ("\nWhat a stupid idea! You can't dig that man!");
		return;
	}
	if (objpos[5]==0)
	{
		if (pos==5)
		{
			if (sand<3)
			{
				sand++;
				if (sand==3)
				{
					printf("\nYou dig, and dig, and dig enough big hole to see a golden key in it.");
					objpos[2]=pos;
				}
				else
					printf ("\n%s",sandtxt[sand]);
			}
			else
				printf ("\nYou can't dig more deeper.");
		}
		else
		{
			if (pos==16)
			{
				printf ("\n\n\nAs you approach to the grave to dig it, you can hear a demonic voice from the\n");
				printf ("grave. You drop the shovel with fright.");
				objpos[5]= pos;
				collision--;
				return;
			}

			printf ("\nYou dig up some small holes. Boring.");
		}
	}
	else
		printf ("\nYou start digging with your hands, but soon you end up with a \ncute sand-castle...but the sea leveled it off.");
}
void read_()
{
	if (objpos[inoun]!=0 && objpos[inoun]!=pos && objpos[inoun]!=-3 && objpos[inoun]!=(pos+10) )
	{
		printf ("\nWhere can you see that?");
		return;
	}

	if (inoun==8)
	{
		printf ("\nOn the paper is written DEATHMATCH.");
		return;
	}
	if (inoun==17)
	{
		if(pos==1)
			printf ("\nOn the sign is written :'Tourists! Take some food before leaving island...'");
		if(pos==16)
			printf ("\nOn the sign is written :'Beware of falling over the ledge!'");

		return;
	}
	printf ("\nHow can you read that?");

}
void open_()
{
	if (objpos[inoun]!=0 && objpos[inoun]!=pos && objpos[inoun]!=(pos+100) )
	{
		printf ("\nWhere can you see that here?");
		return;
	}
	if (inoun==9)
	{
		if (chest==LOCKED)
		{
			printf ("\nThe chest is locked.");
			return;
		}
		if (chest==CLOSED)
		{
			printf ("\nYou open a golden chest, and you see a small paper in it.");
			objpos[8]=-3;
			object_info[9]= "The golden chest is opened. There is a piece of paper in it.";
			chest=OPENED;
		}
		else
			printf ("\nThe golden chest is already opened.");
		return;
	}
	if (inoun==3)
	{
		if (can==CLOSED)
		{
			printf ("\nAs you open an iron can the smell of fuel fills the air.");
			can=OPENED;
		}
		else
			printf ("\nThe iron can is already opened.");
		return;
	}
	if (inoun==15)
	{
		if (room[9].e==10)
			printf ("\nThe iron door is already opened.");
		else
			e_();
//			opendoor();
		return;
	}
	printf ("\nThe gods wount let you..");
}
void close_()
{
        if (objpos[inoun]!=0 && objpos[inoun]!=pos && objpos[inoun]!=(pos+100) )
	{
		printf ("\nWhere can you see that here ?");
		return;
	}
	if (inoun==9)
	{
		if (chest==OPENED)
		{
			printf ("\nYou close a golden chest.");
			chest=CLOSED;
			object_info[9]= "The golden chest is closed.";
		}
		else
			printf ("\nThe golden chest is already closed.");
		return;
	}
	if (inoun==3)
	{
		if (can==OPENED)
		{
			printf ("\nYou close an iron can, and the smell of fuel dissapears.");
			can=CLOSED;
		}
		else
			printf ("\nThe iron can is already closed.");
		return;
	}
	printf ("\nHow can you close that ?");
}
void unlock()
{
        if (objpos[inoun]!=0 && objpos[inoun]!=pos && objpos[inoun]!=(pos+100))
	{
		printf ("\nWhere can you see that?");
		return;
	}
	if (inoun==9)
	{
		if (pos!=14)
		{
			printf ("\nWhere can you see the chest ?");
			return;
		}
		if(chest==LOCKED)
		{
			if (objpos[2]==0)
			{
				printf ("\nYou unlock the golden chest.");
				object_info[9]= "The golden chest is closed.";
				chest=CLOSED;
				return;
			}
			else
			{
				printf ("\nYou don't have the key to unlock it.");
				return;
			}

		}
		else
			printf ("\nThe chest is already unlocked.");
	}
	if (inoun==15)
	{
		printf ("\nThe door has been locked from inside.");
		return;
	}
	printf ("\nHow can you unlock that ?");
}
void start()
{
	if (objpos[inoun]!=0 && objpos[inoun]!=pos && objpos[inoun]!=(pos+100))
	{
		printf ("\nWhere can you see that?");
		return;
	}
	if (inoun==10)
	{
		if (pos==1)
			printf ("\nYou can't.The airplane has crashed, remember?");
		else
			printf ("\nWhere can you see that?");
		return;
	}
	if (inoun==1 || inoun==12)
	{
		if (objpos[3]==0)
			endgame();
		else
			printf ("\nYou don't have the fuel to start it.");
		return;
	}
	printf ("\nHow can you start that?");
}
void turn()
{
		printf ("\nYou kept turning that stuff for an hour and find nothing.");
		return;
}
void save_()
{
	printf ("\nNot in this version....!");
}
void load_()
{
	printf ("\nNot in this version....!");

}
void fall()
{
	printf ("\nYou have obviously not read the sign, so you slip off the ledge and fall to \nyour death.");
	printf ("\n\n    The game is over. You died.");
	printf ("\n\nPress any key to return to DOS.");
	getch();
	// asm( "mov ax, 4c00h" );
	// asm( "int 21h" );
}
void opendoor()
{
	char *code;

	printf ("\nAs you approach the old house, there, a voice booms from somewhere:\n");
	printf ("\n\n'Do you know the code to enter the house ???'");
	printf ("\n\nThe Code>>>");
	gets(code);
	clearfields();
	enargv(code);
	if (compare (globargv[0],"DEATHMATCH")==ERR)
		printf ("\n\nThe voice booms: 'Code invalid!! Access denied!'");
	else
	{
		printf ("\nThe door bulges forward for few times and then opens.");
		room[9].e=10;
	}
}
void endgame()
{
	printf ("\n\n\n\n  You put the fuel into the engine, start it, and hit the road to somewhere...");
	if (objpos[4]==0)
	{
		printf ("\n\n\n        Congratulations!!!");
		printf ("\n\nYou have finished the game, and you have proved yourself to be an andventurist.");
		printf ("\nNow you can play some more seriously adventure games like ");
		printf ("\n\n\n                 Pandaemonium");
		printf ("\n\nThanks for playing The Island...");
		printf ("\n\n\n\n\nPress any key to return to DOS");
		getch();
		clrscr();

		// asm( "mov ax, 4c00h" );
		// asm( "int 21h" );
	}
	else
	{
		printf ("\n\nYou were wandering around on sea for few days, and then you died of hunger.");
		printf ("\n\nThe game is over. Press any key to return to DOS.");
		getch();
		exit(0);
	}
}

void see_error()
{
	int i;

	err=0;
	iverb=0;
	inoun=0;
	iclan=0;
	ipron=0;

	if (!globargc)
	{
		err= 2;
		return;
	}

	for (i=0; i< globargc; i++)
	{

		if (wordtype[i] == 0)
		{
			if (i==0)
			{
				err= 1;
				errpos= 0;
				return;
			}
			err= 1;
			errpos= i;
			return;
		}
	}

	if (wordtype[0] != 2)
	{
		err= 3;
		return;
	}

	for (i=1; i< (verbs+1); i++)
		if ( compare (globargv[0], verb[i]) == NERR )
			iverb = i;

	if (iverb<5 || iverb==13 || iverb==14 || (iverb>18 && iverb<23 ))
	{
		if (globargc>1)
			err= 3;
		return;
	}

	if (iverb == 5)
	{
		if (globargc==1)
			return;
	}

	for (i=1; i< (nouns+1); i++)
		if ( compare (globargv[1], noun[i]) == NERR )
		{
			inoun = i;
			return;
		}

	for (i=1; i< (nouns+1); i++)
		if ( compare (globargv[1], adverb[i]) == NERR )
		{
			ipron = i;

			for (i=1; i< (nouns+1); i++)
				if ( compare (globargv[2], noun[i]) == NERR )
				{
					inoun = i;
					return;
				}
			err= 3;
			return;
		}

	for (i=1; i< 5; i++)
		if ( compare (globargv[1], clan[i]) == NERR )
		{
			iclan = i;

			for (i=1; i< (nouns+1); i++)
				if ( compare (globargv[2], noun[i]) == NERR )
				{
					inoun = i;
					return;
				}

			for (i=1; i< (nouns+1); i++)
				if ( compare (globargv[2], adverb[i]) == NERR )
				{
					ipron = i;

					for (i=1; i< (nouns+1); i++)
						if ( compare (globargv[3], noun[i]) == NERR )
						{
							inoun = i;
							return;
						}
					err= 3;
					return;
				}
		}
}

void recognize()
{
	int i, j;

	for (j=0; j< globargc; j++)
	{
		for (i=1 ;i< (verbs+1) ;i++)
			if (  compare(globargv[j],verb[i])==NERR  )
				wordtype[j]=2;
		for (i=1 ;i< (nouns+1) ;i++)
			if (  compare(globargv[j],noun[i])==NERR  )
				wordtype[j]=1;
		for (i=1 ;i< (nouns+1) ;i++)
			if (  compare(globargv[j],adverb[i])==NERR  )
				wordtype[j]=4;
		for (i=1 ;i< 5 ;i++)
			if (  compare(globargv[j],clan[i])==NERR  )
				wordtype[j]=3;
	}

}

void clearfields()
{
	int i;
	for (i=0;i<globargc+1;i++)
	{
		globargv[i]=NULL;
		wordtype[i]=0;
	}
}

// This code has some serious problems
int compare (char *s, char *s1)
{
	int a=0,i=0;

	if (strlen(s) == strlen(s1))
		for (i=0;i<strlen(s)+1;i++)
		{
			if (UP(*(s+i)) != UP(*(s1+i)))
			a=1;
		}
	else
		a=1;
return ( a );
}

void enargv( char *buf )
{
	char *bufptr;
	char pbuf[50];
	globargc=0;
	bufptr=buf;
	while (  bufptr=word_parse( bufptr,pbuf,0)  )
	{
		globargv[ globargc ] = (char*) malloc( strlen(pbuf)+1 );
		strcpy ( globargv[ globargc++ ],pbuf );
	}
	// globargv[ globargc ]=NULL;
}

char *word_parse( char *line, char *word, int delim )
{
	int quot=0;
	/**
	* "*line" checks if the 1st character is not a null terminator.
	* "&&" will execute the rest of the condition if "*line" is not null terminator.
	* "(isspace(*line) || *line==delim)" checks if the character is a space or not
	* and the "*line==delim" is useless because we are already checking it in
	* the first "*line".
	*/
	while ( *line && (isspace(*line) || *line==delim) )
		line++;
	// this line is also useless since isspace() alredy cheks for it,
	// keeping it for historical purposes.
	if ( *line=='\n' )
		line++;

	// If I understand it correctly, this condition is supposed to be true,
	// only if *line is '\0' and we are previously ensuring that this is not
	// the case. Kami no itouri.
	if ( !*line )
	{
		*word='\0';
		return ( 0 );
	}
	while ( *line )
	{
		if ( !quot && (*line=='\"' || *line=='\'') )
			quot=*line++;
		// if character is a white space and the previous character wasn't a quote
		if ( (isspace(*line) || *line==delim) && !quot )
			break;
		else // this line checks (you guess it)!
			if ( quot && *line==quot )
			{
				quot=0;
				line++;
			}
			else
				*word++=*line++;
	}
	*word='\0';
return ( line );
}

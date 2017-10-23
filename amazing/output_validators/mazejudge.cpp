// mazejudge.cpp : Defines the entry point for the console application.
//

#ifdef linux
#define MAX_PATH 1024
#include <cstring>
#include <sys/types.h>
#include <dirent.h>
#define strnicmp(x,y,z) strncasecmp(x, y, z)
#else
#error no longer supported
#endif

#ifndef _MAZE_H_
#define _MAZE_H_

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<ctime>
using namespace std;

#define	OFFSET_FACTOR	20.0
#define	CELL_UNIT		1.0

class Maze{
public:
	Maze(int nwidth, int nheight, int has_soln = 1);
	int width;
	int height;
	int has_solution;
	int size;
	double xOffset;
	double yOffset;
	int find_root(int);	
	void union_cell(int, int);
	bool Build(void);
	void remove(int);
	bool FindSol(int from, int to);
	int MoveRight(void);
	int MoveLeft(void);
	int MoveUp(void);
	int MoveDown(void);
	inline bool Solved(void) { return m_bSolved; }
	inline int GetCurrentPos(void) { return m_nCurPos; }
	static Maze *Restore(const char *szFile);
	void Reset(void);
	static bool GetNext(char *&s, int &r, int &d);
	bool HasSolution(void) { return has_solution != 0; }
	vector<int> s;  /* parent's id */
	vector<double> xcoord;  /* lower right coordinate x */
	vector<double> ycoord;  /* lower right coordinate y */
	/* for the next two vectors, -1 means down, 0 means must be up 1 means up */
	vector<int> down;	  /* lower wall; knocked down or there */
	vector<int> right;  /* right wall; knocked down or there */
	vector<int> visited;  /* 1 means visited and 0 means not visited yet while finding solution */
	vector<int> tried;	/* Bitmask indicating directions tried. */

	vector<int> lottery;
	vector<int>	sol;
	int m_nCurPos;
	bool m_bSolved;
	int m_nMoves;
};

#define	WALL_DOWN		-1
#define	WALL_MUST_BE_UP	0
#define	WALL_UP			1

#define	NODE_VISITED	1
#define	NODE_NOTVISITED	0

#define	DIR_RIGHT	0x01
#define	DIR_DOWN	0x02
#define	DIR_LEFT	0x04
#define	DIR_UP		0x08

#endif

//#define	DEBUG
#undef	DEBUG

#define	MAX_TRIES		100
#define	NO_WAY_STRING	"no way out"
#define	NO_WAY_LEN		10

Maze *pMaze;
char szMazeFilePath[MAX_PATH];
char *szMazeFiles[10000];
int nMazeFiles;

char *szResultFile;
int bSentXML = 0;
char szErr[256];
#ifdef WIN32
#define	snprintf	_snprintf
#endif
void GenXML(const char *szMsg1, const char *szMsg2)
{
	FILE *fp;

	if(szResultFile != NULL && *szResultFile != '\0'){
		fp = fopen(szResultFile, "wt");
		if(fp != NULL){
			fprintf(fp, "<?xml version=\"1.0\"?>\n");
			fprintf(fp, "<result outcome=\"%s\" security=\"%s\">%s</result>\n", szMsg1, szResultFile, szMsg2 == NULL ? "" : szMsg2);
			bSentXML = 1;
			fclose(fp);
		} else {
			fprintf(stderr, "Can't open result file %s\n", szResultFile);
		}
	} else {
		fprintf(stderr, "No result file: %s - %s\n", szMsg1, szMsg2 == NULL ? "" : szMsg2);
	}
}

int CmpFiles(const void *p1, const void *p2)
{
	const char *s1 = *((const char **)p1);
	const char *s2 = *((const char **)p2);
	return(strcmp(s1, s2));
}

int main(int argc, char **argv)
{
        FILE *debug = fopen("/dev/tty", "w");
        int nRet, nLen;
        char szBuf[64];
        char szFileBuf[MAX_PATH];
        int ntries;
        int nprob;
        bool bDone;
        const char *szFile, *szDir;
#ifdef WIN32
        HANDLE hDir;
        WIN32_FIND_DATA fData;
#else
        DIR *pDir;
        struct dirent *pent;
#endif

        szFile = argv[1];
        szResultFile = NULL;
        nprob = 0;
        nMazeFiles = 0;

#ifdef DEBUG
		fprintf(stderr, "Begin file %s\n", szFile);
#endif
		pMaze = Maze::Restore(szFile);
		if(pMaze != NULL){
			ntries = 0;
			bDone = false;
			while(::fgets(&(szBuf[0]), sizeof(szBuf), stdin) != NULL){
#ifdef DEBUG
				fprintf(stderr, "Got move %s", &(szBuf[0]));
				fprintf(debug, "Got move %s", &(szBuf[0]));
#endif
				ntries++;
				if(::strncmp(&(szBuf[0]), NO_WAY_STRING, NO_WAY_LEN) == 0){
					if(pMaze->HasSolution() == false){
						pMaze->m_bSolved = true;
					    fprintf(stdout, "solved\n");
					} else {
						::snprintf(&(szErr[0]), sizeof(szErr)-1, "No solution indicated on maze %d(%s), but there is one", nprob+1, szFile);
					    fprintf(stdout, "wrong\n");
						exit(43); // GenXML("Wrong Answer", &(szErr[0]));
					}
					break;
				}
				switch(szBuf[0]){
				case 'r': nRet = pMaze->MoveRight(); break;
				case 'l': nRet = pMaze->MoveLeft(); break;
				case 'd': nRet = pMaze->MoveDown(); break;
				case 'u': nRet = pMaze->MoveUp(); break;
				default:
					fprintf(stderr, "Bad move %c\n", szBuf[0]);
					nRet = -1;
					break;
				}
#ifdef DEBUG
				fprintf(stderr, "Return value from move is: %d\n", nRet);
				fprintf(debug, "Return value from move is: %d\n", nRet);
#endif
				switch(nRet){
				case 0:
					fprintf(stdout, "wall\n");
                    fflush(stdout);
					break;
				case 1:
					fprintf(stdout, "ok\n");
                    fflush(stdout);
					break;
				case 2:
					/* duplicate move */
					fprintf(stdout, "wrong\n");
					exit(43); // GenXML("Wrong Answer", "Duplicate Move");
					bDone = true;
					break;
				case 3:
					fprintf(stdout, "solved\n");
                    fflush(stdout);
					bDone = true;
					break;
				}
#ifdef DEBUG
				fprintf(stderr, "After checking nRet(%d), bDone=%d bSentXML=%d\n", nRet, bDone, bSentXML);
#endif
				if(bDone == true){
#ifdef DEBUG
					fprintf(stderr, "Detected bDone==true - breaking out of gets loop\n");
#endif
					break;
				}
			}
#ifdef DEBUG
			fprintf(stderr, "Exit gets loop - bDone=%d bSentXML=%d\n", bDone, bSentXML);
#endif
			if(pMaze->Solved() == false){
				fprintf(stdout, "wrong\n");
                exit(43); // GenXML("Wrong Answer", "not solved/EOF");
			}
		} else {
			::snprintf(&(szErr[0]), sizeof(szErr)-1, "Can not open maze file %s", szFile);
			exit(-1); //GenXML("Other - Contact Staff", &(szErr[0]));
		}
#ifdef DEBUG
	fprintf(stderr, "Exit main file loop - bSentXML=%d\n", bSentXML);
#endif
	return(42);
}

/*
precondition: n must be a positive integer
postcondition: s, xcoord, ycoord, down, right, visited are assigned values
*/
Maze::Maze(int nwidth, int nheight, int has_soln){
	int i, x, y, temp;

	width = nwidth;
	height= nheight;
	has_solution = has_soln;
	/*
	 * Border offsets
	 */
	xOffset = width/OFFSET_FACTOR;
	yOffset = height/OFFSET_FACTOR;

	size = height * width;
	for(i = 0; i < size; i++){
		tried.push_back(0);
		s.push_back(-1);
		x = i%width+1;
		y = height-i/width-1;
		xcoord.push_back(xOffset+x*CELL_UNIT);
		ycoord.push_back(yOffset+y*CELL_UNIT);
		temp = (i >= width*(height-1)) ? WALL_MUST_BE_UP : WALL_UP;
		down.push_back(temp);
		temp = (((i+1)%width) == 0) ? WALL_MUST_BE_UP : WALL_UP;
		right.push_back(temp);
		visited.push_back(NODE_NOTVISITED);
	}
	m_bSolved = false;
}
/*
precondition: n must be >= 0 and < s.size()
postcondition: return the root of n
*/
int Maze::find_root(int n){
	return s[n]<0 ? n : find_root(s[n]);
}
/*
precondition: root1 and root2 both must be >= 0 and < s.size()
postcondition: root1 and root2 belong to the same set
*/
void Maze::union_cell(int root1, int root2){
	s[find_root(root2)] = root1;
}

/*
precondition: victim should, but not must, be an element in lottery
postcondition: victim is erased from lottery
*/
void Maze::remove(int victim)
{
	vector<int>::iterator vi;
	for(vi = lottery.begin(); vi != lottery.end(); vi++){
		if(*vi == victim){
			lottery.erase(vi);
			return;
		}
	}
}

bool Maze::Build()
{
	int i, victim, neighbor, neighbor2;

	/* push all elements into a vector except the last one because it has no walls to knock down */
	for(i = 0; i < size-1; i++){
		lottery.push_back(i);
	}

	/* use a while loop to construct the maze */
	while(lottery.size() != 0){ 
		victim = lottery[rand() % lottery.size()];

		/* victim has two neighbors */
		if(down[victim] != 0 && right[victim] != 0){
			neighbor = victim+1;
			neighbor2 = victim+width;
			/* if neither of them is joined, pick one and knock down the mutual wall */
			if(find_root(neighbor) != find_root(victim) &&
				find_root(neighbor2) != find_root(victim)){
				if((rand() & 512) == 0){
					union_cell(victim, neighbor);
					right[victim] = WALL_DOWN;
				}
				else{
					union_cell(victim, neighbor2);
					down[victim] = WALL_DOWN;
				}
			} else if(find_root(neighbor) != find_root(victim)){
				/* if only one of them is joined, join another one and knock down the intersecting wall AND remove victim in vector lottery */
				union_cell(victim, neighbor);
				right[victim] = WALL_DOWN;
				remove(victim);
			} else if(find_root(neighbor2) != find_root(victim)){
				union_cell(victim, neighbor2);
				down[victim] = -1;
				remove(victim);
			} else {
				/* if both of them are joined, remove victim in vector lottery */
				remove(victim);
			}
			
		} else {
			/*victim has one neighbor*/
			/* determine which neighbor it is and if they are joined if not joined, join them and knock down the wall if joined, do nothing */
			if(((victim+1) % width) == 0){
				neighbor = victim+width;
				if(find_root(neighbor) != find_root(victim)){
					union_cell(victim, neighbor);
					down[victim] = WALL_DOWN;
				}
			} else{
				neighbor = victim+1;
				if(find_root(neighbor) != find_root(victim)){
					union_cell(victim, neighbor);
					right[victim] = WALL_DOWN;
				}
			}
			remove(victim);
		}
	}
	m_nCurPos = 0;
	m_bSolved = false;
	m_nMoves = 0;
	tried.resize(0);
	for(i = 0; i < size; i++){
		tried.push_back(0);
	}
	return(FindSol(0, size-1));
}

int Maze::MoveRight() 
{
	int nret = 0;
	if(m_nCurPos == size-1){
		if(right[m_nCurPos] != WALL_DOWN){
			nret = 0;
		} else {
			m_bSolved = 1;
			nret = 3;
		}
	} else {
		if(m_nCurPos < size){
			if(tried[m_nCurPos] & DIR_RIGHT){
				fprintf(stderr, "Already went right from %d\n", m_nCurPos);
				nret = 2;
			} else {
				tried[m_nCurPos] |= DIR_RIGHT;
				if(m_nCurPos < size && right[m_nCurPos] == WALL_DOWN){
					m_nCurPos++;
					nret = 1;
				}
			}
		}
	}
	return(nret);
}

int Maze::MoveLeft() 
{
	int nret = 0;

	if(m_nCurPos < size){
		if(tried[m_nCurPos] & DIR_LEFT){
			fprintf(stderr, "Already went left from %d\n", m_nCurPos);
			nret = 2;
		} else {
			tried[m_nCurPos] |= DIR_LEFT;
			if(m_nCurPos%width != 0 && m_nCurPos >= 1 && right[m_nCurPos-1] == WALL_DOWN){
				m_nCurPos--;
				nret = 1;
			}
		}
	}
	return(nret);
}

int Maze::MoveDown() 
{
	int nret = 0;
	int npos;

	if(m_nCurPos < size){
		if(tried[m_nCurPos] & DIR_DOWN){
			fprintf(stderr, "Already went down from %d\n", m_nCurPos);
			nret = 2;
		} else {
			tried[m_nCurPos] |= DIR_DOWN;
			npos = m_nCurPos - width;
			if(npos >= 0 && down[npos] == WALL_DOWN){
				m_nCurPos = npos;
				nret = 1;
			}
		}
	}
	return(nret);
}

int Maze::MoveUp() 
{
	int nret = 0;
	int npos;

	if(m_nCurPos < size){
		if(tried[m_nCurPos] & DIR_UP){
			fprintf(stderr, "Already went up from %d\n", m_nCurPos);
			nret = 2;
		} else {
			tried[m_nCurPos] |= DIR_UP;
			if(m_nCurPos >= 0 && down[m_nCurPos] == WALL_DOWN){
				nret = 1;
				npos = m_nCurPos + width;
				m_nCurPos = npos;
			}
		}
	}
	return(nret);
}
/*
 * precondition: from and to must be >= 0 and < numOfCells
 * postcondition: findSol terminates by either calling displaySol or running out; sol stores the solution to the maze
 */
bool Maze::FindSol(int from, int to)
{
	m_nMoves++;
	sol.push_back(from);
	visited[from] = 1;
	if(from == to){
		return(true);
	}
	/*right wall is knocked down*/
	if(from >= 0 && from < size && right[from] == WALL_DOWN){
		if(visited[from+1] != NODE_VISITED) {
			if(FindSol(from+1, to) == true){
				return(true);
			}
			sol.pop_back();
		}
	}
	/*lower wall is knocked down*/
	if(from >= 0 && from < size && down[from] == WALL_DOWN){
		if(visited[from+width] != NODE_VISITED) {
			if(FindSol(from+width, to) == true){
				return(true);
			}
			sol.pop_back();
		}
	}
	/*upper wall is knocked down*/
	if(from-width >= 1 && from-width < size && down[from-width] == WALL_DOWN){
		if(visited[from-width] != NODE_VISITED) {
			if(FindSol(from-width, to) == true){
				return(true);
			}
			sol.pop_back();
		}
	}
	/*left wall is knocked down*/
	if(from%width != 0 && from-1 >= 1 && from-1 < size && right[from-1] == WALL_DOWN){
		if(visited[from-1] != 1) {
			if(FindSol(from-1, to) == true){
				return(true);
			}
			sol.pop_back();
		}
	}
	return(false);
}

void Maze::Reset()
{
	m_nCurPos = 0;
	m_bSolved = false;
}

bool Maze::GetNext(char *&s, int &r, int &d)
{
	int v;

	if(*s >= '0' && *s <= '9'){
		v = (*s - '0');
	} else if(*s >= 'a' && *s <= 'f'){
		v = (*s - 'a' + 10);
	} else if(*s >= 'A' && *s <= 'F'){
		v = (*s - 'A' + 10);
	} else {
		return(false);
	}
	s++;
	d = (v >> 2) - 1;
	r = (v & 0x3) - 1;
	return(true);
}

Maze *Maze::Restore(const char *szFile)
{
	char szBuf[128];
	int nState = 0, w, h, r, d, cnt, nNode, has_soln;
	Maze *pm = NULL;
	bool bDone = false;
	FILE *fp = ::fopen(szFile, "rt");
	char *p;

	if(fp != NULL){
		while(bDone == false && ::fgets(&(szBuf[0]), sizeof(szBuf), fp) != NULL){
			switch(nState){
			case 0:
				if(::strnicmp(szBuf, "start ", 6) == 0 && sscanf(&(szBuf[6]), "%d %d %d", &(w), &(h), &(has_soln)) == 3){
					cnt = w*h;
					pm = new Maze(w, h, has_soln);
					if(pm != NULL){
						pm->Reset();
						nState = 1;
						nNode = 0;
					}
				}
				break;
			case 1:
				for(p = &(szBuf[0]); GetNext(p, r, d) == true; ){
					pm->right[nNode] = r;
					pm->down[nNode] = d;
//					fprintf(stdout, "Node %d: R=%d D=%d\n", nNode, r, d);
					nNode++;
					if(nNode >= cnt){
						bDone = true;
						break;
					}
				}
				break;
			}
		}
		::fclose(fp);
		if(nNode < cnt){
			fprintf(stderr, "Not enough points\n");
			delete pm;
			pm = NULL;
		}
	}
	return(pm);
}

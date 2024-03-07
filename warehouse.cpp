//Alexander Norman
//CS4332
/*
1
5 8
BBBB....
.###...X
.XX#...X
...#....
........
*/
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

struct Point{
    int x, y;
    Point(int x1, int y1){x = x1; y = y1;}
};

struct State{
    vector<Point> boxes;
    vector<Point> goals;
    vector<vector<char>> grid;
    int f, g, h;
    bool operator<(const State& other) const {//for priority queue
        return f > other.f;
    }
};

vector<vector<char>> readGrid(vector<Point>&, vector<Point>&);//read in grid into 2d vector
int estimateDistance (const Point&, const Point&);//calculates distance between 2 points
//int estimateH(const vector<Point> &, const vector<Point> &);//estimating heuristic. takes a box and finds the shortest distance to any goal
int estimateH2(const vector<Point> &, const vector<Point> &);//use this one, not the above one
bool moveDirection(State&, int, const char&);
string gridToString(const vector<vector<char>>&);//for storing in closed list
int aStar(const vector<vector<char>>&, const vector<Point>&, const vector <Point>&);
bool isStateGoal(const State&);

int main()
{
    int numTestCases;
    cin >> numTestCases;
    for(int i=0;i<numTestCases;i++)//runs for # of test cases
    {
        vector<Point> tempBox;
        vector<Point> tempGoal;
        vector<vector<char>> grid = readGrid(tempBox, tempGoal);//create 2d grid from input
        int count = aStar(grid, tempBox, tempGoal);
        cout << count << endl;

    }

    return 0;
}

int aStar(const vector<vector<char>>& grid, const vector<Point>& boxes, const vector <Point>& goals)
{
    State startState;//maybe clean this up later
    startState.boxes=boxes;
    startState.grid=grid;
    startState.goals=goals;
    startState.g = 0;
    startState.h = estimateH2(boxes, goals);

    priority_queue<State> open;//priorirty queue holdign states
    unordered_map<string, int> closed;//grids are stored as strings here

    open.push(startState);
    closed.insert(make_pair(gridToString(startState.grid), startState.g));


    vector<char> chars = {'N','E','S','W'};
    //int count = 0;
    while(!open.empty())
    {
        /*count++;
        cout << count << endl;*/

        State state = open.top();
        open.pop();

        /*if(state.h==0)//goal state, return depth
            return state.g;*/

        if(isStateGoal(state))//goal state, return depth
            return state.g;

        //vector<char> chars = {'N','E','S','W'};
        //generate children and push to open
        for(int i=0;i<state.boxes.size();i++)//for each box
        {
            for(int j=0;j<chars.size();j++)//each direction
            {
                State newState = state;
                if(moveDirection(newState, i, chars[j]))
                {
                    /*if(isStateGoal(newState))//goal state, return depth
                        return newState.g;*/
                    if(closed.count(gridToString(newState.grid)) > 0 ) // if closed contains the grid layout
                    {
                        //compare values
                        if(closed[gridToString(newState.grid)] <= newState.f)
                        {
                            //do nothing
                        }
                        else//push to open, update closed
                        {
                            closed[gridToString(newState.grid)] = newState.f;
                            open.push(newState);
                        }

                    }
                    else//if doesn't exist yet add to both
                    {
                        open.push(newState);
                        closed.insert(make_pair(gridToString(newState.grid), newState.f));
                    }
                }
            }
        }
        //cout << "DEPTH: " << state.g << " DISTANCE: " << state.h << endl;
    }
    return -1;
}

bool isStateGoal(const State& s)
{
    /*bool isGoal = true;
    for(int i=0;i<s.boxes.size();i++)
    {
        bool hasTarget = false;
        for(int j=0;j<s.goals.size();j++)
        {
            if(s.boxes[i].x == s.goals[j].x && s.boxes[i].y == s.goals[j].y)
                hasTarget = true;
        }
        if(!hasTarget)
            isGoal = false;
    }
    return isGoal;*/

    bool isGoal = true;
    for(int i=0;i<s.grid.size();i++)
    {
        for(int j=0;j<s.grid[i].size();j++)
        {
            if(s.grid[i][j] == 'X')
                isGoal = false;
        }
    }
    return isGoal;
}

string gridToString(const vector<vector<char>>& grid)
{
    string s="";
    for(int i=0;i<grid.size();i++)
    {
        for(int j=0;j<grid[i].size();j++)
            s+=grid[i][j];
    }
    return s;
}

bool moveDirection(State&s, int i, const char&c)
{
/*vector<vector<char>> grid;
    vector<Point> goals;
    vector<Point> boxes;
    vector<pair<Point, Point>> bgPairs;//box goal pairs
    int g, h;*/
    int x = s.boxes[i].x;
    int y = s.boxes[i].y;
    switch (c){
        case 'N':
            y-=1;
            break;
        case 'E':
            x+=1;
            break;
        case 'S':
            y+=1;
            break;
        case 'W':
            x-=1;
            break;
    }
    int height = s.grid.size();
    int width = s.grid[0].size();//making sure doesn't go out of grid
    if(x < 0 || x >= width || y < 0 || y >= height)
        return false;
    //now check for obstacles
    else if(s.grid[y][x] == 'B' || s.grid[y][x] == '#')
        return false;
    //check if box reached goal
    /*else if(s.grid[y][x] == 'X')
    {
        if(s.goals[i].x == x && s.goals[i].y == y)
        {
            s.grid[y][x] = 'B';
            s.grid[s.boxes[i].y][s.boxes[i].x] = '.';//update grid
            s.boxes[i].x = x;
            s.boxes[i].y = y;
            s.h = estimateH(s.boxes, s.goals);
            s.g+=1;
            s.f = s.g + s.h;
            return true;
        }



        return false;
    }*/
    //if no obstacles and valid direction, update state
    else
    {   
        s.grid[y][x] = 'B';
        s.grid[s.boxes[i].y][s.boxes[i].x] = '.';//update grid
        //if previous square should have been an X
        for(int j=0;j<s.goals.size();j++)
        {
            if(s.goals[j].x == s.boxes[i].x && s.goals[j].y == s.boxes[i].y)
                s.grid[s.boxes[i].y][s.boxes[i].x] = 'X';//update grid
        }
        s.boxes[i].x = x;
        s.boxes[i].y = y;
        s.h = estimateH2(s.boxes, s.goals);
        s.g+=1;
        s.f = s.g + s.h;
        return true;
    }
    return false;
}

vector<vector<char>> readGrid(vector<Point>&boxes, vector<Point>&goals)//read in a grid into 2d vector
{
    int height, width;
    cin >> height >> width;
    vector<vector<char>> grid (height, vector<char>(width));
    for(int i=0;i<height;i++)
    {
        string line;
        cin >> line;
        for(int j=0;j<width;j++)
        {
            char c = line[j];
            grid[i][j] = c;
            if(c == 'B')
            {
                Point p(j, i);
                boxes.push_back(p);
            }
            else if(c == 'X')
            {
                Point p(j, i);
                goals.push_back(p);
            }
        }
    }
    return grid;
}

int estimateH2(const vector<Point> &boxes, const vector<Point> &goals)
{
    int total = 0;
    for(int i=0;i<boxes.size();i++)
    {
        int smallNum = estimateDistance(boxes[i], goals[0]);
        for(int j=1;j<goals.size();j++)
        {
            if(estimateDistance(boxes[i], goals[j]) < smallNum)
                smallNum = estimateDistance(boxes[i], goals[j]);
        }
        total+=smallNum;
    }
    return total;
}

/*int estimateH(const vector<Point> &boxes, const vector<Point> &goals)
{
    int total = 0;
    for(int i=0;i<boxes.size();i++)
    {
        total+=estimateDistance(boxes[i], goals[i]);
    }
    return total;
}*/

int estimateDistance (const Point &box, const Point &goal)
{
    return abs(box.x - goal.x) + abs(box.y - goal.y);
}
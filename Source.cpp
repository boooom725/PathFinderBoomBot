#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Node.h"
#include <list>
#include <map>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <chrono>
#include <Windows.h>

using namespace std;

float Dist(int x1, int y1, int x2, int y2) {
    return (abs(x1 - x2) + abs(y1 - y2));
}

void PathFinder(Node grid[50][50], Node Start, Node End, sf::RenderWindow &window) {
    auto start = chrono::high_resolution_clock::now();

    vector<Node> Open;
    vector<Node> Closed;

    
    
    std::cout << "Start " << std::endl;

    // add start node
    sf::Vector2i pos = Start.GetPos();
    Start = grid[pos.y][pos.x];
    grid[pos.y][pos.x].Score = 0;
    Open.push_back(grid[pos.y][pos.x]);

    sf::Vector2i SPos = Start.GetPos();
    sf::Vector2i EPos = End.GetPos();




    while (Open.size() > 0) {

        Node best;
        int id = 0;
        for (int i = 0; i < Open.size(); i++) {
            if (Open[i].Score < best.Score) {
                id = i;
                best = Open[i];
            }
        }
        

        // add best to closed + remove from open + add neigbors to open
        Closed.push_back(best);
        

        Open.erase(Open.begin() + id);
        

        pos = best.GetPos();
        int row = pos.y;
        int col = pos.x;

        grid[row][col].SetClosed();

        if (EPos.y == row && EPos.x == col) {
            std::cout << "found" << endl;
            sf::Vector2i n = pos;
            while (n != SPos) {
                grid[n.y][n.x].SetPath();
                n = grid[n.y][n.x].CameFrom;
            }


            auto stop = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

            cout << duration.count() << " ms" << endl;

            return;
        }


        Node* cur;

        if (row + 1 < 50) {
            cur = &grid[row + 1][col];
            if (cur->GetType() != 4 && cur->GetType() != 1 && cur->GetType() != 5) {

                cur->Score = Dist(col, row + 1, SPos.x, SPos.y) + Dist(col, row + 1, EPos.x, EPos.y);
                cur->SetOpen();
                cur->CameFrom = pos;
                Open.push_back(*cur);

            }
        }
        

        if (row - 1 >= 0) {
            cur = &grid[row - 1][col];
            if (cur->GetType() != 4 && cur->GetType() != 1 && cur->GetType() != 5) {

                cur->Score = Dist(col, row - 1, SPos.x, SPos.y) + Dist(col, row - 1, EPos.x, EPos.y);
                cur->SetOpen();
                cur->CameFrom = pos;
                Open.push_back(*cur);
            }
        }

        if (col + 1 < 50) {
            cur = &grid[row][col + 1];
            if (cur->GetType() != 4 && cur->GetType() != 1 && cur->GetType() != 5) {

                cur->Score = Dist(col + 1, row, SPos.x, SPos.y) + Dist(col + 1, row, EPos.x, EPos.y);
                cur->SetOpen();
                cur->CameFrom = pos;
                Open.push_back(*cur);
            }
        }

        if (col - 1 >= 0) {
            cur = &grid[row][col - 1];
            if (cur->GetType() != 4 && cur->GetType() != 1 && cur->GetType() != 5) {

                cur->Score = Dist(col - 1, row, SPos.x, SPos.y) + Dist(col - 1, row, EPos.x, EPos.y);
                cur->SetOpen();
                cur->CameFrom = pos;
                Open.push_back(*cur);
            }
        }




        
        // draw grid
        
        for (int i = 0; i < 50; i++) {
            for (int j = 0; j < 50; j++) {
                grid[i][j].Draw(window);
            }
        }
        window.display();
        
    }

    std::cout << "end" << endl;

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    cout << duration.count() << " ms" << endl;
}


int main()
{
    bool rel = true;
    bool started = false;

    bool StartNode = false;
    bool EndNode = false;
    Node Start;
    Node End;
    

    Node grid[50][50];
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            grid[i][j] = Node(i,j,16);
        }
    }

    
    
    sf::RenderWindow window(sf::VideoMode(800, 800), "A* alg");

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i pos = sf::Mouse::getPosition(window);

            int row = pos.x / 16;
            int col = pos.y / 16;

            if (row >= 0 && row < 50 && col >= 0 && col < 50) {
                grid[row][col].SetWall();
            }
            
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (rel) {
                sf::Vector2i pos = sf::Mouse::getPosition(window);

                int row = pos.x / 16;
                int col = pos.y / 16;

                if (!StartNode) {
                    grid[row][col].SetStart();
                    Start = grid[row][col];
                    StartNode = true;
                    
                }
                else if (!EndNode) {
                    grid[row][col].SetEnd();
                    End = Node(row, col, 16);
                    EndNode = true;
                }
                else if(!started){
                    // Start !!!              
                    PathFinder(grid, Start, End, window);
                    started = true;
                }
                else {

                    started = false;
                    Node Start;
                    Node End;
                    StartNode = false;
                    EndNode = false;
                    for (int i = 0; i < 50; i++) {
                        for (int j = 0; j < 50; j++) {
                            grid[i][j] = Node(i, j, 16);
                        }
                    }
                }
                
            }
            rel = false;
        }
        else {
            rel = true;
        }


        // draw grid
        for (int i = 0; i < 50; i++) {
            for (int j = 0; j < 50; j++) {
                grid[i][j].Draw(window);
            }
        }

        window.display();
    }

    return 0;
}

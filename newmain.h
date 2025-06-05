#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <random>
#include <cstdlib>
#pragma once

class Set_up;   
class Player;   

class Board{
public:
    Board(std::vector<std::string> status, std::vector<std::string> pool)
      : Status(status), Pool(pool) {}

    std::vector<std::string> get_status() { return Status; }
    std::vector<std::string> get_pool() { return Pool; }

    void set_status(const std::vector<std::string>& status) { Status = status; } 
    void set_pool(const std::vector<std::string>& pool) { Pool = pool; } 

    void Print(){
        std::cout << "Status: ";
        for (const auto& s : Status) std::cout << s << " ";
        std::cout << "\nPool  : ";
        for (const auto& p : Pool) std::cout << p << " ";
        std::cout << "\n";
    }
  
private:   
    std::vector<std::string> Pool;
    std::vector<std::string> Status;
};

class Action {
public:
    Action(Board& b, Set_up& setup, std::vector<Player>& players)
        : board(b), setup(setup), players(players) {}

    void Take_Aim(int y){
            
        auto pool = board.get_pool();      
        auto status = board.get_status();  

        status[y-1] = "[X]"; 
        
        board.set_pool(pool);              
        board.set_status(status);   
    }

    void Shoot(int y){
        auto pool = board.get_pool();      
        auto status = board.get_status();  
            
        if (status[y - 1] != "[ ]") {            
            (pool).erase((pool).begin()+y-1);
            status[y - 1] = "[ ]";
        } 
        else {
            std::cout << "\nOpps, you miss the shot\n";
            status[y - 1] = "[ ]";
        }
    
        board.set_pool(pool);              
        board.set_status(status);  
    }

    void Quick_Shot(int y){        
        auto pool = board.get_pool();      
        auto status = board.get_status();  

        pool.erase(pool.begin()+y-1);
        status[y - 1] = "[ ]";

        board.set_pool(pool);              
        board.set_status(status);  
    }

    void Misfire(int y){        
        auto pool = board.get_pool();      
        auto status = board.get_status();  

        if(status[y-2] == "[X]" || status[y] == "[X]"){
            pool.erase(pool.begin()+y-1);
            status[y - 1] = "[ ]";
        }

        else if(status[y-2] == "[ ]" && status[y] == "[ ]"){
            std::cout << "\nOpps, you miss the shot\n";
        }

        board.set_pool(pool);              
        board.set_status(status);  
    }

    void Move_Ahead(int y){        
        auto pool = board.get_pool();      

        if(y == 6){
            std::cout << "\nOpps, can't move anywhere else.\n";
        }

        else if(y >= 1 && y < 6){
            std::swap(pool[y-1], pool[y]);
        }
       
        board.set_pool(pool);              

    }

    void Give_Peace_A_Chance(){        
        auto status = board.get_status();  

        for(int i = 0 ; i < 5;i++){
            status[i]="[ ]";
        }
        
        board.set_status(status);  
    }


private:
    Board& board;
    Set_up& setup;
    std::vector<Player>& players;
};

struct Duck {
    int id;
    int position;
    int status;

    Duck(int _id, int _pos = 0, int _sta = 0) : id(_id), position(_pos), status(_sta) {}
};

class Player{
public:

    Player(std::string playername) : _name(playername) {
        for (int i = 0; i < 6; ++i) {
            _ducks.push_back(Duck(i));
            _player_ducks_deck.push_back(std::vector<std::string>{playername, std::to_string(i)});
        }
    }

    std::vector<Duck> get_ducks() const { return _ducks; }
    std::string get_name() const { return _name; }
    std::vector<std::string> get_player_action_deck() const { return _player_action_deck; }
    std::vector<std::vector<std::string>> get_player_ducks_deck() const { return _player_ducks_deck; }

    void set_ducks(const std::vector<Duck>& ducks) { _ducks = ducks; }
    void set_name(const std::string& name) { _name = name; }
    void set_player_action_deck(const std::vector<std::string>& player_action_deck ) { _player_action_deck = player_action_deck ; }
    void set_player_ducks_deck(const std::vector<std::vector<std::string>>& player_ducks_deck) { _player_ducks_deck = player_ducks_deck; }

    //เรียกดูข้อมูลผู้เล่น
    void Print_player(){
        std::cout << "Deck:\n";
        for (const std::string& card : _player_action_deck) {
            std::cout << "  - " << card << "\n";
        }
    }

private:
    std::vector<Duck> _ducks;
    std::string _name ;
    std::vector<std::string> _player_action_deck;
    std::vector<std::vector<std::string>>_player_ducks_deck;
};

class Set_up{
public:
    
    Set_up() = default; 
    
    Set_up(const std::vector<std::vector<std::string>>& ducks_deck, const std::vector<std::string>& action_deck)
        : _ducks_deck(ducks_deck), _action_deck(action_deck) {}

    std::vector<std::vector<std::string>> get_ducks_deck() const { return _ducks_deck; }
    std::vector<std::string> get_action_deck() const { return _action_deck; }

    void set_ducks_deck(const std::vector<std::vector<std::string>>& ducks_deck) { _ducks_deck = ducks_deck; }
    void set_action_deck(const std::vector<std::string>& action_deck) {_action_deck = action_deck; }

    //สร้างกองการ์ดเป็ด สำหรับเรียกเข้าสู่สระน้ำ
    void get_allducks_from_players(const std::vector<Player>& players){
        _ducks_deck.clear();
        for (const Player& player : players) {
            const std::vector<std::vector<std::string>>& ducks = player.get_player_ducks_deck();
            _ducks_deck.insert(_ducks_deck.end(), ducks.begin(), ducks.end());
            }
    }

    //สับกองการ์ดเป็ด 
    void shuffle_ducks_deck(){
        std::random_device rd; 
        std::mt19937 g(rd());   
        std::shuffle(_ducks_deck.begin(), _ducks_deck.end(), g);
    }
    
    //สับกองการ์ดเเอคชั่น
    void shuffle_action_deck(){
        std::random_device rd; 
        std::mt19937 g(rd());   
        std::shuffle(_action_deck.begin(), _action_deck.end(), g);
    }

    //จั่วการ์ดให้ผู้เล่นคนละ 3 ใบ
    void draw_action_cards(std::vector<Player>& players, int card){
        for (Player& player : players) {
            auto a = get_action_deck(); // เรียกกองการ์ดแอคชั่น
            std::vector<std::string> player_deck = player.get_player_action_deck();
            for (int i = 0; i < card; ++i) {
                if (!_action_deck.empty()) { // เช็คก่อนจั่ว
                    std::string card = _action_deck.back();
                    _action_deck.pop_back();
                    player_deck.push_back(card);
                } else {
                    std::cout << "Action deck is empty! Cannot draw more cards.\n";
                    break;
                }
        }
            player.set_player_action_deck(player_deck);
        }
    }

   void fill_pool_from_ducks(Board& board) {
        std::vector<std::string> pool;
        for (const auto& duck : _ducks_deck) {
            pool.push_back(duck[0]);
            _ducks_deck.erase(_ducks_deck.begin());
            if (pool.size() == 6) break;
        }
        while (pool.size() < 6) pool.push_back(" ");
            board.set_pool(pool);
    }

    //setupเริ่มต้นเกมส์
    void set_up(std::vector<Player>& players , Board& board ) {
        get_allducks_from_players(players);
        shuffle_ducks_deck();
        shuffle_action_deck();
        draw_action_cards(players, 3);
        fill_pool_from_ducks(board);
    }

private:
    std::vector<std::vector<std::string>> _ducks_deck ={};
    // std::vector<std::string> _action_deck = {"Bump_Left_Bump_Right", "Bump_Left_Bump_Right", "Bump_Left_Bump_Right",
    //                                         "Take_Aim", "Take_Aim", "Take_Aim", "Take_Aim", "Take_Aim",
    //                                         "Shoot", "Shoot", "Shoot", "Shoot", "Shoot",
    //                                         "Give_Peace_A_Chance", "Give_Peace_A_Chance", "Give_Peace_A_Chance",
    //                                         "Double_Barrel", "Double_Barrel", "Double_Barrel",
    //                                         "Move_Ahead", "Move_Ahead", "Move_Ahead",
    //                                         "Fast_Forward", "Fast_Forward", "Fast_Forward",
    //                                         "Two_Birds", "Two_Birds", "Two_Birds",
    //                                         "Hang_Back", "Hang_Back", "Hang_Back",
    //                                         "Quick_Shot", "Quick_Shot", "Quick_Shot",
    //                                         "Misfire", "Misfire", "Misfire"};
    std::vector<std::string> _action_deck = {"Take_Aim", "Take_Aim", "Take_Aim", "Take_Aim", "Take_Aim",
                                            "Take_Aim", "Take_Aim", "Take_Aim", "Take_Aim", "Take_Aim",
                                            "Shoot", "Shoot", "Shoot", "Shoot", "Shoot",
                                            "Shoot", "Shoot", "Shoot", "Shoot", "Shoot",
                                            "Quick_Shot", "Quick_Shot", "Quick_Shot","Quick_Shot", "Quick_Shot",
                                            "Quick_Shot", "Quick_Shot", "Quick_Shot","Quick_Shot", "Quick_Shot", 
                                            "Give_Peace_A_Chance", "Give_Peace_A_Chance", "Give_Peace_A_Chance",
                                            "Misfire", "Misfire", "Misfire","Misfire", "Misfire", "Misfire",
                                            "Move_Ahead", "Move_Ahead", "Move_Ahead","Move_Ahead", "Move_Ahead", "Move_Ahead"
                                            };   
                                  
};
                                

class Playloop{
public:
    // สร้างลูบเงื่อนไขคือจนกว่าการ์ดเป็ดในบ่อจะเหลือ 1 ใบ
    // เรียกข้อมูลผู้เล่นได้ทีละ 1 คน
    // บังคับเล่นการ์ด 1 ใบ
    //  จั่วการ์ด 1 ใบ
  
    Playloop(const std::vector<std::vector<std::string>>& ducks_deck, const std::vector<std::string>& action_deck,
             Board& board, Set_up& setup , std::vector<Player>& players , std::vector<std::string> pool, std::vector<std::string> status)
        : board(board), setup(setup), players(players), pool(pool), status(status),
          _ducks_deck(ducks_deck), _action_deck(action_deck) {}
        
    Board get_board() const { return board; }
    Set_up get_setup() const { return setup; }  
    std::vector<Player> get_players() const { return players; }
    std::vector<std::string> get_actiondeck() const { return actiondeck; }
    std::vector<std::string> get_pool() const { return pool; }
    std::vector<std::string> get_status() const { return status; }
    std::vector<std::vector<std::string>> get_ducks_deck() const { return _ducks_deck; }
    std::vector<std::string> get_action_deck() const { return _action_deck; }
    void set_board(const Board& b) { board = b; }
    void set_setup(const Set_up& s) { setup = s; }
    void set_players(const std::vector<Player>& p) { players = p; }
    void set_actiondeck(const std::vector<std::string>& a) { actiondeck = a; }
    void set_pool(const std::vector<std::string>& p) { pool = p; }
    void set_status(const std::vector<std::string>& s) { status = s; }
    void set_ducks_deck(const std::vector<std::vector<std::string>>& ducks_deck) { _ducks_deck = ducks_deck; }
    void set_action_deck(const std::vector<std::string>& action_deck) { _action_deck = action_deck; }
    
    void run(std::vector<Player>& players, Set_up& setup ,Board& board , Action& action){
        int turn = 0;
        int chosen;
        int position;
    
    // setup.set_up(players , board); /*เรียก setup เริ่มต้นเกม (แจกเป็ด, สับ deck, แจกการ์ด action)*/

        
    while (board.get_pool().size() > 1) {

        board.Print(); /*แสดงสถานะของบอร์ด*/
        

        Player& current = players[turn % players.size()]; /*กำหนดตัวผู้เล่น*/


        std::cout << "\nCurrent Player: " << current.get_name() << "\n";
        

        /*บังคับเล่นการ์ด*/
        std::vector<std::string> actiondeck = current.get_player_action_deck();
        if (actiondeck.empty() == false){
            std::cout << "\nYour action deck: \n"; 
            
            for (int i = 0; i < actiondeck.size(); ++i) {
                std::cout << " [" << i+1 << "] " << actiondeck[i] << "\n";
            }

            std::cout  <<  "\nChoose your action card (1-3) : ";                        // get action card player want to play
            std::cin >> chosen;

            while(std::cin.fail() || chosen != 1 && chosen != 2 && chosen != 3) {        // check if the input is valid
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');     // discard invalid input               
                std::cout << "Invalid input. Please choose the right number of your action card.\n";
                std::cout  <<  "\nChoose your action card (1-3) : ";
                std::cin >> chosen; 
            }    

            std::string playedcard = actiondeck[chosen-1]; 

            if(playedcard == "Move_Ahead") {
                std::cout << "Choose the position to swap to (1-5) : ";               // get position to play the action card
                std::cin >> position;
            }
            else if(playedcard == "Give_Peace_A_Chance" ) {
                
                               // get position to play the action card
            }             // get position to play the action card
            else if(playedcard != "Move_Ahead" && playedcard != "Give_Peace_A_Chance"){
                std::cout << "Choose the position to play the card (1-6) : ";               // get position to play the action card
                std::cin >> position;
            }
            
            while(playedcard != "Give_Peace_A_Chance" && std::cin.fail() || position != 1 && position != 2 && position != 3 && position != 4 && position != 5 && position != 6) {// check if the input is valid 
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');     // discard invalid input
                std::cout << "Invalid input. Please choose the right position of your action card.\n";
                if(playedcard != "Move_Ahead"){
                    std::cout << "Choose the position to play the card (1-6) : ";               // get position to play the action card
                    std::cin >> position;
                }
                else if(playedcard == "Move_Ahead") {
                    std::cout << "Choose the position to swap to (1-5) : ";               // get position to play the action card
                    std::cin >> position;
                }
            }  

            // /*เล่นการ์ด*/
                                         // get the name of the action card
            if (playedcard == "Take_Aim") {
                action.Take_Aim(position); 
            }
            else if (playedcard == "Shoot") {
                action.Shoot(position);
            }   
            else if (playedcard == "Quick_Shot") {
                action.Quick_Shot(position);
            }
            else if (playedcard == "Misfire") {
                action.Misfire(position);
            }   
            else if (playedcard == "Move_Ahead") {
                action.Move_Ahead(position);
            }
            else if (playedcard == "Give_Peace_A_Chance") {
                action.Give_Peace_A_Chance();
            }     

            // ลบการ์ดที่เล่นออกจาก deck
            actiondeck.erase(actiondeck.begin() + (chosen - 1));
            current.set_player_action_deck(actiondeck);
        
            std::cout << "\n-------------------------------------------------\n" << std::endl;
            auto pool = board.get_pool();
            auto ducks_deck = setup.get_ducks_deck();
            if (pool.size() < 6) {
                auto duck = ducks_deck.back();
                std::string card = duck[0]; // หรือเลือกเฉพาะ duck[0] ก็ได้
                ducks_deck.pop_back();
                pool.push_back(card);
                setup.set_ducks_deck(ducks_deck);
                board.set_pool(pool);
            }

            board.Print(); /*แสดงสถานะของบอร์ด*/

            /*จั่วการ์ด */
            std::vector<std::string> globaldeck = setup.get_action_deck();
            if (!globaldeck.empty()) {
                std::string card = globaldeck.back();
                globaldeck.pop_back();
                actiondeck.push_back(card);
                current.set_player_action_deck(actiondeck);
                setup.set_action_deck(globaldeck);
                std::cout << "\nYou drew " << card << " card.\n";
            } else {
                std::cout << "\nAction deck is empty. No card drawn.\n";
            }
        }

        std::cout << "\nPlease type 'Next' to finish turn.\n";
        std::string ans;
        std::cin >> ans;
        while(ans != "Next") {
            std::cout << "Invalid input. Please type 'Next' to finish your turn.\n";
            std::cin >> ans; 
        }

        system("cls");                                 //clear terminal for next player
        
        std::cout << "\nNext player, Please type 'Ready' to start turn.\n";
        std::cin >> ans;
        while(ans != "Ready") {
            std::cout << "Invalid input. Please type 'Ready' to start your turn.\n";
            std::cin >> ans; 
        }    

        system("cls");                                 //clear terminal for next player    

        ++turn ; /*เปลี่ยนตัวคนเล่น*/
    }
    std::cout << "\nGame Over! The last duck in the pool is: " << board.get_pool().back() << "\n";
    std::cout << "Congratulations to the winner!\n";
    }

private:
    
    Board board;
    Set_up setup;
    std::vector<Player> players;
    std::vector<std::string> actiondeck;
    std::vector<std::string> pool;
    std::vector<std::string> status;
    std::vector<std::vector<std::string>> _ducks_deck;
    std::vector<std::string> _action_deck;
};




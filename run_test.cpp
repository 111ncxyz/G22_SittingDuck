#include "newmain.h"
#include <iostream>


int main() {
    // สร้างผู้เล่น
    std::vector<Player> players;
    players.emplace_back("PL1");
    players.emplace_back("PL2");
    players.emplace_back("PL3");

    // สร้างข้อมูลเริ่มต้นสำหรับ Board
    std::vector<std::string> status(6, "[ ]");
    std::vector<std::string> pool;

    Board board(status, pool);

    // สร้าง Set_up (ใช้ default deck)
    Set_up setup;
    
    // // setup เริ่มต้นเกม (แจกเป็ด, สับ deck, แจกการ์ด action)
    setup.set_up(players , board);



    // สร้าง Action โดยเชื่อมกับ board, setup, players
    Action action(board, setup, players);

    // สร้าง Playloop (ส่งค่าที่จำเป็นทั้งหมด)
    Playloop playloop(
        setup.get_ducks_deck(),
        setup.get_action_deck(),
        board,
        setup,
        players,
        pool,
        status
    );

    // เรียก run เพื่อเริ่มเกม
    playloop.run(players, setup, board, action);

    return 0;
}

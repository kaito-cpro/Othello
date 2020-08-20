#include <bits/stdc++.h>
using namespace std;

using P = pair<int, int>;
using vv = vector<vector<int>>;
const int SPACE = 0, HUMAN = 1, COMPUTER = 2;
int BOARD_SIZE = 8;  // <- must be even number!
vector<char> STONE{'-', 'O', 'X'};
vector<string> PLAYER{"", "You", "Computer"};

// 盤面の初期化
void init(vv& board, bool set_data=false) {
    if (set_data) {
        bool ok;
        // 盤面のサイズ指定
        do {
            ok = true;
            cout << " Board size (even number): ";
            string query;
            getline(cin, query);
            // クエリの解析
            int i = 0;
            while (i < (int)query.size()) {
                if (query[i] == ' ') query = query.erase(i);
                else ++i;
            }
            for (char c : query) if (!isdigit(c)) ok = false;
            if (!ok || stoi(query) % 2 || stoi(query) <= 4) {
                cout << " Error! Invalid input!" << endl;
                ok = false;
            }
            else BOARD_SIZE = stoi(query);
        } while (!ok);
        // ユーザーネーム指定
        do {
            ok = true;
            cout << " Your name: ";
            getline(cin, PLAYER[HUMAN]);
        } while (!ok);
        cout << endl;
    }

    board.resize(BOARD_SIZE + 2, vector<int>(BOARD_SIZE + 2));
    for (int x = 1; x <= BOARD_SIZE; ++x) {
        for (int y = 1; y <= BOARD_SIZE; ++y) {
            board[x][y] = SPACE;
        }
    }

    assert(BOARD_SIZE % 2 == 0);
    board[BOARD_SIZE/2][BOARD_SIZE/2] = board[BOARD_SIZE/2 + 1][BOARD_SIZE/2 + 1] = HUMAN;
    board[BOARD_SIZE/2][BOARD_SIZE/2 + 1] = board[BOARD_SIZE/2 + 1][BOARD_SIZE/2] = COMPUTER;

    STONE[HUMAN] = 'O';  STONE[COMPUTER] = 'X';  STONE[SPACE] = '-';
}

// 石の個数の集計
P count_stones(vv board) {
    int point_human = 0, point_computer = 0;
    for (int x = 1; x <= BOARD_SIZE; ++x) {
        for (int y = 1; y <= BOARD_SIZE; ++y) {
            if (board[x][y] == HUMAN) ++point_human;
            else if (board[x][y] == COMPUTER) ++point_computer;
        }
    }
    return P(point_human, point_computer);
}

// 盤面の表示
void show_board(vv board) {
    for (int x = 1; x <= BOARD_SIZE; ++x) {
        for (int y = 1; y <= BOARD_SIZE; ++y) {
            if (y == 1) cout << "     ";
            cout << " " << STONE[board[x][y]] << " ";
        }
        cout << endl;
    }
    cout << endl;

    int point_human = count_stones(board).first, point_computer = count_stones(board).second;
    cout << "   " << PLAYER[HUMAN] << ": " << setw(2) << right << point_human << endl;
    cout << "   " << PLAYER[COMPUTER] << ": " << setw(2) << right << point_computer << endl << endl;
}

// ルールの表示
void show_rule() {
    cout << endl
         << "  Welcome to Othello !!" << endl << endl
         << "  --- Rule -------------------------------------------------" << endl
         << " | * Your stone is \'" << STONE[HUMAN] << "\', and computer's stone is \'" << STONE[COMPUTER] << "\'         |" << endl
         << " | * In your turn, please select a position in this way:    |" << endl
         << " |     --> If you want to select (3, 5),                    |" << endl
         << " |         please type the two digits separated by space:   |" << endl
         << " |          ex)   3  5                                      |" << endl
         << "  ----------------------------------------------------------" << endl << endl;
}

// 結果の表示
void show_result(vv board) {
    show_board(board);
    int point_human = count_stones(board).first, point_computer = count_stones(board).second;
    cout << " Game Finish!" << endl;
    cout << " Result  -->  " << PLAYER[HUMAN] << ": " << setw(2) << right << point_human << endl;
    cout << "              " << PLAYER[COMPUTER] << ": " << setw(2) << right << point_computer << endl << endl;

    if (point_human > point_computer) cout << " Winner: " << PLAYER[HUMAN] << "!! Congratulations!!" << endl << endl;
    else if (point_human < point_computer) cout << " Winner: " << PLAYER[COMPUTER] << "... Let's try again!!" << endl << endl;
    else if (point_human == point_computer) cout << " Draw... Please play this game once again!" << endl << endl;
}

// 盤面から溢れていないか
bool is_in_board(P pos, vv board) {
    int x = pos.first, y = pos.second;
    return 1 <= x && x <= BOARD_SIZE && 1 <= y && y <= BOARD_SIZE;
}

// 座標に石を置けるか
bool can_put(P pos, vv board, int turn) {
    int x = pos.first, y = pos.second;
    if (!is_in_board(pos, board)) return false;
    if (board[x][y] != SPACE) return false;

    // 8方向のいずれかの先にプレイヤーと同じ石があれば true
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx, ny = y + dy;
            P npos = P(nx, ny);
            if (is_in_board(npos, board) && board[nx][ny] == (turn==HUMAN ? COMPUTER : HUMAN)) {
                while (is_in_board(npos, board) && board[nx][ny] != SPACE) {
                    nx += dx;  ny += dy;
                    npos = P(nx, ny);
                    if (board[nx][ny] == (turn==HUMAN ? HUMAN : COMPUTER)) return true;
                }
            }
        }
    }
    return false;
}

// ランダムな座標を生成
P get_random() {
    int min = 11, max = BOARD_SIZE*10 + BOARD_SIZE;
    int random_num = min + (int)(rand() * (max-min+1.0) / (1.0+RAND_MAX));  // min 以上 max 以下の乱数生成
    return P(random_num / 10, random_num % 10);
}

// 石を置く座標を選択
P select_pos(vv board, int turn) {
    P pos;
    bool ok = true;
    do {
        cout << ">>Player " << turn << ": " << PLAYER[turn] << endl;
        if (turn == COMPUTER) {
            // ランダムに座標を選択
            while (true) {
                pos = get_random();  // ランダムな座標を生成
                if (can_put(pos, board, turn)) break;
            }
        }
        else if (turn == HUMAN) {
            ok = true;
            cout << " Please select a position: ";
            string query;
            getline(cin, query);
            // クエリの解析
            string row = ""s, column = ""s;
            int i = 0, num = 1;
            while (i < (int)query.size() && ok) {
                if (query[i] != ' ') {
                    if (!isdigit(query[i])) ok = false;
                    while (i < (int)query.size() && query[i] != ' ' && ok) {
                        if (num == 1) row += query[i++];
                        else if (num == 2) column += query[i++];
                        else ok = false;
                    }
                    ++num;
                }
                else ++i;
            }
            if (ok && num == 3 && !row.empty() && !column.empty()) {
                int x = stoi(row), y = stoi(column);
                pos = P(x, y);
                if (!can_put(pos, board, turn)) ok = false;
            }
            else ok = false;

            if (!ok) cout << " Error! Invalid input!" << endl;
        }
    } while (!ok);
    cout << " " << PLAYER[turn] << " selected: (" << pos.first << ", " << pos.second << ")"<< endl;

    cout << endl;
    return pos;
}

// 石を置く
void put_stone(P pos, vv& board, int turn) {
    assert(can_put(pos, board, turn));
    int x = pos.first, y = pos.second;
    board[x][y] = turn;

    // 挟まれた石を裏返す
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx, ny = y + dy;
            P npos = P(nx, ny);
            if (is_in_board(npos, board) && board[nx][ny] == (turn==HUMAN ? COMPUTER : HUMAN)) {
                nx += dx;  ny += dy;
                while (is_in_board(npos, board) && board[nx][ny] == (turn==HUMAN ? COMPUTER : HUMAN)) {
                    nx += dx;  ny += dy;
                    npos = P(nx, ny);
                }
                if (is_in_board(npos, board) && board[nx][ny] == (turn==HUMAN ? HUMAN : COMPUTER)) {
                    // pos から npos までを裏返す
                    for (int k = 1; !(x + k*dx == nx && y + k*dy == ny); ++k) {
                        board[x + k*dx][y + k*dy] = turn;
                    }
                }
            }
        }
    }
}

// 手番をパス
void pass(int turn) {
    cout << ">>Player " << turn << ": " << PLAYER[turn] << " pass" << endl << endl;
}

// 終了判定
bool ended(vv board) {
    // いずれかのプレイヤーが置ける座標が存在するかどうか
    bool res = true;
    for (int x = 1; x <= BOARD_SIZE; ++x) {
        for (int y = 1; y <= BOARD_SIZE; ++y) {
            if (board[x][y] == SPACE) {
                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        if (dx == 0 && dy == 0) continue;
                        int nx = x + dx, ny = y + dy;
                        P npos = P(nx, ny);
                        if (!is_in_board(npos, board) || board[nx][ny] == SPACE) continue;
                        nx += dx, ny += dy;
                        npos = P(nx, ny);
                        while (is_in_board(npos, board) && board[nx][ny] == board[nx - dx][ny - dy]) {
                            nx += dx, ny += dy;
                            npos = P(nx, ny);
                        }
                        if (is_in_board(npos, board) && board[nx][ny] != SPACE) res = false;
                    }
                }
            }
        }
    }
    return res;
}

// 手番をプレイ
void play(vv& board, int turn) {
    // 置ける座標が存在するかどうか
    bool can_put_anywhere = false;
    for (int x = 1; x <= BOARD_SIZE; ++x) {
        for (int y = 1; y <= BOARD_SIZE; ++y) {
            P pos = P(x, y);
            if (can_put(pos, board, turn)) can_put_anywhere = true;
        }
    }
    if (can_put_anywhere) {
        P pos = select_pos(board, turn);
        put_stone(pos, board, turn);
    }
    else pass(turn);
}

int main() {
    vv board;  // 盤面(init() でリサイズする); 0: 空白, 1: Human, 2: Computer
    int turn = 1;  // 手番; 1: Human, 2: Computer

    show_rule();
    init(board, true);

    while (!ended(board)) {
        show_board(board);
        play(board, turn);
        turn = 3 - turn;  // 手番の交代
    }
    show_result(board);
}

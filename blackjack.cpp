//1211201118 林優花
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

// カードのマークを表す列挙型
enum Suit {  // enum 関連する定数値をグループ化。
    Spade, Heart, Diamond, Club
};

// カードを表す構造体
struct Card {  //カードの構造体
    Suit suit;  // マーク
    int rank;   // ランク
};

// カードの数値を記号のものは記号に変換
string rankToString(int rank) {
    if (rank == 1) return "A";
    else if (rank == 11) return "J";
    else if (rank == 12) return "Q";
    else if (rank == 13) return "K";
    else return to_string(rank);
}

// カードの記マークをきれいに表現
string cardToString(const Card& card) {
  string str = ""; //string 定義
  switch (card.suit) {
    case Spade:
      str += "♠";
      break;
    case Heart:
      str += "♥";
      break;
    case Diamond:
      str += "♦";
      break;
    case Club:
      str += "♣";
      break;
  }  
  str += rankToString(card.rank); //stringに記号入れる
  return str;
}

// トランプの山札を作成する関数
vector<Card> createDeck() {
  vector<Card> deck;
  for (int suit = Spade; suit <= Club; suit++) {
    for (int rank = 1; rank <= 13; rank++) {
      Card card = { static_cast<Suit>(suit), rank }; //二つの要素を保存できる‼　cardにはenum型のカードのマークを、int型のrankを入れている。
      deck.push_back(card);
      }
  }
  return deck;
}

// カードの配列をランダムに並び替える  
void shuffleDeck(vector<Card>& deck) {
  int n = deck.size();
  for (int i = 0; i < n; i++) {
    int j = rand() % (n - i) + i;
    swap(deck[i], deck[j]);
  }
}

// カードを引く
Card drawCard(vector<Card>& deck) {
  Card card = deck.back();
  deck.pop_back();
  return card;
}

// カードの合計値を計算
int calcHandValue(const vector<Card>& hand) {
  int value = 0;
  int aceCount = 0;
  for (const Card& card : hand) {
    if (card.rank >= 2 && card.rank <= 9) {
      value += card.rank;
    } else if (card.rank >= 10) {
      value += 10;
    } else {  // Ace
      value += 11;
      aceCount++;
    }
  }
  while (value > 21 && aceCount > 0) {
    value -= 10;
    aceCount--;
  }
  return value;
}

// hit or stand
char getPlayerAction() {
  char action;
  do {
    cout << "Hit or Stand? (h/s): ";
    cin >> action;
  } while (action != 'h' && action != 's');
  return action;
}

// player, dealerの状態表示
void printGameStatus(const vector<Card>& playerHand, const vector<Card>& dealerHand, bool isPlayerTurn, bool isDealerTurn, bool cases) { //countasterisk いらない
  //１ターン目しか表示してはいけないよ
  if (isPlayerTurn) {
    cout << "Player: ";
    for (const Card& card : playerHand) {
      cout << cardToString(card) << " ";
    }
    cout << "(" << calcHandValue(playerHand) << ")" << endl;
  } 
  if (isDealerTurn) {
    cout << "Dealer: ";
    if (!cases) {
      cout << cardToString(dealerHand[0]) << " ***" << endl;
    } else {
      for (const Card& card : dealerHand) {
        cout << cardToString(card) << " ";
      }
      cout << "(" << calcHandValue(dealerHand) << ")" << endl;
    }
  } 
  cout << endl;
}

// ゲームの結果を表示する関数
void printGameResult(const vector<Card>& playerHand, const vector<Card>& dealerHand) {
  int playerValue = calcHandValue(playerHand);
  int dealerValue = calcHandValue(dealerHand);
  //result 表示
  cout << endl;
  cout << "result" << endl;
  cout << "--------------------------" << endl;
  
  cout << "Player: ";
  for (const Card& card : playerHand) {
    cout << cardToString(card) << " ";
  }
  cout << "(" << calcHandValue(playerHand) << ")" << endl;
  
  cout << "Dealer: ";
  for (const Card& card : dealerHand) {
    cout << cardToString(card) << " "; // cardの要素を表示
  }
  cout << "(" << calcHandValue(dealerHand) << ")" << endl; 

  if (playerValue > 21) {
      cout << "Player busts! You lose." << endl;
  } else if (dealerValue > 21) {
    cout << "Dealer busts! You win." << endl;
  } else if (playerValue > dealerValue) {
    cout << "You win." << endl;
  } else if (playerValue < dealerValue) {
    cout << "You lose." << endl;
  } else {
    cout << "Pushed. It's a tie." << endl;
  }
}

// game play
void playBlackjack() {
  bool cases = false; // gamestatus 
  char action = ' ';
  vector<Card> deck = createDeck();
  shuffleDeck(deck);
  vector<Card> playerHand, dealerHand;
  // プレイヤーとディーラーが2枚ずつカードを引く
  playerHand.push_back(drawCard(deck));
  dealerHand.push_back(drawCard(deck));   
  playerHand.push_back(drawCard(deck));
  dealerHand.push_back(drawCard(deck));
  bool isPlayerTurn = true;
  bool isDealerTurn = true;

  while (isPlayerTurn) {
    printGameStatus(playerHand, dealerHand, isPlayerTurn, isDealerTurn, cases);
    isDealerTurn = false;
    cases = true;
    char action = getPlayerAction();
    if (action == 'h') {
      playerHand.push_back(drawCard(deck));
      if (calcHandValue(playerHand) > 21) {
        printGameStatus(playerHand, dealerHand, isPlayerTurn, isDealerTurn, cases);
        isPlayerTurn = false;
        isDealerTurn = false;     
      } else {
        isPlayerTurn = true;
        isDealerTurn = false;
      }
    } else if (action == 's') { //s
      isPlayerTurn = false;
      isDealerTurn = true;
    }
  } 
  
  while (isDealerTurn) {
    if (calcHandValue(dealerHand) < 17) { // 17未満なら引く
      dealerHand.push_back(drawCard(deck));  
    } else { // dealer turn 終了
      printGameStatus(playerHand, dealerHand, isPlayerTurn, isDealerTurn, cases);
      isDealerTurn = false;
      break;
    }
  }
  cases = false;
  printGameStatus(playerHand, dealerHand, isPlayerTurn, isDealerTurn, cases);
  printGameResult(playerHand, dealerHand);
}

int main() {
  srand(static_cast<unsigned int>(time(NULL)));
  playBlackjack();
  return 0;
}
   
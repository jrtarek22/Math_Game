#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <windows.h>
using namespace std;

// ==================== Enums ====================
enum enOp { Add = 1, Sub = 2, Mul = 3, Div = 4, Mixed = 5 };
enum enLevel { Easy = 1, Medium = 2, Hard = 3, MixLevel = 4 };

// ==================== Structs ====================
struct stQuestion {
    int num1;
    int num2;
    char op;
    double correctAnswer;
};

struct stResults {
    int right = 0;
    int wrong = 0;
    int total = 0;
};

// ==================== Color Helper ====================
void setColor(int colorCode) {
    string command = "color 0" + to_string(colorCode);
    system(command.c_str());
}

void resetColor() {
    system("color 07");
}

// ==================== Helpers ====================
int readPositiveNumber(string msg, int from, int to) {
    int num;
    do {
        cout << msg;
        cin >> num;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            num = -1;
        }
    } while (num < from || num > to);
    return num;
}

int randomNumber(int from, int to) {
    return rand() % (to - from + 1) + from;
}

// ==================== Question Generation ====================
int getNumberByLevel(enLevel level) {
    switch (level) {
    case Easy:     return randomNumber(1, 10);
    case Medium:   return randomNumber(10, 99);
    case Hard:     return randomNumber(100, 999);
    case MixLevel: return randomNumber(1, 999);
    }
    return 1;
}

char getRandomOperator() {
    char ops[] = { '+', '-', '*', '/' };
    return ops[randomNumber(0, 3)];
}

char getOperatorByChoice(enOp choice) {
    switch (choice) {
    case Add: return '+';
    case Sub: return '-';
    case Mul: return '*';
    case Div: return '/';
    case Mixed: return getRandomOperator();
    }
    return '+';
}

stQuestion generateQuestion(enLevel level, enOp op) {
    stQuestion q;
    q.num1 = getNumberByLevel(level);
    q.num2 = getNumberByLevel(level);
    q.op = getOperatorByChoice(op);

    if (q.op == '/' && q.num2 == 0) q.num2 = 1;
    if (q.op == '/') q.num1 = q.num1 * q.num2;

    switch (q.op) {
    case '+': q.correctAnswer = q.num1 + q.num2; break;
    case '-': q.correctAnswer = q.num1 - q.num2; break;
    case '*': q.correctAnswer = q.num1 * q.num2; break;
    case '/': q.correctAnswer = q.num1 / q.num2; break;
    }
    return q;
}

void printQuestion(stQuestion q, int index) {
    cout << "\nQuestion [" << index << "]: ";
    cout << q.num1 << " " << q.op << " " << q.num2 << " = ? ";
}

// ==================== Game Loop ====================
void playGame(int rounds, enLevel level, enOp op, stResults& res) {
    for (int i = 1; i <= rounds; i++) {
        stQuestion q = generateQuestion(level, op);
        printQuestion(q, i);

        double userAnswer;
        cin >> userAnswer;

        res.total++;

        if (userAnswer == q.correctAnswer) {
            setColor(2);   // أخضر
            cout << "Correct!\n";
            res.right++;
        }
        else {
            setColor(4);   // أحمر
            cout << "Wrong! Correct answer is: " << q.correctAnswer << endl;
            cout << "\a";
            res.wrong++;
        }
        resetColor();
    }
}

void printFinalResults(stResults res) {
    cout << "\n========================================\n";
    cout << "              GAME OVER                 \n";
    cout << "========================================\n";
    cout << "Total Questions : " << res.total << endl;
    cout << "Correct Answers : " << res.right << endl;
    cout << "Wrong Answers   : " << res.wrong << endl;
    cout << "----------------------------------------\n";

    double percentage = (res.total > 0) ? (res.right * 100.0 / res.total) : 0;
    cout << "Your Score      : " << fixed << setprecision(1) << percentage << "%\n";

    if (percentage >= 70) {
        setColor(2);
        cout << "Result: Excellent!\n";
    }
    else if (percentage >= 50) {
        setColor(6);
        cout << "Result: Good!\n";
    }
    else {
        setColor(4);
        cout << "Result: You need more practice.\n";
    }
    resetColor();
    cout << "========================================\n";
}

bool playAgain() {
    char c;
    cout << "\nPlay again? (Y/N): ";
    cin >> c;
    return (c == 'y' || c == 'Y');
}

// ==================== Main ====================
int main() {
    srand((unsigned)time(NULL));

    do {
        system("cls");
        resetColor();
        cout << "========================================\n";
        cout << "              MATH GAME                 \n";
        cout << "========================================\n\n";

        int rounds = readPositiveNumber("How many questions? (1-20): ", 1, 20);

        cout << "\nChoose Level:\n";
        cout << "  [1] Easy    (1 - 10)\n";
        cout << "  [2] Medium  (10 - 99)\n";
        cout << "  [3] Hard    (100 - 999)\n";
        cout << "  [4] Mix\n";
        int levelChoice = readPositiveNumber("Your choice: ", 1, 4);

        cout << "\nChoose Operation:\n";
        cout << "  [1] Add (+)\n";
        cout << "  [2] Subtract (-)\n";
        cout << "  [3] Multiply (*)\n";
        cout << "  [4] Divide (/)\n";
        cout << "  [5] Mixed\n";
        int opChoice = readPositiveNumber("Your choice: ", 1, 5);

        stResults res;
        playGame(rounds, (enLevel)levelChoice, (enOp)opChoice, res);
        printFinalResults(res);

    } while (playAgain());

    system("cls");
    resetColor();
    cout << "========== Thanks for playing ==========\n";
    return 0;
}
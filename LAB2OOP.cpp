#include <iostream>
#include <bitset>
#include <fstream>
#include <string>
const int MAX = 128;
using namespace std;

//Варіант 9. Завдання 1.
//Задано цілі числа a, c та d. Обчислити вираз без використання операцій множення та ділення(замінивши на їх операцій зсувів).
//(a*200 + c*312) / 16 - d * 120 + c * 124


void Task1() {
    int a, c, d, x, y;

    cout << "Enter a, c & d" << endl;
    cout << "a="; cin >> a; cout << "c="; cin >> c; cout << "d="; cin >> d;

    x = (((a << 7) + (a << 6) + (a << 3) + (c << 8) + (c << 5) + (c << 4) + (c << 3)) >> 4) - ((d << 6) + (d << 5) + (d << 4) + (d << 3)) + ((c << 6) + (c << 5) + (c << 4) + (c << 3) + (c << 2));
    y = (a * 200 + c * 312) / 16 - d * 120 + c * 124;

    cout << "x=" << x << endl << "y=" << y << endl;
}

//Задано текст до 128 символів. Доповнити пробілами до 128 символів. Шифрувати тексти таким чином, щоб кожний символ тексту записувався 
//у два байти. Два байти мають таку структуру :
// 
// 0 біт – біт парності(1 біт)
// у бітах 1 - 8 ASCII - код букви(8 біт),
// у бітах 9 - 15 позиція букви у рядку(7 біти)
void Task2_1(char c[]) {
    unsigned short Rez[128];
    unsigned short i, j, r, w, t, b;

    for (i = 0; i < 128; i++) {
        r = c[i];    /*код букви ASCII*/
        w = i;       /*позиція букви*/
        w <<= 8;
        r |= w;
        t = 1;
        b = 0;
        for (j = 0; j < 128; j++) {
            if (r & t) {
                if (b == 0) { b = 1; }    /*обчислення біта парності*/
                else { b = 0; }
            }
            t <<= 1;
        }
        r <<= 1;
        r |= b;  /* біт парності*/
        if (b == 1) { r |= 1; }
        Rez[i] = r;
    }
    for (i = 0; i < 128; i++) {
        std::bitset<16> x(Rez[i]);
        cout << x << endl;
    }

    ofstream bfile("Bin_File.bin", ios::out | ios::binary);
    if (bfile.fail()) return;
    for (i = 0; i < 128; i++) {
        bfile.write((char*)&Rez[i], sizeof(Rez[i]));
    }
    bfile.close();
}

//Зашифрований текст зчитується з файлу, розшифровує та виводить у інший файл і консоль. Структура шифру такаж сама, як і для попереднього завдання

void Task2_2(char c[]) {
    char Text[128];
    unsigned short Rez[128];
    unsigned short i, j, r, b, t;
    unsigned short ascii, position;

    ifstream bfile("Bin_File.bin", ios::in | ios::binary);
    if (bfile.fail()) return;
    for (i = 0; i < 128; i++) {
        bfile.read((char*)&Rez[i], sizeof(unsigned short));
    }
    bfile.close();
    for (i = 0; i < 128; i++) {
        r = Rez[i];
        b = 0;
        t = 1;
        for (j = 0; j < 15; j++) {
            if (r & t) {
                if (b == 0) { b = 1; }  /*перевірка біта парності*/
                else { b = 0; }
            } t <<= 1;
        }
        ascii = r & 0x1fe;      /*маска для коду букви ASCII 0000 0001 1111 1110*/
        ascii >>= 1;
        position = r & 0xfe00;   /*маска для позиції символу 1111 1110 0000 0000*/
        position >>= 9;
        if (position == 255) { // special value for space
            Text[i] = ' ';
        }
        else {
            Text[position] = ascii;
        }
        //Text[position] = ascii;  /*збирання шифру*/
    }

    for (i = 0; i < 128; i++) {
        cout << Text[i];
    }

    ofstream tfile("Txt_File_Final.txt");
    if (tfile.fail()) return;
    for (i = 0; i < 128; i++) {
        tfile << Text[i];
    }
    tfile.close();

}

int MainMenu() {
    int mainchoice;
    cout << "Main Menu\n";
    cout << "1. Task 1\n";
    cout << "2. Task 2: information from text file, result into binary file\n";
    cout << "3. Task 2: encrypted information from text file, decrypted information into console and other text file\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: " << endl;
    cin >> mainchoice;
    system("cls");
    return mainchoice;
}
void FirstInfIntoTextFile() {
    char c[129];
    unsigned short text_length, spaces_to_add, i;
    cout << "Enter the text: " << endl;
    cin.get(c, 128);
    text_length = strlen(c);
    spaces_to_add = 128 - text_length;
    for (i = text_length; i < 128; i++) {
        c[i] = ' ';
    }
    c[128] = '\0';
    ofstream tfile("Txt_File.txt");
    if (tfile.fail()) return;
    tfile << c;
    tfile.close();
}

int main()
{
    int mainchoice = MainMenu();
    if (mainchoice > 4 || mainchoice < 1) {
        cout << "Please choose task";
    }
    switch (mainchoice) {
    case 1: {
        Task1();
        break;
    }
    case 2: {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        FirstInfIntoTextFile();
        ifstream readfile("Txt_File.txt");
        char c[129];
        readfile.getline(c, 129);
        readfile.close();
        Task2_1(c);
        break;
    }
    case 3: {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        FirstInfIntoTextFile();
        ifstream readfile("Txt_File.txt");
        char c[128];
        readfile.getline(c, 128);
        readfile.close();
        Task2_1(c);
        cout << "\n";
        cout << "Your decrypted text: " << endl;
        Task2_2(c);
        break;
    }
    case 4: {
        cout << "Bye))";
        break;
    }
    default:
        break;
    }
    return 0;
}
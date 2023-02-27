#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <list>
const char* path = "D:/GhosperRifle";
#define DEBUG
using namespace std;
class Scene {
private:
    bool** dots;
    unsigned int height;
    unsigned int width;
public:
    Scene();
    Scene(unsigned int еheight, unsigned int еwidth);
    Scene(const Scene& a);
    ~Scene() { for (size_t i = 0; i < height; i++) delete[] dots[i]; delete[] dots; }

    Scene& operator=(const Scene& temp);
    bool operator==(const Scene& temp) const;
    void SaveBin() const;
    void LoadBin();
    unsigned int GetWidth() const { return width; }
    unsigned int GetHeight() const { return height; }
    void SetScene();
    void Init();
    void NextGen(Scene previous);
    bool GetDot(int x, int y) const { return dots[y][x]; }
    void Print() const;
    unsigned int GetAliveCount() const;
    void GetNeighbors(int nb[][2], unsigned int x, unsigned int y) const;
    unsigned short CountLiveNeighbors(unsigned int x, unsigned int y) const;
};
void HideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 200;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
void ShowCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 200;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
Scene::Scene(unsigned int еheight, unsigned int еwidth) : height(еheight), width(еwidth) { 
    dots = new bool* [height]; 
    for (size_t i = 0; i < height; i++) {
        dots[i] = new bool[width];
        for (size_t j = 0; j < width; j++)
            dots[i][j] = false;
    }
       
         
    COORD c({ 0, 0 });
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    cout << " Задати поле вручну\n Рандом генерація";
    SetConsoleCursorPosition(output, c);
    char a = _getch();
    while (true) {
        if (a == -32 and c.Y == 1)
            c.Y--;
        else if (a == -32 and c.Y == 0)
            c.Y++;
        else if ((a == ' ' or a == 13) and c.Y == 0) {
            SetScene();
            system("cls");
            return;
        }   
        else if ((a == ' ' or a == 13) and c.Y == 1) {
            Init();
            system("cls");
            return;
        }
        SetConsoleCursorPosition(output, c);
        Sleep(50);
        a = _getch();
    }
}
Scene::Scene() : height(10), width(10) { 
    dots = new bool* [height]; 
    for (size_t i = 0; i < height; i++) {
        dots[i] = new bool[width];
        for (size_t j = 0; j < width; j++)
            dots[i][j] = false;
    }
    COORD c({ 0, 0 });
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, c);
    char a = _getch();
    cout << " Задати поле вручну\n Рандом генерація";
    while (true) {
        if (a == -32 and c.Y == 1)
            c.Y--;
        else if (a == -32 and c.Y == 0)
            c.Y++;
        else if (a == ' ' and c.Y == 0)
            SetScene();
        else if ((a == ' ' or a == 13) and c.Y == 1)
            Init();
        SetConsoleCursorPosition(output, c);
        Sleep(50);
        a = _getch();
    }
}
Scene::Scene(const Scene& a) {
    height = a.GetHeight();
    width = a.GetWidth(); 
    dots = new bool* [height]; 
    for (int i = 0; i < height; i++) { 
        dots[i] = new bool[width]; 
        for (int j = 0; j < height; j++) 
            dots[i][j] = a.GetDot(j, i);
    } 
}

Scene& Scene::operator=(const Scene& temp)
{
    if (this != &temp)
        for (size_t i = 0; i < height; i++)
            for (size_t j = 0; j < width; j++)
                dots[i][j] = temp.GetDot(j, i);
    return *this;
}
bool Scene::operator==(const Scene& temp) const
{
    for (size_t i = 0; i < height; i++)
        for (size_t j = 0; j < width; j++)
            if (dots[i][j] != temp.GetDot(j, i))
                return false;
    return true;
}
void Scene::Init()
{
    unsigned short num = rand() % 1000 + 1;
    for (size_t i = 0; i < height; i++)
        for (size_t j = 0; j < width; j++) {
            if (num % 2 == 0)
                dots[i][j] = true;
            else
                dots[i][j] = false;
            num = rand() % 1000 + 1;
        }
}

void Scene::Print() const
{
    SetConsoleCP(8661);
    SetConsoleOutputCP(8661);
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            if (dots[i][j])
                cout << " * ";
            else
                cout << "   ";
        }
        cout << endl;
    }
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
}
unsigned int Scene::GetAliveCount() const
{
    unsigned int count = 0;
    for (size_t i = 0; i < height; i++)
        for (size_t j = 0; j < width; j++)
            if (dots[i][j])
                count++;
    return count;
}
void Scene::GetNeighbors(int nb[][2], unsigned int x, unsigned int y) const
{
    unsigned short k = 0;
    for (size_t i = y - 1; i <= y + 1; i++)
        for (size_t j = x - 1; j <= x + 1; j++) {
            if (i == y && j == x)
                continue;
            nb[k][0] = i;
            nb[k][1] = j;
            k++;
        }
}
unsigned short Scene::CountLiveNeighbors(unsigned int x, unsigned int y) const
{
    int count = 0;
    for (int i = y - 1; i <= y + 1; i++)
        for (int j = x - 1; j <= x + 1; j++) {
            if (i < 0 or j < 0 or j >= width or i >= height)
                continue;
            else if (i == y && j == x) {
                continue;
            }
            else if (dots[i][j])
                count++;
        }
    return count;
}
void Scene::NextGen(Scene previous)
{
    unsigned short livenb;
    bool p;
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            livenb = previous.CountLiveNeighbors(x, y);
            if (livenb == 3)
                dots[y][x] = true;
            else if (livenb < 2 || livenb > 3)
                dots[y][x] = false;
        }
    }
}
void LiveGame()
{
    unsigned int width, height, livepoints = 0;
    bool isoptimal = false;
    system("cls");
    cout << "Введіть ширину: ";
    cin >> width;
    cout << "Введіть висоту: ";
    cin >> height;
    system("cls");
    Scene world(height, width), prev(world);
    HideCursor();
    do
    {
        system("cls");
        world.Print();
        cout << "Нажміть любу клавішу для паузи" << endl;
        HideCursor();
        prev = world;
        world.NextGen(prev);
        if (_kbhit()) {
            _getch();
            Sleep(50);
            _getch();
        }
            

        isoptimal = (prev == world);
        livepoints = world.GetAliveCount();
        if (isoptimal) {
            cout << "Знайдена оптимальна генерація" << endl;
            system("pause");
            ShowCursor();
            return;
        }
        else if (livepoints == 0) {
            cout << "Всі точки мертві" << endl;
            system("pause");
            ShowCursor();
            return;
        }
        Sleep(100);

    } while (livepoints != 0 && !isoptimal);

}
void Scene::SetScene()
{
    COORD c({ 1, 1 });
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");
    cout << "Переміщення - WASD, пробіл - змінити стан клітинки, е - закінчити форматування, k - зберегти, l - загрузити\n";
    this->Print();
    SetConsoleCursorPosition(output, c);
    char a = _getch();
    while (a != 'e') {
        Sleep(10);
        if ((a == 'a' or a == 'A') and c.X > 2)
            c.X -= 3;
        else if ((a == 's' or a == 'S') and c.Y < height)
            c.Y++;
        else if ((a == 'd' or a == 'D') and c.X < (width * 3) - 1)
            c.X += 3;
        else if ((a == 'w' or a == 'W') and c.Y > 1)
            c.Y--;
        else if (a == ' ' or a == 13)
            dots[c.Y - 1][c.X / 3] = !dots[c.Y - 1][c.X / 3];
        else if (a == 'e')
            return;
        else if (a == 'k')
            this->SaveBin();
        else if (a == 'l')
            this->LoadBin();
        system("cls");
        cout << "Переміщення - WASD, пробіл - змінити стан клітинки, е - закінчити форматування, k - зберегти, l - загрузити\n";
        this->Print();
        SetConsoleCursorPosition(output, c);
        a = _getch();
    }
}
void Scene::SaveBin() const
{
    FILE* stream;
    fopen_s(&stream, path, "wb");
    fwrite(&height, sizeof(int), 1, stream);
    fwrite(&width, sizeof(int), 1, stream);
    for (size_t i = 0; i < height; i++)
        for (size_t j = 0; j < width; j++)
            fwrite(&dots[i][j], sizeof(bool), 1, stream);
    fclose(stream);
}
void Scene::LoadBin() 
{
    FILE* stream;
    fopen_s(&stream, path, "rb");
    fread(&height, sizeof(height), 1, stream);
    fread(&width, sizeof(width), 1, stream);
    for (size_t i = 0; i < height; i++) 
        delete[] dots[i]; 
    delete[] dots;
    dots = new bool*[height];
    for (size_t i = 0; i < height; i++)
    {
        dots[i] = new bool[width];
    }
    for (size_t i = 0; i < height; i++)
        for (size_t j = 0; j < width; j++)
            fread(&dots[i][j], sizeof(bool), 1, stream);
    fclose(stream);
}
int main()
{
    srand(time(NULL));
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    COORD c({ 0, 0 });
    ShowCursor();
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    cout << " Грати в гру життя\n Вихід";
    SetConsoleCursorPosition(output, c);
    char a = _getch();
    while (true) {
        if (a == -32 and c.Y == 1)
            c.Y--;
        else if (a == -32 and c.Y == 0)
            c.Y++;
        else if ((a == ' ' or a == 13) and c.Y == 0) {
            LiveGame();
            system("cls");
            cout << " Грати знову\n Вихід";
        }
        else if (a == ' ' and c.Y == 1)
            return 0;
        SetConsoleCursorPosition(output, c);
        Sleep(50);
        a = _getch();
    }
}
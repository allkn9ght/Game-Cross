#include <iostream>
#include <windows.h>
#include <cstring>

using namespace std;

struct god
{
    char name[4];
    int cross;
};

const int COUNT_OF_GODS = 4;
const int GODS_IN_BOAT = 2;
const int CROSS_LIMIT = 18;
int crossing(god *bank);

god left_bank[COUNT_OF_GODS];
god right_bank[COUNT_OF_GODS];
god boat[2];

bool ask_yes_no (const char *invitation);
bool all_in(god *bank, bool is_boat = false);
int select_god(const char * invitation, god * bank);
void init_state();
void load_unload(int i, god * fbank, god * tbank, bool from_boat = false);
void show_bank(god *bank, bool is_boat = false);
void show_state();


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    cout << "Игра - Переправа\n" << endl;
    cout << "Задание: необходимо переправить богов Ах, Бах, Вау и Гэ с левого берега реки\n"
    << "на правый. Каждый из богов по отдельности может переправится на лодке через реку\n"
    << "за определенное время: \n"
    << "Ах за 2 часа, Бах за 5 часов, Вау за 1 час, Гэ на 10 часов.\n"
    << "Лодка двухместная, при этом если два бога находится в лодке, то она плывет со\n"
    << "скоростью самого медленного из них. С правового берега необходимо чтобы кто-то\n"
    << "возращал лодку за ней, чтобы забрать оставшихся на левом берегу богов.\n"
    << "Задание считается выполненным если Вам удасться завершить переправу всех богов\n"
    << "за 18 часов или менее,в ином случае задание считается проваленным...\n" << endl;

    bool reply = ask_yes_no("Беретесь за это задание?");
    while(reply)
    {
        int total_time = 0;
        int time_left = 0;
        init_state();
        cout << "\nНачинаем переправу..\n";
        do{
            show_state();
            int i = select_god("Первый кто погрузится в лодку для переправы?", left_bank);
            cout << endl;
            load_unload(i, left_bank, boat);
            i = select_god("Второй кто погрузится в лодку для переправы?", left_bank);
            cout << endl;
            load_unload(i, left_bank, boat);
            show_state();
            int cross_time = crossing(right_bank);
            total_time = total_time + cross_time;
            time_left = CROSS_LIMIT - total_time;
            cout << " \n Лодка переправилась на правый берег за " << cross_time << " час(а|ов). \n"
                    "Общее время переправы " << total_time << " час(а|ов). Оставшееся время "
                    << time_left << " час(а|ов). \n";
            cout << endl;

            show_state();
            if (all_in(right_bank))
                break;

            i = select_god("Кто вернет лодку с правого берега на левый?", right_bank);
            load_unload(i, right_bank, boat);
            show_state();
            cross_time = crossing(left_bank);
            total_time = total_time + cross_time;
            time_left = CROSS_LIMIT - total_time;
            cout << " \n Лодка переправилась на левый берег за " << cross_time << " час(а|ов). \n"
                    "Общее время переправы " << total_time << " час(а|ов). Оставшееся время "
                    << time_left << " час(а|ов). \n";
            cout << endl;
        } while(true);
        if(total_time <= CROSS_LIMIT){
            cout << "Вы успешно справились с заданием! Боги благославляют Вас! \n";
            reply = false;
        } else{
            cout << "Вы переправили всех, но боги остались недовольны..\n\n";
            reply = ask_yes_no("Попробовать еще раз?");
        }
    }
    return 0;
}
bool ask_yes_no (const char *invitation){
    bool reply = false;
    char user_reply;
    do
    {
       cout << invitation << "(Д/Н): ";
       cin >> user_reply;
       cin.sync();
    } while (user_reply != 'Д' && user_reply != 'д' && user_reply != 'Н' && user_reply != 'н');

    if (user_reply == 'Д' || user_reply == 'д')
        reply = true;
    return reply;
}
bool all_in(god *bank, bool is_boat){
    bool reply = true;
    int lim = (is_boat)? GODS_IN_BOAT : COUNT_OF_GODS;
    for (int i = 0; i < lim; i++)
        if(!bank[i].cross) {
            reply = false;
            break;
        }
    return reply;
}
int select_god(const char * invitation, god * bank){
    int reply = -1;
    do {

        cout << " " << invitation << " (Имя): ";
        char user_reply[4];
        cin >> user_reply;
        if(user_reply[3]!= '\0')
            user_reply[3] = '\0';
        while(cin.get() != '\n');
        for(int i = 0; i < COUNT_OF_GODS; i++)
            if(bank[i].cross && !strcmp(bank[i].name,user_reply))
            {
             reply = i;
             break;
            }

    }while(reply < 0);
    return reply;
}
void init_state(){
    const char * god_names [4] = {"Ах", "Бах", "Вау", "Гэ"};
    int god_crosses [4] = {2, 5, 1, 10};
    for(int i =0; i < COUNT_OF_GODS; i++)
    {
        strncpy(left_bank[i].name, god_names[i], 4);
        left_bank[i].cross = god_crosses[i];

        strncpy(right_bank[i].name,"\0", 4);
        right_bank[i].cross = 0;

        if(i < GODS_IN_BOAT)
        {
            strncpy(boat[i].name,"\0", 4);
            boat[i].cross = 0;
        }
    }
}
void load_unload(int i, god * fbank, god * tbank, bool from_boat){
    int room = -1;
    int lim = (from_boat) ? COUNT_OF_GODS : GODS_IN_BOAT;
    for (int j =0; j < lim; j++)
    if(!tbank[j].cross){
            room = j;
            break;
    }
    if(room >= 0)
    {
        god nobody = {"\0", 0};
        tbank[room] = fbank[i];
        fbank[i] = nobody;
    }
}
int crossing(god *bank){
    int reply = 0;
    for (int i = 0; i < GODS_IN_BOAT; i++)
        if (boat[i].cross){
            if (boat[i].cross > reply)
                reply = boat[i].cross;
            load_unload(i, boat, bank, true);
        }
    return reply;
}
void show_bank(god *bank, bool is_boat){
    int c = 0;
    int lim = (is_boat)? GODS_IN_BOAT : COUNT_OF_GODS;
    for(int i = 0; i < lim; i++)
        if (bank[i].cross)
    {
        if(c)
            cout << ", ";
        cout << bank[i].name;
        c++;
    }
    if(!c)
    {
        cout << "Никого";
    }
}
void show_state(){
        cout << " |- Левый берег: ";
        show_bank(left_bank);
        cout << "; <|оо|> В лодке: ";
        show_bank(boat, true);
        cout << "; -| Правый берег: ";
        show_bank(right_bank);
        cout << endl;
}

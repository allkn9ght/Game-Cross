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

    cout << "���� - ���������\n" << endl;
    cout << "�������: ���������� ����������� ����� ��, ���, ��� � �� � ������ ������ ����\n"
    << "�� ������. ������ �� ����� �� ����������� ����� ������������ �� ����� ����� ����\n"
    << "�� ������������ �����: \n"
    << "�� �� 2 ����, ��� �� 5 �����, ��� �� 1 ���, �� �� 10 �����.\n"
    << "����� �����������, ��� ���� ���� ��� ���� ��������� � �����, �� ��� ������ ��\n"
    << "��������� ������ ���������� �� ���. � ��������� ������ ���������� ����� ���-��\n"
    << "�������� ����� �� ���, ����� ������� ���������� �� ����� ������ �����.\n"
    << "������� ��������� ����������� ���� ��� �������� ��������� ��������� ���� �����\n"
    << "�� 18 ����� ��� �����,� ���� ������ ������� ��������� �����������...\n" << endl;

    bool reply = ask_yes_no("�������� �� ��� �������?");
    while(reply)
    {
        int total_time = 0;
        int time_left = 0;
        init_state();
        cout << "\n�������� ���������..\n";
        do{
            show_state();
            int i = select_god("������ ��� ���������� � ����� ��� ���������?", left_bank);
            cout << endl;
            load_unload(i, left_bank, boat);
            i = select_god("������ ��� ���������� � ����� ��� ���������?", left_bank);
            cout << endl;
            load_unload(i, left_bank, boat);
            show_state();
            int cross_time = crossing(right_bank);
            total_time = total_time + cross_time;
            time_left = CROSS_LIMIT - total_time;
            cout << " \n ����� ������������� �� ������ ����� �� " << cross_time << " ���(�|��). \n"
                    "����� ����� ��������� " << total_time << " ���(�|��). ���������� ����� "
                    << time_left << " ���(�|��). \n";
            cout << endl;

            show_state();
            if (all_in(right_bank))
                break;

            i = select_god("��� ������ ����� � ������� ������ �� �����?", right_bank);
            load_unload(i, right_bank, boat);
            show_state();
            cross_time = crossing(left_bank);
            total_time = total_time + cross_time;
            time_left = CROSS_LIMIT - total_time;
            cout << " \n ����� ������������� �� ����� ����� �� " << cross_time << " ���(�|��). \n"
                    "����� ����� ��������� " << total_time << " ���(�|��). ���������� ����� "
                    << time_left << " ���(�|��). \n";
            cout << endl;
        } while(true);
        if(total_time <= CROSS_LIMIT){
            cout << "�� ������� ���������� � ��������! ���� ������������� ���! \n";
            reply = false;
        } else{
            cout << "�� ����������� ����, �� ���� �������� ����������..\n\n";
            reply = ask_yes_no("����������� ��� ���?");
        }
    }
    return 0;
}
bool ask_yes_no (const char *invitation){
    bool reply = false;
    char user_reply;
    do
    {
       cout << invitation << "(�/�): ";
       cin >> user_reply;
       cin.sync();
    } while (user_reply != '�' && user_reply != '�' && user_reply != '�' && user_reply != '�');

    if (user_reply == '�' || user_reply == '�')
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

        cout << " " << invitation << " (���): ";
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
    const char * god_names [4] = {"��", "���", "���", "��"};
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
        cout << "������";
    }
}
void show_state(){
        cout << " |- ����� �����: ";
        show_bank(left_bank);
        cout << "; <|��|> � �����: ";
        show_bank(boat, true);
        cout << "; -| ������ �����: ";
        show_bank(right_bank);
        cout << endl;
}

#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <unistd.h>

using namespace std;

class Decoder
{
private:
    string line;
    char *header;

    stack<string> word_list;
    stack<int> how_many_list;

    char *hex_check;
    char *word_StartPoint;
    char *word_EndPoint;

    string result;
    string previous;
    int bracket_num = 0;
    bool is_first = true;

public:
    Decoder(string str) : line(str)
    {
        header = &line[0];
        word_StartPoint = header;
    }
    ~Decoder()
    {
    }

    void print_before()
    {
        cout << line << endl;
    }
    void print_after()
    {
        cout << result << endl;
    }

    string return_word(char *start, char *end)
    {
        string word;
        for (start; start <= end; ++start)
        {
            word += *start;
        }
        return word;
    }

    int HexToDec(char c)
    {
        string s = "";
        s += c;
        return stoi(s, 0, 16);
    }

    bool is_valid()
    {
        char *ptr = &line[0];
        for (int i = 0; i < line.length(); i++)
        {
            if (*ptr == '{')
            {
                hex_check = ptr;
                if (isxdigit(*(--hex_check))) // { 왼쪽 값이 16진수인지 확인
                {
                    /*do nothing*/
                }
                else
                    return false; // 하나라도 삑사리 나면 에러
            }
            ++ptr;
        }

        return true;
    }

    void decode()
    {
        if (is_valid())
        {
            for (int i = 0; i < line.length(); ++i)
            {
                if (*header == '{')
                {
                    hex_check = header;
                    --hex_check;
                    how_many_list.push(HexToDec(*hex_check));

                    word_EndPoint = header;
                    word_EndPoint -= 2; // { 왼쪽은 16진수 값이므로 한칸 더 가야함
                    string word = return_word(word_StartPoint, word_EndPoint);
                    word_list.push(word);

                    word_StartPoint = header;
                    ++word_StartPoint;

                    ++bracket_num;
                }

                else if (bracket_num == 0)
                {
                    is_first = true;
                    char *check_next = header;
                    ++check_next;
                    if (*check_next == '{') // 중괄호가 없는 상태에서 16진수 값이 result에 추가되는 것 방지
                    {
                        /* do nothing */
                    }
                    else
                        result += *header;
                }

                else if (*header == '}')
                {
                    word_EndPoint = header;
                    word_EndPoint -= 1; // } 왼쪽까지
                    string right_word = return_word(word_StartPoint, word_EndPoint);
                    word_StartPoint = header;
                    ++word_StartPoint; // 다음 명령을 시행할 수 있게 } 오른쪽으로
                    --bracket_num;

                    int how_many = how_many_list.top();
                    how_many_list.pop();
                    if (is_first == true)
                    {
                        string temp;
                        for (int i = 0; i < how_many; ++i)
                            temp += right_word;
                        if (bracket_num == 0)
                            result += temp;
                        else
                            previous += temp;

                        is_first = false;
                    }

                    else
                    {
                        string left_word = word_list.top();
                        word_list.pop();

                        string temp;
                        for (int i = 0; i < how_many; ++i)
                        {
                            temp += left_word + previous + right_word;
                        }
                        previous = temp;
                        if (bracket_num == 0)
                        {
                            is_first = true; // 다 나왔다는 얘기므로 초기화
                            result += previous;
                        }
                    }
                }
                ++header;
            }
        }
        else
            result = "ERROR: Invalid input";
    }

    string get_result()
    {
        return result;
    }
};

int main(int argc, char **argv)
{

    ifstream data;
    ofstream out;

    char arr[256];
    getcwd(arr, 256);
    string input_dir(argv[1]);
    string current_dir(arr);
    string line;

    data.open(input_dir);
    out.open(current_dir + "\\test126.txt");

    while (getline(data, line))
    {
        Decoder test1(line);
        test1.decode();
        out << test1.get_result() << endl;
        // test1.print_all();
    }

    data.close();
    out.close();
}

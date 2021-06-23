#include <iostream>
#include <vector>
#include "histogram.h"
#include <curl/curl.h>
#include <sstream>
#include <string>
#include <windows.h>

using namespace std;

string make_info_text()
{
    stringstream buffer;
    DWORD WINAPI info = GetVersion();


    DWORD mask = 0x0000ffff;
    DWORD platform = info >> 16;
    DWORD version = info & mask;

    DWORD mask2 = 0x00ff;
    DWORD version_major = version >> 8;
    DWORD version_minor = version & mask2;

    DWORD build;
    if ((info & 0x10000000) == 0)
    {
        build = platform;
    }
    cout << "\n\nWindows v"<<version_minor<<"."<<version_major<<" (build "<<build <<")\n";


    char length[MAX_COMPUTERNAME_LENGTH+1];
    DWORD size;
    size=sizeof(length);
    GetComputerName(length,&size);


    cout << "Computer name: "<<length;
    return buffer.str();
}


struct Input
{
    size_t number_count;
    vector<double> numbers;
    size_t bin_count;
};

vector<double> input_numbers(istream& in, size_t count)
{
    vector<double> result(count);
    for (size_t i = 0; i < count; i++)
    {
        in >> result[i];
    }
    return result;
}

Input read_input(istream& in, bool prompt) {
    Input data;

    if (prompt) cerr << "Enter number count: ";
    size_t number_count;
    in >> number_count;

    if (prompt) cerr << "Enter numbers: ";
    data.numbers = input_numbers(in, number_count);

    if (prompt) cerr << "Enter bin count: ";
    in >> data.bin_count;

    return data;
}

vector<size_t> make_histogram(Input data)
{
    const auto bin_count = data.bin_count;
    const auto numbers = data.numbers;
    double min, max;
    size_t number_count = numbers.size();
    find_minmax(numbers, min, max);
    vector<size_t> bins(bin_count);
    double bin_size = (max - min) / bin_count;
    for (size_t i = 0; i < number_count; i++)
    {
        bool found = false;
        for (size_t j = 0; (j < bin_count - 1) && !found; j++)
        {
            auto lo = min + j * bin_size;
            auto hi = min + (j + 1) * bin_size;
            if ((lo <= numbers[i]) && (numbers[i] < hi))
            {
                bins[j]++;
                found = true;
            }
        }
        if (!found)
        {
            bins[bin_count - 1]++;
        }
    }
    return bins;
}

int show_histogram_text(const vector<size_t>& bins)
{
    const size_t SCREEN_WIDTH = 80;
    const size_t MAX_ASTERISK = SCREEN_WIDTH - 4 - 1;

    size_t max_count = bins[0];

    for (size_t bin : bins)
    {
        if (bin > max_count)
        {
            max_count = bin;
        }
    }

    const bool scaling_needed = max_count > MAX_ASTERISK;

    for (size_t bin : bins)
    {
        if (bin < 100)
        {
            cout << ' ';
        }
        if (bin < 10)
        {
            cout << ' ';
        }
        cout << bin << "|";

        size_t height = bin;
        if (scaling_needed)
        {
            const double scaling_factor = (double)MAX_ASTERISK / max_count;
            height = (size_t)(bin * scaling_factor);
        }

        for (size_t i = 0; i < height; i++)
        {
            cout << '*';
        }
        cout << '\n';
    }
    return max_count;
}

int shkala(const int &max_name, int &int_shkal, int &j)
{
    int kof_shkal = max_name/int_shkal + 1;
    cout << "   ";
    for (j=0; j<kof_shkal; j++)
    {
        cout<<"|";
        for (int z=0; z<int_shkal-1; z++)
        {
            cout<<"-";
        }
    }

    cout<<"|"<<endl;
    cout<<"   ";

    int chislo_shkal=-int_shkal;

    for (j=0; j<kof_shkal*int_shkal; j++)
    {
        chislo_shkal = chislo_shkal + int_shkal;

        if (chislo_shkal <= int_shkal || chislo_shkal == kof_shkal*int_shkal) //Проверка для вывода 1ого, 2ого и последнего числа под шкалой
        {
            cout<<chislo_shkal;
        }

        if (chislo_shkal < 10)
        {
            for (int z=0; z<int_shkal-1; z++)
            {
                cout<<" ";
            }
        }
        if ((chislo_shkal>=10)&&(chislo_shkal<100))
        {
            for (int z=0; z<int_shkal; z++)
            {
                cout<<" ";
            }
        }
        if (chislo_shkal>=100)
        {
            for (int z=0; z<int_shkal+1; z++)
            {
                cout<<" ";
            }
        }
    }
}

size_t write_data(void* items, size_t item_size, size_t item_count, void* ctx)
{

    auto data_size = item_size * item_count;
    stringstream* buffer = reinterpret_cast<stringstream*>(ctx);
    buffer->write(reinterpret_cast<char*>(items), data_size);
    return data_size;
}

Input download(const string& address)
{

    stringstream buffer;
    char *ip;

    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();

    if(curl)
    {
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_URL,  address.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            cout << address<<endl;
            cout << curl_easy_strerror(res);
            exit(1);
        }
        if((res == CURLE_OK) && !curl_easy_getinfo(curl, CURLINFO_PRIMARY_IP, &ip) && ip)
        {
            cerr << "IP: " <<  ip <<"\n";
        }
        curl_easy_cleanup(curl);
    }
    return read_input(buffer, false);
}

/*
void
svg_begin(double width, double height) {
    cout << "<?xml version='1.0' encoding='UTF-8'?>\n";
    cout << "<svg width='" << width << "' height='" << height << "' "
         << "viewBox='0 0 " << width << " " << height << "' "
         << "xmlns='http://www.w3.org/2000/svg'>\n";
}

void
svg_end() {
    cout << "</svg>\n";
}

void svg_text(double left, double baseline, string text)
{
    cout<<"<text x='"<<left<<"' y='"<<baseline<<"'>"<<text<<"</text>";
}

void svg_rect(double x, double y, double width, double height, string stroke, string fill )
{
    cout<< "<rect x='"<<x<<"' y='"<<y<<"' width='"<<width<<"' height='"<<height<<"' stroke='"<<stroke<<"' fill='"<<fill<<"' />";
}

void
show_histogram_svg(const vector<size_t>& bins) {
    const auto IMAGE_WIDTH = 400;
    const auto IMAGE_HEIGHT = 300;
    const auto TEXT_LEFT = 20;
    const auto TEXT_BASELINE = 20;
    const auto TEXT_WIDTH = 50;
    const auto BIN_HEIGHT = 30;
    svg_begin(IMAGE_WIDTH, IMAGE_HEIGHT);
    double top = 0;
for (size_t bin : bins) {
    const double bin_width = 10 * bin;
    svg_text(TEXT_LEFT, top + TEXT_BASELINE, to_string(bin));
    svg_rect(TEXT_WIDTH, top, bin_width, BIN_HEIGHT, "black", "red");
    top += BIN_HEIGHT;
}
    svg_end();

}
*/


int main(int argc, char* argv[])
{

    int int_shkal, j;

    cout << "Vvedite interval shkali (ot 2 do 9):";
    cin >> int_shkal;

    if (int_shkal<2 || int_shkal>9) //Проверка соответствия интервала шкалы по отношению к условию (от 2 до 9)
    {
        cout << endl << "ERROR" << endl;
    }
    else //Если введённый интервал соответствует условию, программа продолжает работу
    {
        Input input;
        if (argc > 1)
        {
            input = download(argv[1]);
        }
        else
        {
            input = read_input(cin, true);
        }

        const auto bins = make_histogram(input);
        const auto max_name = show_histogram_text(bins);

        shkala(max_name, int_shkal, j);

        make_info_text();

    }
    return 0;

}



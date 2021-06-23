#include <iostream>
#include <vector>
#include "histogram.h"
#include <curl/curl.h>

using namespace std;

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

Input
read_input(istream& in)
{
    Input data;

    cerr << "Enter number count: ";
    size_t number_count;
    in >> number_count;

    cerr << "Enter numbers: ";
    data.numbers = input_numbers(in, number_count);

    cerr << "Enter Bin Count: ";
    size_t bin_count;
    in >> bin_count;


    return data;
}

vector<size_t> make_histogram(Input)
{
    Input data2 = read_input(cin);

    double min, max;

    find_minmax(data2.numbers, min, max);

    vector<size_t> bins (data2.bin_count);
    for (double number : data2.numbers)
    {
        size_t bin = (size_t)((number - min) / (max - min) * data2.bin_count);
        if (bin == data2.bin_count)
        {
            bin--;
        }
        bins[bin]++;
    }
    return bins;
}

void show_histogram_text(const vector<size_t>& bins)
{
    const size_t SCREEN_WIDTH = 80;
    const size_t MAX_ASTERISK = SCREEN_WIDTH - 4 - 1;

    size_t max_count = 0;
    for (size_t count : bins)
    {
        if (count > max_count)
        {
            max_count = count;
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
    //return max_count;
}
/*
int shkala(size_t &max_name, int &int_shkal, int &j)
{
        int kof_shkal = max_name/int_shkal + 1;

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
int main()
{
    curl_global_init(CURL_GLOBAL_ALL);
    // Ввод данных
    int int_shkal;

    cout << "Vvedite interval shkali (ot 2 do 9):";

    cin >> int_shkal;

    if (int_shkal<2 || int_shkal>9) //Проверка соответствия интервала шкалы по отношению к условию (от 2 до 9)
    {
        cout << endl << "ERROR" << endl;
    }

    else //Если введённый интервал соответствует условию, программа продолжает работу
    {

    const auto input = read_input(cin);
    const auto bins = make_histogram(input);

    show_histogram_text(bins);

    // Обработка данных

    //size_t max_name=

    //cerr << "   ";

    //shkala(max_name, int_shkal, j);

    //show_histogram_svg(bins);
    }

    return 0;
}

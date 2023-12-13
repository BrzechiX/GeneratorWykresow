#include <iostream>
#include <fstream>
#include <sstream>

struct Bar{
    std::string date;
    double open;
    double high;
    double low;
    double close;
    double volume;
    bool color;
    void setColor(){
        color = close > open;
    }
};

class Diagram{
private:
    int bars = -1;
    std::string line;
    Bar* bar;
    double max = 0;
    double min = 1000000000;
    double amplitude;
public:
    int init(){
        std::ifstream file("aapl_us_d.csv");
        if(!file.is_open()) return 1;
        while(std::getline(file, line)) bars++;
        bar = new Bar[bars];
        file.clear();
        file.seekg(0, std::ios::beg);
        for(unsigned int i=0; std::getline(file, line); i++){
            if(i==0) continue;
            std::istringstream data(line);
            std::string position;
            std::getline(data, position, ',');
            bar[i-1].date = position;
            std::getline(data, position, ',');
            bar[i-1].open = std::stod(position);
            std::getline(data, position, ',');
            bar[i-1].high = std::stod(position);
            if(std::stod(position) > max) max = std::stod(position);
            std::getline(data, position, ',');
            bar[i-1].low = std::stod(position);
            if(std::stod(position) < min) min = std::stod(position);
            std::getline(data, position, ',');
            bar[i-1].close = std::stod(position);
            std::getline(data, position, ',');
            bar[i-1].volume = std::stod(position);
            bar[i-1].setColor();
        }
        amplitude = max - min;
        return 0;
    }
    ~Diagram(){
        delete[] bar;
    }
    int displayDiagram(){
        for(int i = 50; i > 0; i--){
            double v = min + i * (amplitude / 49);
            for(int j = 0; j < bars; j++){
                if(bar[j].color){
                    std::cout << "\u001b[32m";
                    if(bar[j].high <= v) std::cout << " ";
                    else if(bar[j].high > v && bar[j].close < v) std::cout << "\263";
                    else if(bar[j].open <= v && bar[j].close >= v) std::cout << "\333";
                    else if(bar[j].open > v && bar[j].low < v) std::cout << "\263";
                    else if(bar[j].low >= v) std::cout << " ";
                    else std::cout << "!";
                }
                else{
                    std::cout << "\u001b[31m";
                    if(bar[j].high <= v) std::cout << " ";
                    else if(bar[j].high > v && bar[j].open < v) std::cout << "\263";
                    else if(bar[j].open >= v && bar[j].close <= v) std::cout << "\333";
                    else if(bar[j].close > v && bar[j].low < v) std::cout << "\263";
                    else if(bar[j].low >= v) std::cout << " ";
                    else std::cout << "!";
                }
            }
            std::cout << std::endl;
        }
        std::cout << "\u001b[00m";
    }
};

int main(){
    Diagram diagram;
    if(diagram.init() == 0){
        diagram.displayDiagram();
        return 0;
    }
    else{
        std::cerr << "File does not exist!" << std::endl;
        return 1;
    }
}

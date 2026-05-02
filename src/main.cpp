#include "../include/WarGame.h"
#include "../include/FileReader.h"
#include <iostream>
#include <string>
#include <exception>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./war_game <input_csv> <output_csv>\n";
        return 1;
    }

    std::string inputCsv = argv[1];
    std::string outputCsv = argv[2];

    try {
        FileReader reader; 
        Deck deck = reader.readDeckFromCSV(inputCsv); 
        
        WarGame game(std::move(deck), outputCsv);
        
        game.play();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

#include "WarGame.h"
#include <iostream>

WarGame::WarGame(Deck deck, const std::string& outputPath) 
    : round_(1), writer_(outputPath) {
    auto [a, b] = deck.split();
    playerA_ = std::move(a);
    playerB_ = std::move(b);
}

void WarGame::play() {
    std::cout << "Starting War\n";
    
    while (!playerA_.empty() && !playerB_.empty()) {
        std::cout << "Round " << round_ << "\n";
        playRound();
        writer_.writeRound(round_, playerA_, playerB_);
        round_++;
    }
    
    std::cout << "Game Over\n";
    
    if (playerA_.empty() && playerB_.empty()) {
        std::cout << "It's a tie!\n";
    } else if (playerB_.empty()) {
        std::cout << "Player A wins" << std::endl;
    } else {
        std::cout << "Player B wins" << std::endl;
    }
}

void WarGame::playRound() {
    auto cardA = playerA_.draw();
    auto cardB = playerB_.draw();

    if (!cardA || !cardB) return;

    std::cout << "Player A plays: " << *cardA << "\n";
    std::cout << "Player B plays: " << *cardB << "\n";

    if (*cardB < *cardA || *cardB == *cardA) { 
        playerA_.addToBottom(std::move(cardA));
        playerA_.addToBottom(std::move(cardB)); 
    } else {
        playerB_.addToBottom(std::move(cardB));
        playerB_.addToBottom(std::move(cardA));
    }
}

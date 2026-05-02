#pragma once
#include "Card.h"
#include <deque>
#include <memory>
#include <iostream>

class Deck {
private:
    std::deque<std::unique_ptr<Card>> cards_;

public:
    Deck() = default;
    Deck(Deck&&) = default;
    Deck& operator=(Deck&&) = default;
    
    Deck(const Deck&) = delete; 
    Deck& operator=(const Deck&) = delete;

    size_t size() const;
    bool empty() const;

    auto begin();
    auto end();
    auto begin() const;
    auto end() const;

    std::unique_ptr<Card> draw();
    void addToBottom(std::unique_ptr<Card> card);
    std::pair<Deck, Deck> split();

    friend std::ostream& operator<<(std::ostream& os, const Deck& deck);
};

#include "FaceCard.h"

FaceCard::FaceCard(std::string suit, int rank) : PlayingCard(std::move(suit), rank) {}

void FaceCard::print(std::ostream& os) const {
    std::string rankStr;
    switch (rank_) {
        case 11:
            rankStr = "Jack"; 
            break;
        case 12: 
            rankStr = "Queen"; 
            break;
        case 13: 
            rankStr = "King"; 
            break;
        default:
            rankStr = std::to_string(rank_); 
            break; 
    }
    os << rankStr << " of " << suit_;
}

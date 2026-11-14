#include "Wallet.h"

#include <stdexcept>

Wallet::Wallet(int initialBalance) : balance(initialBalance) {
    if (initialBalance < 0) {
        throw std::invalid_argument("Wallet balance cannot start negative");
    }
}

int Wallet::getBalance() const {
    return balance;
}

bool Wallet::canAfford(int amount) const {
    return amount <= balance;
}

void Wallet::credit(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Cannot credit a negative amount");
    }
    balance += amount;
}

bool Wallet::debit(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Cannot debit a negative amount");
    }
    if (!canAfford(amount)) {
        return false;
    }
    balance -= amount;
    return true;
}

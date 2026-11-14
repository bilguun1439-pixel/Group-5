#ifndef COINPATH_WALLET_H
#define COINPATH_WALLET_H

// Wallet - the player's coin balance (FR3).
//
// Owns the one invariant every coin transaction must respect: the balance is
// never negative. Doors move coins only through credit() and debit(), so no
// other class can break that rule.
class Wallet {
public:
    // Throws std::invalid_argument if initialBalance is negative.
    explicit Wallet(int initialBalance);

    int getBalance() const;
    bool canAfford(int amount) const;

    // Adds coins. Throws std::invalid_argument if amount is negative.
    void credit(int amount);

    // Removes coins if the balance covers them. Returns false, and leaves the
    // balance unchanged, when it does not. Throws on a negative amount.
    bool debit(int amount);

private:
    int balance;
};

#endif // COINPATH_WALLET_H

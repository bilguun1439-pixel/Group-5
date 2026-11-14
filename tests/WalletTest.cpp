#include <gtest/gtest.h>

#include <stdexcept>

#include "Wallet.h"

TEST(WalletTest, StartsWithInitialBalance) {
    EXPECT_EQ(Wallet(5).getBalance(), 5);
}

TEST(WalletTest, RejectsNegativeInitialBalance) {
    EXPECT_THROW(Wallet(-1), std::invalid_argument);
}

TEST(WalletTest, DebitWithinBalanceSucceeds) {
    Wallet wallet(5);
    EXPECT_TRUE(wallet.debit(5));
    EXPECT_EQ(wallet.getBalance(), 0);
}

TEST(WalletTest, DebitBeyondBalanceFailsAndLeavesBalance) {
    Wallet wallet(2);
    EXPECT_FALSE(wallet.debit(3));
    EXPECT_EQ(wallet.getBalance(), 2);
}

TEST(WalletTest, CreditAddsCoins) {
    Wallet wallet(1);
    wallet.credit(4);
    EXPECT_EQ(wallet.getBalance(), 5);
}

TEST(WalletTest, NegativeAmountsAreRejected) {
    Wallet wallet(3);
    EXPECT_THROW(wallet.credit(-1), std::invalid_argument);
    EXPECT_THROW(wallet.debit(-1), std::invalid_argument);
    EXPECT_EQ(wallet.getBalance(), 3);
}

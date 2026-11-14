#include <gtest/gtest.h>

#include <stdexcept>

#include "Door.h"
#include "Wallet.h"

TEST(PaidDoorTest, OpensWhenAffordableAndCharges) {
    PaidDoor door({0, 0}, 3);
    Wallet wallet(5);
    EXPECT_TRUE(door.tryPass(wallet));
    EXPECT_TRUE(door.isOpen());
    EXPECT_EQ(wallet.getBalance(), 2);
}

TEST(PaidDoorTest, StaysClosedWhenUnaffordable) {
    PaidDoor door({0, 0}, 3);
    Wallet wallet(2);
    EXPECT_FALSE(door.tryPass(wallet));
    EXPECT_FALSE(door.isOpen());
    EXPECT_EQ(wallet.getBalance(), 2);
}

TEST(PaidDoorTest, OpenDoorIsFreeToPassAgain) {
    PaidDoor door({0, 0}, 3);
    Wallet wallet(3);
    ASSERT_TRUE(door.tryPass(wallet));
    EXPECT_TRUE(door.tryPass(wallet));
    EXPECT_EQ(wallet.getBalance(), 0);
}

TEST(RewardDoorTest, GivesCoinsOnlyOnce) {
    RewardDoor door({0, 0}, 4);
    Wallet wallet(0);
    EXPECT_TRUE(door.tryPass(wallet));
    EXPECT_TRUE(door.tryPass(wallet));
    EXPECT_EQ(wallet.getBalance(), 4);
}

TEST(DoorTest, CoinChangeIsSignedByKind) {
    EXPECT_EQ(PaidDoor({0, 0}, 3).getCoinChange(), -3);
    EXPECT_EQ(RewardDoor({0, 0}, 2).getCoinChange(), 2);
}

TEST(DoorTest, NonPositiveAmountsAreRejected) {
    EXPECT_THROW(PaidDoor({0, 0}, 0), std::invalid_argument);
    EXPECT_THROW(RewardDoor({0, 0}, -1), std::invalid_argument);
}

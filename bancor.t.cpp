#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <eosio/check.hpp>
#include <uint128_t/uint128_t.cpp>

#include "bancor.hpp"

TEST_CASE( "get_amount_out #1 (pass)" ) {
    // Inputs
    const uint64_t amount_in = 1000000;
    const uint64_t reserve_in = 578125412;
    const uint64_t reserve_out = 2170087186740517;
    const uint64_t reserve_weight_in = 500000;
    const uint64_t reserve_weight_out = 500000;
    const uint64_t fee = 2000;

    // Calculation
    const uint64_t amountOut = bancor::get_amount_out( amount_in, reserve_in, reserve_weight_in, reserve_out, reserve_weight_out, fee);

    REQUIRE( amountOut == 3732206312408 );
}

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
    const uint64_t amount_out = bancor::get_amount_out( amount_in, reserve_in, reserve_weight_in, reserve_out, reserve_weight_out, fee);

    REQUIRE( amount_out == 3732206312408 );
}

TEST_CASE( "get_amount_out #2 (pass)" ) {
    // Inputs
    const uint64_t amount_in = 10000;
    const uint64_t reserve_in = 45851931234;
    const uint64_t reserve_weight_in = 50000;
    const uint64_t reserve_out = 125682033533;
    const uint64_t reserve_weight_out = 50000;
    const uint64_t fee = 2000;

    // Calculation
    const uint64_t amount_out = bancor::get_amount_out( amount_in, reserve_in, reserve_weight_in, reserve_out, reserve_weight_out, fee);

    REQUIRE( amount_out == 27300 );
}

// TEST_CASE( "get_amount_in #1 (pass)" ) {
//     // Inputs
//     const uint64_t amount_out = 27328;
//     const uint64_t reserve_in = 45851931234;
//     const uint64_t reserve_weight_in = 50000;
//     const uint64_t reserve_out = 125682033533;
//     const uint64_t reserve_weight_out = 50000;
//     const uint64_t fee = 2000;

//     // Calculation
//     const uint64_t amount_in = bancor::get_amount_in( amount_out, reserve_in, reserve_weight_in, reserve_out, reserve_weight_out, fee );

//     REQUIRE( amount_in == 10000 );
// }

TEST_CASE( "quote #1 (pass)" ) {
    // Inputs
    const uint64_t amount_a = 10000;
    const uint64_t reserve_a = 45851931234;
    const uint64_t reserve_b = 125682033533;
    const uint64_t reserve_weight_a = 50000;
    const uint64_t reserve_weight_b = 50000;

    // Calculation
    const uint64_t amount_b = bancor::quote( amount_a, reserve_a, reserve_weight_a, reserve_b, reserve_weight_b );

    REQUIRE( amount_b == 27410 );
}

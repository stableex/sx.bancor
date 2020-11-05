#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <eosio/check.hpp>
#include <uint128_t/uint128_t.cpp>

#include "bancor.hpp"

TEST_CASE( "get_amount_out #1 (pass)" ) {
    // Inputs
    const uint64_t amount_in = 10000;
    const uint64_t reserve_in = 100000000;
    const uint64_t reserve_out = 400000000;
    const uint64_t reserve_weight_in = 500000;
    const uint64_t reserve_weight_out = 500000;

    // Calculation
    const uint64_t amountOut = bancor::get_amount_out( amount_in, reserve_in, reserve_weight_in, reserve_out, reserve_weight_out );

    REQUIRE( amountOut == 39876 );
}

TEST_CASE( "get_amount_out #2 (pass)" ) {
    // Inputs
    const uint64_t amount_in = 100000;
    const uint64_t reserve_in = 833515447;
    const uint64_t reserve_out = 10395237882;
    const uint64_t reserve_weight_in = 20;
    const uint64_t reserve_weight_out = 80;

    // Calculation
    const uint64_t amountOut = bancor::get_amount_out( amount_in, reserve_in, reserve_weight_in, reserve_out, reserve_weight_out );

    REQUIRE( amountOut == 310830 );
}

TEST_CASE( "get_amount_in (pass)" ) {
    // Inputs
    const uint64_t amount_out = 39876;
    const uint64_t reserve_in = 100000000;
    const uint64_t reserve_out = 400000000;
    const uint64_t reserve_weight_in = 500000;
    const uint64_t reserve_weight_out = 500000;

    // Calculation
    const uint64_t amountIn = bancor::get_amount_in( amount_out, reserve_in, reserve_weight_in, reserve_out, reserve_weight_out );

    REQUIRE( amountIn == 10000 );
}

TEST_CASE( "quote (pass)" ) {
    // Inputs
    const uint64_t amount_a = 10000;
    const uint64_t reserve_a = 100000000;
    const uint64_t reserve_b = 400000000;
    const uint64_t reserve_weight_a = 500000;
    const uint64_t reserve_weight_b = 500000;

    // Calculation
    const uint64_t amount_b = bancor::quote( amount_a, reserve_a, reserve_weight_a, reserve_b, reserve_weight_b );

    REQUIRE( amount_b == 40000 );
}
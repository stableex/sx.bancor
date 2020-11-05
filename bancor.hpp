#pragma once

#include <sx.safemath/safemath.hpp>
#include <math.h>

namespace bancor {

    struct [[eosio::table("settings")]] settings {
        name            smart_contract;
        asset           smart_currency;
        bool            smart_enabled;
        bool            enabled;
        name            network;
        bool            require_balance;
        uint64_t        max_fee;
        uint64_t        fee;

        uint64_t primary_key() const { return currency.code().raw(); }
    };

    struct [[eosio::table("reserves")]] reserves {
        name        contract;
        asset       currency;
        uint64_t    ratio;
        bool        p_enabled

        uint64_t primary_key() const { return currency.code().raw(); }
    };

    struct [[eosio::table("converter.v2")]] converter {
        symbol                              currency;
        name                                owner;
        uint64_t                            fee;
        map<symbol_code, uint64_t>          reserve_weights;
        map<symbol_code, extended_asset>    reserve_balances;
        map<name, bool>                     protocol_features;
        map<name, string>                   metadata_json;

        uint64_t primary_key() const { return currency.code().raw(); }
    };

    /**
     * ## STATIC `get_amount_out`
     *
     * Given an input amount of an asset and pair reserves, returns the maximum output amount of the other asset
     *
     * ### params
     *
     * - `{uint64_t} amount_in` - amount input
     * - `{uint64_t} reserve_in` - reserve input
     * - `{uint64_t} reserve_weight_in` - reserve input weight
     * - `{uint64_t} reserve_out` - reserve output
     * - `{uint64_t} reserve_weight_out` - reserve output weight
     * - `{uint8_t} fee` - (optional) trading fee (pips 1/100 of 1%)
     *
     * ### example
     *
     * ```c++
     * // Inputs
     * const uint64_t amount_in = 10000;
     * const uint64_t reserve_in = 45851931234;
     * const uint64_t reserve_weight_in = 50000;
     * const uint64_t reserve_out = 125682033533;
     * const uint64_t reserve_weight_out = 50000;
     * const uint8_t fee = 30;
     *
     * // Calculation
     * const uint64_t amount_out = bancor::get_amount_out( amount_in, reserve_in, reserve_weight_in, reserve_out, reserve_weight_out );
     * // => 27328
     * ```
     */
    static uint64_t get_amount_out( const uint64_t amount_in, const uint64_t reserve_in, const uint64_t reserve_weight_in, const uint64_t reserve_out, const uint64_t reserve_weight_out, const uint8_t fee )
    {
        // checks
        eosio::check(amount_in > 0, "sx.bancor: INSUFFICIENT_INPUT_AMOUNT");
        eosio::check(reserve_in > 0 && reserve_out > 0, "sx.bancor: INSUFFICIENT_LIQUIDITY");
        eosio::check(reserve_weight_in > 0 && reserve_weight_out > 0, "sx.bancor: INVALID_WEIGHT");

        // calculations
        const double weight_ratio = (static_cast<double>(reserve_weight_in) / reserve_weight_out);
        const double amount_in_with_fee = amount_in * (10000 - fee);
        const double numerator = (reserve_in * 10000) / ((reserve_in * 10000) + amount_in_with_fee);
        const double denominator = 1 - pow(numerator, weight_ratio);
        const uint64_t amount_out = reserve_out * denominator;

        return amount_out;
    }

    /**
     * ## STATIC `get_amount_in`
     *
     * Given an output amount of an asset and pair reserves, returns a required input amount of the other asset.
     *
     * ### params
     *
     * - `{uint64_t} amount_out` - amount input
     * - `{uint64_t} reserve_in` - reserve input
     * - `{uint64_t} reserve_weight_in` - reserve input weight
     * - `{uint64_t} reserve_out` - reserve output
     * - `{uint64_t} reserve_weight_out` - reserve output weight
     * - `{uint8_t} [fee=30]` - (optional) trading fee (pips 1/100 of 1%)
     *
     * ### example
     *
     * ```c++
     * // Inputs
     * const uint64_t amount_out = 27328;
     * const uint64_t reserve_in = 45851931234;
     * const uint64_t reserve_weight_in = 50000;
     * const uint64_t reserve_out = 125682033533;
     * const uint64_t reserve_weight_out = 50000;
     * const uint8_t fee = 30;
     *
     * // Calculation
     * const uint64_t amount_in = bancor::get_amount_in( amount_out, reserve_in, reserve_weight_in, reserve_out, reserve_weight_out, fee );
     * // => 10000
     * ```
     */
    static uint64_t get_amount_in( const uint64_t amount_out, const uint64_t reserve_in, const uint64_t reserve_weight_in, const uint64_t reserve_out, const uint64_t reserve_weight_out, const uint8_t fee = 30 )
    {
        // checks
        eosio::check(amount_out > 0, "sx.bancor: INSUFFICIENT_OUTPUT_AMOUNT");
        eosio::check(reserve_in > 0 && reserve_out > 0, "sx.bancor: INSUFFICIENT_LIQUIDITY");

        const double weight_ratio = static_cast<double>(reserve_weight_in) / reserve_weight_out;
        const double numerator = static_cast<double>(reserve_in) *  amount_out * 10000;
        const double denominator = ((reserve_out - amount_out) * (10000 - fee)) * weight_ratio;
        const uint64_t amount_in = 1 + ( numerator / denominator);

        return amount_in;
    }

    /**
     * ## STATIC `quote`
     *
     * Given some amount of an asset and pair reserves, returns an equivalent amount of the other asset
     *
     * ### params
     *
     * - `{uint64_t} amount_a` - amount A
     * - `{uint64_t} reserve_a` - reserve A
     * - `{uint64_t} reserve_weight_a` - reserve A weight
     * - `{uint64_t} reserve_b` - reserve B
     * - `{uint64_t} reserve_weight_b` - reserve B weight
     *
     * ### example
     *
     * ```c++
     * // Inputs
     * const uint64_t amount_a = 10000;
     * const uint64_t reserve_a = 45851931234;
     * const uint64_t reserve_b = 125682033533;
     * const uint64_t reserve_weight_a = 50000;
     * const uint64_t reserve_weight_b = 50000;
     *
     * // Calculation
     * const uint64_t amount_b = bancor::quote( amount_a, reserve_a, reserve_weight_a, reserve_b, reserve_weight_b );
     * // => 27410
     * ```
     */
    static uint64_t quote( const uint64_t amount_a, const uint64_t reserve_a, const uint64_t reserve_weight_a, const uint64_t reserve_b, const uint64_t reserve_weight_b )
    {
        eosio::check(amount_a > 0, "sx.bancor: INSUFFICIENT_AMOUNT");
        eosio::check(reserve_a > 0 && reserve_b > 0, "sx.bancor: INSUFFICIENT_LIQUIDITY");
        const uint64_t amount_b = safemath::mul(amount_a, reserve_b * 10000 / reserve_weight_b) / (reserve_a * 10000 / reserve_weight_a);
        return amount_b;
    }
}
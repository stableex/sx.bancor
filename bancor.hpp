#pragma once

#include <eosio.token/eosio.token.hpp>
#include <eosio/singleton.hpp>
#include <sx.safemath/safemath.hpp>
#include <math.h>

namespace bancor {
    using namespace eosio;
    using namespace std;
    using eosio::name;
    using eosio::singleton;

    struct [[eosio::table("settings")]] settings_row {
        name            smart_contract;
        asset           smart_currency;
        bool            smart_enabled;
        bool            enabled;
        name            network;
        bool            require_balance;
        uint64_t        max_fee;
        uint64_t        fee;

        //uint64_t primary_key() const { return smart_contract.value; }
    };
    typedef eosio::singleton< "settings"_n, settings_row > settings;

    struct [[eosio::table("reserves")]] reserves_row {
        name        contract;
        asset       currency;
        uint64_t    ratio;
        bool        p_enabled;

        uint64_t primary_key() const { return contract.value; }
    };
    typedef eosio::multi_index< "reserves"_n, reserves_row > reserves;

    struct [[eosio::table("converter.v2")]] converter_row {
        symbol                              currency;
        name                                owner;
        uint64_t                            fee;
        map<symbol_code, uint64_t>          reserve_weights;
        map<symbol_code, extended_asset>    reserve_balances;
        map<name, bool>                     protocol_features;
        map<name, string>                   metadata_json;

        uint64_t primary_key() const { return currency.code().raw(); }
    };
    typedef eosio::multi_index< "converter.v2"_n, converter_row > converter;

    const string _bancorcnvrtr_act = "bancorcnvrtr";

    /**
     * ## STATIC `get_amount_out`
     *
     * Given an input amount of an asset and pair reserves, returns the output amount of the other asset
     *
     * ### params
     *
     * - `{uint64_t} amount_in` - amount input
     * - `{uint64_t} reserve_in` - reserve input
     * - `{uint64_t} reserve_weight_in` - reserve input weight
     * - `{uint64_t} reserve_out` - reserve output
     * - `{uint64_t} reserve_weight_out` - reserve output weight
     * - `{uint64_t} fee` - (optional) trading fee (pips 1/10000 of 1%)
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
     * const uint64_t fee = 2000;
     *
     * // Calculation
     * const uint64_t amount_out = bancor::get_amount_out( amount_in, reserve_in, reserve_weight_in, reserve_out, reserve_weight_out );
     * // => 27328
     * ```
     */
    static uint64_t get_amount_out( const uint64_t amount_in, const uint64_t reserve_in, const uint64_t reserve_weight_in, const uint64_t reserve_out, const uint64_t reserve_weight_out, const uint64_t fee )
    {
        // checks
        eosio::check(amount_in > 0, "sx.bancor: INSUFFICIENT_INPUT_AMOUNT");
        eosio::check(reserve_in > 0 && reserve_out > 0, "sx.bancor: INSUFFICIENT_LIQUIDITY");
        eosio::check(reserve_weight_in > 0 && reserve_weight_out > 0, "sx.bancor: INVALID_WEIGHT");

        // calculations
        double weight_ratio = static_cast<double>(reserve_weight_in) / reserve_weight_out;
        auto amount_out = reserve_out * pow ( static_cast<double>(amount_in) / (reserve_in + amount_in), weight_ratio ) ;

        return amount_out  * pow ( 1 - static_cast<double> (fee) / 1000000, 2);
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
     * - `{uint64_t} [fee]` - trading fee (pips 1/100 of 1%)
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
     * const uint64_t fee = 2000;
     *
     * // Calculation
     * const uint64_t amount_in = bancor::get_amount_in( amount_out, reserve_in, reserve_weight_in, reserve_out, reserve_weight_out, fee );
     * // => 10000
     * ```
     */
    static uint64_t get_amount_in( const uint64_t amount_out, const uint64_t reserve_in, const uint64_t reserve_weight_in, const uint64_t reserve_out, const uint64_t reserve_weight_out, const uint64_t fee )
    {
        // checks
        check(false, "sx.bancor: TODO");
        eosio::check(amount_out > 0, "sx.bancor: INSUFFICIENT_OUTPUT_AMOUNT");
        eosio::check(reserve_in > 0 && reserve_out > 0, "sx.bancor: INSUFFICIENT_LIQUIDITY");

        return 0;
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
        const uint64_t amount_b = safemath::mul(amount_a, reserve_b * 1000000 / reserve_weight_b) / (reserve_a * 1000000 / reserve_weight_a);
        return amount_b;
    }

    /**
     * ## STATIC `is_legacy`
     *
     * Given pair_id returns whether it's a legacy implementation
     *
     * ### params
     *
     * - `{string} pair_id` - pair_id, i.e. "bancorc11154", "bancorcnvrtr:USDBNT"
     *
     * ### example
     *
     * ```c++
     * bool legacy = bancor::is_legacy( "bancorcnvrtr:USDBNT" );
     * // => false
     * ```
     */
    static uint64_t is_legacy( const string& pair_id )
    {
        return pair_id.find(_bancorcnvrtr_act) != 0;
    }


namespace legacy {

    /**
     * ## STATIC `get_fee`
     *
     * Get total fee
     *
     * ### params
     *
     * - `{pair_id} ` - name for conversion contract, i.e. bnt2eoscnvrt
     *
     * ### returns
     *
     * - `{uint64_t}` - total fee (trade + protocol)
     *
     * ### example
     *
     * ```c++
     * const uint64_t fee = bancor::legacy::get_fee("bancorc11154");
     * // => 2000
     * ```
     */
    static uint64_t get_fee( const string& pair_id )
    {
        bancor::settings _settings( name{pair_id}, name{pair_id}.value );
        return _settings.get().fee;
    }

    /**
     * ## STATIC `get_reserves`
     *
     * Get reserves for a contract
     *
     * ### params
     *
     * - `{string} pair_id` - name for conversion contract, i.e. bnt2eoscnvrt
     *
     * ### returns
     *
     * - `{vector<pair<asset, uint64_t>>}` - vector of reserve assets as {reserve, weight} pair
     *
     * ### example
     *
     * ```c++
     *
     * const auto [reserve0, reserve1] = bancor::legacy::get_reserves( "bnt2eoscnvrt" );
     * // reserve0 => {"4638.535 IQ", 50000}
     * // reserve1 => {"13614.8381 BNT", 50000}
     * ```
     */
    static vector<pair<asset, uint64_t>> get_reserves( const string& pair_id )
    {
        check(is_account(name{pair_id}), "sx.bancor: Bad account");

        vector<pair<asset, uint64_t>> reserves;
        bancor::reserves _reserves( name{pair_id}, name{pair_id}.value );

        for(auto& row : _reserves){
            auto balance = eosio::token::get_balance(row.contract, name{pair_id}, row.currency.symbol.code());
            reserves.push_back({ balance, row.ratio });
        }

        return reserves;
    }
}

namespace multi {

    /**
     * ## STATIC `get_fee`
     *
     * Get total fee
     *
     * ### params
     *
     * - `{pair_id} ` - pair id = bancorcnvrtr:XXXXXX, where XXXXXX - pool name, i.e. "bancorcnvrtr:USDBNT"
     *
     * ### returns
     *
     * - `{uint64_t}` - total fee (trade + protocol)
     *
     * ### example
     *
     * ```c++
     * const uint64_t fee = bancor::multi::get_fee("bancorcnvrtr:USDBNT");
     * // => 2000
     * ```
     */
    static uint64_t get_fee( const string& pair_id )
    {
        auto suffix = pair_id.substr( _bancorcnvrtr_act.length() + 1 );

        bancor::converter _converter( name{_bancorcnvrtr_act}, name{_bancorcnvrtr_act}.value );
        return _converter.get(symbol_code(suffix).raw(), "sx.bancor: Invalid pair_id").fee;
    }

    /**
     * ## STATIC `get_reserves`
     *
     * Get reserves for a contract
     *
     * ### params
     *
     * - `{string} pair_id` - pair id = bancorcnvrtr:XXXXXX, where XXXXXX - pool name, i.e. bancorcnvrtr:USDBNT
     *
     * ### returns
     *
     * - `{vector<pair<asset, uint64_t>>}` - vector of reserve assets as {reserve, weight} pair
     *
     * ### example
     *
     * ```c++
     *
     * const auto reserves = bancor::multi::get_reserves( "bancorcnvrtr:USDBNT" );
     * // reserves => [{"4638.535 IQ", 50000}, {"13614.8381 BNT", 50000}]
     * ```
     */
    static vector<pair<asset, uint64_t>> get_reserves( const string& pair_id )
    {
        check(pair_id.find(_bancorcnvrtr_act)==0, "sx.bancor: Invalid pair_id: " + pair_id);

        vector<pair<asset, uint64_t>> reserves;
        auto suffix = pair_id.substr( _bancorcnvrtr_act.length()+1);
        bancor::converter _converter( name{_bancorcnvrtr_act}, name{_bancorcnvrtr_act}.value );

        auto row = _converter.get(symbol_code{suffix}.raw(), "sx.bancor: Invalid pair_id");
        for( auto& [symcode, weight] : row.reserve_weights ){
            check(row.reserve_balances.count(symcode), "sx.bancor: Invalid pair_id: " + pair_id);
            reserves.push_back( {row.reserve_balances.at(symcode).quantity, weight} );
        }
        return reserves;
    }
}
}

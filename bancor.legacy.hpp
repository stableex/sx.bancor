#pragma once

#include <eosio.token/eosio.token.hpp>
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/singleton.hpp>

namespace bancor {

using eosio::name;
using eosio::asset;
using eosio::check;

using std::string;

namespace legacy {
    // reference
    const name id = "bancorlegacy"_n;
    const name code = "bnt2eoscnvrt"_n;
    const string description = "Bancor Legacy Converter";

    /**
     * ## STRUCT `reserve`
     *
     * ### params
     *
     * - `{name} contract` - reserve token contract
     * - `{asset} balance` - amount in the reserve
     * - `{uint64_t} weight` - reserve weight relative to the other reserves
     */
    struct reserve {
        name        contract;
        uint64_t    weight;
        asset       balance;
    };

    /**
     * ## TABLE `settings`
     *
     * This table stores the global settings affecting all the converters in this contract
     *
     * ### params
     *
     * - `{name} smart_contract` - contract account name of the smart token governed by the converter
     * - `{asset} smart_currency` - currency of the smart token governed by the converter
     * - `{bool} smart_enabled` - true if the smart token can be converted to/from, false if not
     * - `{bool} enabled` - true if conversions are enabled, false if not
     * - `{name} network` - bancor network contract name
     * - `{bool} require_balance` - require creating new balance for the calling account should fail
     * - `{uint64_t} max_fee` - maximum conversion fee percentage, 0-30000, 4-pt precision a la eosio.asset
     * - `{uint64_t} fee` - conversion fee for this converter
     */
    struct [[eosio::table("settings")]] settings_row {
        name            smart_contract;
        asset           smart_currency;
        bool            smart_enabled;
        bool            enabled;
        name            network;
        bool            require_balance;
        uint64_t        max_fee;
        uint64_t        fee;
    };
    typedef eosio::singleton< "settings"_n, settings_row > settings;

    /**
     * ## TABLE `converter`
     *
     * This table stores the reserve balances and related information for the reserves of every converter in this contract
     *
     * ### params
     *
     * - `{name} contract` - Token contract for the currency
     * - `{asset} currency` - Symbol of the tokens in this reserve
     * - `{uint64_t} ratio` - Reserve ratio
     * - `{bool} p_enabled` - Are transactions enabled on this reserve
     */
    struct [[eosio::table("reserves")]] reserves_row {
        name        contract;
        asset       currency;
        uint64_t    ratio;
        bool        p_enabled;

        uint64_t primary_key() const { return contract.value; }
    };
    typedef eosio::multi_index< "reserves"_n, reserves_row > reserves;

    /**
     * ## STATIC `get_fee`
     *
     * Get total fee
     *
     * ### params
     *
     * - `{name} code` - converter contract account
     *
     * ### example
     *
     * ```c++
     * const uint64_t fee = bancor::legacy::get_fee( "bnt2eoscnvrt"_n );
     * // => 2000
     * ```
     */
    static uint64_t get_fee( const name code )
    {
        bancor::legacy::settings _settings( code, code.value );
        check( _settings.exists(), "bancor::legacy - settings does not exists");
        return _settings.get().fee;
    }

    /**
     * ## STATIC `get_reserve`
     *
     * Get reserves for a contract
     *
     * ### params
     *
     * - `{name} code` - converter contract account (ex: "bnt2eoscnvrt"_n)
     * - `{name} contract` - token contract for the currency (ex: "eosio.token"_n)
     *
     * ### example
     *
     * ```c++
     * const bancor::reserve reserve0 = bancor::legacy::get_reserve( "bnt2eoscnvrt"_n, "eosio.token"_n );
     * const bancor::reserve reserve1 = bancor::legacy::get_reserve( "bnt2eoscnvrt"_n, "bntbntbntbnt"_n );
     * // reserve0 => {"balance": {"contract": "eosio.token", "balance": "57988.4155 EOS"}, "weight": 500000}
     * // reserve1 => {"balance": {"contract": "bntbntbntbnt", "balance": "216452.6259891919 BNT"}, "weight": 500000}
     * ```
     */
    static bancor::legacy::reserve get_reserve( const name code, const name contract )
    {
        bancor::legacy::reserves _reserves( code, code.value );
        auto row = _reserves.get( contract.value, "bancor::legacy - reserve contract does not exist");
        const asset balance = eosio::token::get_balance( contract, code, row.currency.symbol.code());
        return bancor::legacy::reserve{ contract, row.ratio, balance };
    }
};
}

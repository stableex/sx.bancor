#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/singleton.hpp>

namespace bancor {

using eosio::name;
using eosio::symbol_code;
using eosio::asset;
using eosio::extended_asset;
using eosio::check;
using eosio::symbol;

using std::map;
using std::string;

namespace multi {
    // reference
    const name id = "bancormulti"_n;
    const name code = "bancorcnvrtr"_n;
    const string description = "Bancor MultiConverter";

    /**
     * ## STRUCT `reserve`
     *
     * ### params
     *
     * - `{name} contract` - reserve token contract
     * - `{asset} balance` - amount in the reserve
     * - `{uint64_t} weight` - reserve weight relative to the other reserves
     *
     * ### example
     *
     * ```json
     * {
     *     "contract": "eosio.token",
     *     "balance": "58647.1775 EOS",
     *     "weight": 500000
     * }
     * ```
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
     * - `{uint64_t} max_fee` - maximum conversion fee for converters in this contract
     * - `{name} multi_token` - account name of contract for relay tokens
     * - `{name} network` - account name of the bancor network contract
     * - `{name} staking` - account name of contract for voting and staking
     *
     * ### example
     *
     * ```json
     * {
     *     "max_fee": 30000,
     *     "multi_token": "smarttokens1",
     *     "network": "thisisbancor",
     *     "staking": ""
     * }
     * ```
     */
    struct [[eosio::table("settings")]] settings_row {
        uint64_t    max_fee;
        name        multi_token;
        name        network;
        name        staking;
    };
    typedef eosio::singleton< "settings"_n, settings_row > settings;

    /**
     * ## TABLE `converter`
     *
     * This table stores the reserve balances and related information for the reserves of every converter in this contract
     *
     * ### params
     *
     * - `{symbol} currency` - symbol of the smart token -- representing a share in the reserves of this converter
     * - `{name} owner` - creator of the converter
     * - `{uint64_t} fee` - conversion fee for this converter, applied on every hop
     * - `{map<symbol_code, uint64_t>} reserve_weights` - reserve weights relative to the other reserves
     * - `{map<symbol_code, extended_asset>} reserve_balances` - balances in each reserve
     * - `{map<name, bool>} protocol_features` - [optional] protocol features for converter
     * - `{map<name, string>} metadata_json` - [optional] additional metadata for converter
     *
     * ### example
     *
     * ```json
     * {
     *     "currency": "4,EOSBNT",
     *     "owner": "guztoojqgege",
     *     "fee": 2000,
     *     "reserve_weights": [
     *         { "key": "EOS", "value": 500000 },
     *         { "key": "BNT", "value": 500000 }
     *     ],
     *     "reserve_balances": [
     *         { "key": "EOS", "value": { "quantity": "58647.1775 EOS", "contract": "eosio.token" } },
     *         { "key": "BNT", "value": { "quantity": "214045.8934706095 BNT", "contract": "bntbntbntbnt" } }
     *     ],
     *     "protocol_features": [],
     *     "metadata_json": []
     * }
     * ```
     */
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

    /**
     * ## STATIC `get_fee`
     *
     * Get total fee
     *
     * ### params
     *
     * - `{symbol_code} currency` - currency symbol code (ex: EOSBNT)
     * - `{name} [code="bancorcnvrtr"_n]` - converter contract account
     *
     * ### example
     *
     * ```c++
     * const uint64_t fee = bancor::multi::get_fee( {"EOSBNT"} );
     * // => 2000
     * ```
     */
    static uint64_t get_fee( const symbol_code currency, const name code = bancor::multi::code )
    {
        bancor::multi::converter _converter( code, code.value );
        return _converter.get( currency.raw(), "sx.bancor::multi: reserve pair symbol code not found").fee;
    }

    /**
     * ## STATIC `get_reserve`
     *
     * Get reserve from a currency
     *
     * ### params
     *
     * - `{symbol_code} currency` - currency symbol code (ex: "EOSBNT")
     * - `{symbol_code} reserve` - reserve symbol code (ex: "EOS")
     * - `{name} [code="bancorcnvrtr"_n]` - converter contract account
     *
     * ### example
     *
     * ```c++
     * const bancor::reserve reserve0 = bancor::multi::get_reserve( {"EOSBNT"}, {"EOS"} );
     * const bancor::reserve reserve1 = bancor::multi::get_reserve( {"EOSBNT"}, {"BNT"} );
     * // reserve0 => {"contract": "eosio.token", "weight": 500000, "balance": "57988.4155 EOS"}
     * // reserve1 => {"contract": "bntbntbntbnt", "weight": 500000, "balance": "216452.6259891919 BNT"}
     * ```
     */
    static bancor::multi::reserve get_reserve( const symbol_code currency, const symbol_code reserve, const name code = bancor::multi::code )
    {
        bancor::multi::converter _converter( code, code.value );
        auto row = _converter.get( currency.raw(), "sx.bancor::multi: currency symbol does not exist");
        check(row.reserve_balances.count(reserve), "sx.bancor::multi: reserve balance symbol does not exist");
        check(row.reserve_weights.count(reserve), "sx.bancor::multi: reserve weights symbol does not exist");

        const extended_asset balance = row.reserve_balances.at(reserve);
        return bancor::multi::reserve{ balance.contract, row.reserve_weights.at(reserve), balance.quantity };
    }

    /**
     * ## STATIC `get_reserves`
     *
     * Get all reserves from a currency
     *
     * ### params
     *
     * - `{symbol_code} currency` - currency symbol code (ex: "EOSBNT")
     * - `{name} [code="bancorcnvrtr"_n]` - converter contract account
     *
     * ### example
     *
     * ```c++
     * const auto [ reserve0, reserve1 ]  = bancor::multi::get_reserves( {"EOSBNT"} );
     * // reserve0 => {"contract": "eosio.token", "weight": 500000, "balance": "57988.4155 EOS"}
     * // reserve1 => {"contract": "bntbntbntbnt", "weight": 500000, "balance": "216452.6259891919 BNT"}
     * ```
     */
    static std::vector<bancor::multi::reserve> get_reserves( const symbol_code currency, const name code = bancor::multi::code )
    {
        bancor::multi::converter _converter( code, code.value );
        std::vector<bancor::multi::reserve> reserves;

        auto row = _converter.get( currency.raw(), "sx.bancor::multi: currency symbol does not exist");
        for ( const auto itr : row.reserve_balances ) {
            reserves.push_back( bancor::multi::get_reserve( currency, itr.first, code ) );
        }
        return reserves;
    }
};
}
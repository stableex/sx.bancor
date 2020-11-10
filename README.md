# **`Bancor`**

[![Build Status](https://travis-ci.org/stableex/sx.bancor.svg?branch=main)](https://travis-ci.org/stableex/sx.bancor)

> Peripheral EOSIO smart contracts for interacting with Bancor Protocol

## Quickstart

```c++
#include "bancor.hpp"

// Inputs
const uint64_t amount_in = 1000000;
const uint64_t reserve_in = 578125412;
const uint64_t reserve_out = 2170087186740517;
const uint64_t reserve_weight_in = 500000;
const uint64_t reserve_weight_out = 500000;
const uint64_t fee = 2000;

// Calculation
const uint64_t out = bancor::get_amount_out( amount_in,
                                             reserve_in,
                                             reserve_weight_in,
                                             reserve_out,
                                             reserve_weight_out,
                                             fee );
// => 3732206312408
```

#### Bancor Multi Converter

```c++
#include "bancor.multi.hpp"

const auto [ reserve0, reserve1 ] = bancor::multi::get_reserves( {"EOSBNT"} );
// reserve0 => {"contract": "eosio.token", "weight": 500000, "balance": "57988.4155 EOS"}
// reserve1 => {"contract": "bntbntbntbnt", "weight": 500000, "balance": "216452.6259891919 BNT"}
```

#### Bancor Legacy Converter

```c++
#include "bancor.legacy.hpp"

const auto [ reserve0, reserve1 ]  = bancor::legacy::get_reserves( "bnt2eoscnvrt"_n );
// reserve0 => {"contract": "eosio.token", "weight": 500000, "balance": "55988.4608 EOS"}
// reserve1 => {"contract": "bntbntbntbnt", "weight": 500000, "balance": "204278.1014136003 BNT"}
```

## Table of Content

- [STATIC `get_amount_out`](#static-get_amount_out)
- [STATIC `get_amount_in`](#static-get_amount_in)
- [STATIC `quote`](#static-quote)
- [STATIC `get_fee`](#static-get_fee)
- [STATIC `get_reserve`](#static-get_reserve)
- [STATIC `get_reserves`](#static-get_reserves)
- [TABLE `converter`](#static-converter)
- [TABLE `settings`](#static-settings)
- [STRUCT `reserve`](#static-reserve)

## STATIC `get_amount_out`

Given an input amount of an asset and pair reserves, returns the output amount of the other asset

### params

- `{uint64_t} amount_in` - amount input
- `{uint64_t} reserve_in` - reserve input
- `{uint64_t} reserve_weight_in` - reserve input weight
- `{uint64_t} reserve_out` - reserve output
- `{uint64_t} reserve_weight_out` - reserve output weight
- `{uint64_t} fee` - trading fee (pips 1/10000 of 1%)

### example

```c++
// Inputs
const uint64_t amount_in = 10000;
const uint64_t reserve_in = 45851931234;
const uint64_t reserve_weight_in = 50000;
const uint64_t reserve_out = 125682033533;
const uint64_t reserve_weight_out = 50000;
const uint64_t fee = 2000;

// Calculation
const uint64_t amount_out = bancor::get_amount_out( amount_in, reserve_in, reserve_weight_in, reserve_out, reserve_weight_out );
// => 27300
```

## STATIC `get_amount_in`

Given an output amount of an asset and pair reserves, returns a required input amount of the other asset.

### params

- `{uint64_t} amount_out` - amount input
- `{uint64_t} reserve_in` - reserve input
- `{uint64_t} reserve_weight_in` - reserve input weight
- `{uint64_t} reserve_out` - reserve output
- `{uint64_t} reserve_weight_out` - reserve output weight
- `{uint64_t} fee` - trading fee (pips 1/10000 of 1%)

### example

```c++
// Inputs
const uint64_t amount_out = 27328;
const uint64_t reserve_in = 45851931234;
const uint64_t reserve_weight_in = 50000;
const uint64_t reserve_out = 125682033533;
const uint64_t reserve_weight_out = 50000;
const uint64_t fee = 2000;

// Calculation
const uint64_t amount_in = bancor::get_amount_in( amount_out, reserve_in, reserve_weight_in, reserve_out, reserve_weight_out, fee );
// => 10000
```

## STATIC `quote`

Given some amount of an asset and pair reserves, returns an equivalent amount of the other asset

### params

- `{uint64_t} amount_a` - amount A
- `{uint64_t} reserve_a` - reserve A
- `{uint64_t} reserve_weight_a` - reserve A weight
- `{uint64_t} reserve_b` - reserve B
- `{uint64_t} reserve_weight_b` - reserve B weight

### example

```c++
// Inputs
const uint64_t amount_a = 10000;
const uint64_t reserve_a = 45851931234;
const uint64_t reserve_b = 125682033533;
const uint64_t reserve_weight_a = 50000;
const uint64_t reserve_weight_b = 50000;

// Calculation
const uint64_t amount_b = bancor::quote( amount_a, reserve_a, reserve_weight_a, reserve_b, reserve_weight_b );
// => 27410
```

## STATIC `get_fee`

Get total fee

### params

- `{symbol_code} currency` - currency symbol code (ex: EOSBNT)
- `{name} [code="bancorcnvrtr"_n]` - converter contract account

### example

```c++
const uint64_t fee = bancor::multi::get_fee( {"EOSBNT"} );
// => 2000
```

## STATIC `get_reserve`

Get reserve from a currency

### params

- `{symbol_code} currency` - currency symbol code (ex: "EOSBNT")
- `{symbol_code} reserve` - reserve symbol code (ex: "EOS")
- `{name} [code="bancorcnvrtr"_n]` - converter contract account

### example

```c++
const bancor::reserve reserve0 = bancor::multi::get_reserve( {"EOSBNT"}, {"EOS"} );
const bancor::reserve reserve1 = bancor::multi::get_reserve( {"EOSBNT"}, {"BNT"} );
// reserve0 => {"contract": "eosio.token", "weight": 500000, "balance": "57988.4155 EOS"}
// reserve1 => {"contract": "bntbntbntbnt", "weight": 500000, "balance": "216452.6259891919 BNT"}
```

## STATIC `get_reserves`

Get all reserves from a currency

### params

- `{symbol_code} currency` - currency symbol code (ex: "EOSBNT")
- `{name} [code="bancorcnvrtr"_n]` - converter contract account

### example

```c++
const auto [ reserve0, reserve1 ]  = bancor::multi::get_reserves( {"EOSBNT"} );
// reserve0 => {"contract": "eosio.token", "weight": 500000, "balance": "57988.4155 EOS"}
// reserve1 => {"contract": "bntbntbntbnt", "weight": 500000, "balance": "216452.6259891919 BNT"}
```

## TABLE `converter`

This table stores the reserve balances and related information for the reserves of every converter in this contract

### params

- `{symbol} currency` - symbol of the smart token -- representing a share in the reserves of this converter
- `{name} owner` - creator of the converter
- `{uint64_t} fee` - conversion fee for this converter, applied on every hop
- `{map<symbol_code, uint64_t>} reserve_weights` - reserve weights relative to the other reserves
- `{map<symbol_code, extended_asset>} reserve_balances` - balances in each reserve
- `{map<name, bool>} protocol_features` - [optional] protocol features for converter
- `{map<name, string>} metadata_json` - [optional] additional metadata for converter

### example

```json
{
    "currency": "4,EOSBNT",
    "owner": "guztoojqgege",
    "fee": 2000,
    "reserve_weights": [
        { "key": "EOS", "value": 500000 },
        { "key": "BNT", "value": 500000 }
    ],
    "reserve_balances": [
        { "key": "EOS", "value": { "quantity": "58647.1775 EOS", "contract": "eosio.token" } },
        { "key": "BNT", "value": { "quantity": "214045.8934706095 BNT", "contract": "bntbntbntbnt" } }
    ],
    "protocol_features": [],
    "metadata_json": []
}
```

## TABLE `settings`

This table stores the global settings affecting all the converters in this contract

### params

- `{uint64_t} max_fee` - maximum conversion fee for converters in this contract
- `{name} multi_token` - account name of contract for relay tokens
- `{name} network` - account name of the bancor network contract
- `{name} [staking=""]` - account name of contract for voting and staking

### example

```json
{
    "max_fee": 30000,
    "multi_token": "smarttokens1",
    "network": "thisisbancor",
    "staking": ""
}
```

## STRUCT `reserve`

### params

- `{name} contract` - reserve token contract
- `{asset} balance` - amount in the reserve
- `{uint64_t} weight` - reserve weight relative to the other reserves

### example

```json
{
    "contract": "eosio.token",
    "balance": "58647.1775 EOS",
    "weight": 500000
}
```
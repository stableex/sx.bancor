# **`Bancor`**

[![Build Status](https://travis-ci.org/stableex/sx.bancor.svg?branch=master)](https://travis-ci.org/stableex/sx.bancor)

> Peripheral EOSIO smart contracts for interacting with Bancor Protocol

## Quickstart

```c++
#include "bancor.hpp"

// Inputs
const uint64_t amount_in = 10000;
const uint64_t reserve_in = 45851931234;
const uint64_t reserve_weight_in = 50000;
const uint64_t reserve_out = 125682033533;
const uint64_t reserve_weight_out = 50000;
const uint8_t fee = 30;

// Calculation
const uint64_t out = bancor::get_amount_out( amount_in,
                                             reserve_in,
                                             reserve_weight_in,
                                             reserve_out,
                                             reserve_weight_out,
                                             fee );
// => 27328
```

## Pseudocode Price Formula

```c++
function get_amount_out( amount_in,
                         reserve_in,
                         reserve_weight_in,
                         reserve_out,
                         reserve_weight_out,
                         fee )
{
    weight_ratio = (reserve_weight_in / reserve_weight_out);
    amount_in_with_fee = amount_in * (10000 - fee);
    numerator = (reserve_in * 10000) / ((reserve_in * 10000) + amount_in_with_fee)
    denominator = 1 - (numerator ** weight_ratio);
    amount_out = reserve_out * denominator;
    return amount_out;
}
```

## Table of Content

- [STATIC `get_amount_out`](#static-get_amount_out)
- [STATIC `get_amount_in`](#static-get_amount_in)
- [STATIC `quote`](#static-quote)

## STATIC `get_amount_out`

Given an input amount of an asset and pair reserves, returns the maximum output amount of the other asset

### params

- `{uint64_t} amount_in` - amount input
- `{uint64_t} reserve_in` - reserve input
- `{uint64_t} reserve_weight_in` - reserve input weight
- `{uint64_t} reserve_out` - reserve output
- `{uint64_t} reserve_weight_out` - reserve output weight
- `{uint8_t} [fee=30]` - (optional) trading fee (pips 1/100 of 1%)

### example

```c++
// Inputs
const uint64_t amount_in = 10000;
const uint64_t reserve_in = 45851931234;
const uint64_t reserve_weight_in = 50000;
const uint64_t reserve_out = 125682033533;
const uint64_t reserve_weight_out = 50000;
const uint8_t fee = 30;

// Calculation
const uint64_t amount_out = bancor::get_amount_out( amount_in, reserve_in, reserve_weight_in, reserve_out, reserve_weight_out );
// => 27328
```

## STATIC `get_amount_in`

Given an output amount of an asset and pair reserves, returns a required input amount of the other asset.

### params

- `{uint64_t} amount_out` - amount input
- `{uint64_t} reserve_in` - reserve input
- `{uint64_t} reserve_weight_in` - reserve input weight
- `{uint64_t} reserve_out` - reserve output
- `{uint64_t} reserve_weight_out` - reserve output weight
- `{uint8_t} [fee=30]` - (optional) trading fee (pips 1/100 of 1%)

### example

```c++
// Inputs
const uint64_t amount_out = 27328;
const uint64_t reserve_in = 45851931234;
const uint64_t reserve_weight_in = 50000;
const uint64_t reserve_out = 125682033533;
const uint64_t reserve_weight_out = 50000;
const uint8_t fee = 30;

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
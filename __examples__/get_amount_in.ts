
function get_amount_in_uniswap( amount_out: number, reserve_in: number, reserve_out: number, fee: number ) {
    const numerator = reserve_in * amount_out * 10000;
    const denominator = (reserve_out - amount_out) * (10000 - fee);
    const amount_in = 1 + (numerator / denominator);
    return amount_in;
}

function get_amount_in_bancor( amount_out: number, reserve_in: number, reserve_weight_in: number, reserve_out: number, reserve_weight_out: number, fee: number ) {
    const weight_ratio = (reserve_weight_in / reserve_weight_out);
    const numerator = reserve_in * amount_out * 10000;
    const denominator = (reserve_out - amount_out) * (10000 - fee) * weight_ratio;
    const amount_in = 1 + (numerator / denominator);
    return amount_in;
}

// input
const trade_fee = 30;
const protocol_fee = 0;
const amount_out = 39876;
const reserve_in = 100000000;
const reserve_weight_in = 500000;
const reserve_out = 400000000;
const reserve_weight_out = 500000;

console.log("uniswap:", get_amount_in_uniswap( amount_out, reserve_in, reserve_out, trade_fee ));
console.log("bancor:", get_amount_in_bancor( amount_out, reserve_in, reserve_weight_in, reserve_out, reserve_weight_out, trade_fee ));
// => 10000

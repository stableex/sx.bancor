
function get_amount_out_uniswap( amount_in: number, reserve_in: number, reserve_out: number, fee: number ) {
    const amount_in_with_fee = amount_in * (10000 - fee);
    const numerator = amount_in_with_fee * reserve_out;
    const denominator = (reserve_in * 10000) + amount_in_with_fee;
    const amount_out = numerator / denominator;

    return amount_out;
}

function get_amount_out_bancor( amount_in: number, reserve_in: number, reserve_weight_in: number, reserve_out: number, reserve_weight_out: number, fee: number ) {
    const weight_ratio = (reserve_weight_in / reserve_weight_out);
    const amount_in_with_fee = amount_in * (10000 - fee);
    const numerator = (reserve_in * 10000) / ((reserve_in * 10000) + amount_in_with_fee)
    const denominator = 1 - (numerator ** weight_ratio);
    const amount_out = reserve_out * denominator;
    return amount_out;
}

// input
const trade_fee = 30;
const protocol_fee = 0;
const amount_in = 10000;
const reserve_in = 100000000;
const reserve_weight_in = 400000;
const reserve_out = 400000000;
const reserve_weight_out = 600000;

console.log("uniswap:", get_amount_out_uniswap( amount_in, reserve_in, reserve_out, trade_fee ));
console.log("bancor:", get_amount_out_bancor( amount_in, reserve_in, reserve_weight_in, reserve_out, reserve_weight_out, trade_fee ));
// => 39876


function quote_uniswap( amount_a: number, reserve_a: number, reserve_b: number ) {
    return amount_a * reserve_b / reserve_a;
}

function quote_bancor( amount_a: number, reserve_a: number, reserve_weight_a: number, reserve_b: number, reserve_weight_b: number) {
    return amount_a * (reserve_b * 10000 / reserve_weight_b) / (reserve_a * 10000 / reserve_weight_a);
}

// input
const fee = 30;
const amount_in = 10000;
const reserve_in = 100000000;
const reserve_weight_in = 500000;
const reserve_out = 400000000;
const reserve_weight_out = 500000;

console.log(quote_uniswap( amount_in, reserve_in, reserve_out ));
console.log(quote_bancor( amount_in, reserve_in, reserve_weight_in, reserve_out, reserve_weight_out ));
// => 40000
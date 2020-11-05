#!/bin/bash

# compile
g++ -std=c++11 -o bancor.t.out bancor.t.cpp -I __tests__

# test
./bancor.t.out --success


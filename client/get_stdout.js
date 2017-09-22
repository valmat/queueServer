#!/usr/local/bin/node --max-old-space-size=512


"use strict"

const options      = require('./configs.js');
const RocksHandler = require('./node_rocks/handler.js');
const handler      = new RocksHandler(options);



if (!process.argv[2]) {
    console.log("Usage:\n" + process.argv[1] + ' <index>');
    process.exit(0);
}
const index = process.argv[2];

const key   = options.prefixes.famaly + options.prefixes.stdout + index;

handler.get(key, function(value) {
    console.log(value);
});

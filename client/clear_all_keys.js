#!/usr/local/bin/node --max-old-space-size=512

// Очищает БД, удаляя все ключи

"use strict"

const options      = require('./configs.js');
const RocksHandler = require('./node_rocks/handler.js');
const handler      = new RocksHandler(options);

const pref = options.prefixes.famaly ? options.prefixes.famaly : null;

let keys = [];
handler.getall(pref, function(resp) {
    for(let pair of resp) {
        keys.push(pair.key)
    }
    if(keys.length) {
        handler.mdel(keys, function(ok) {
            console.log({clear_all_keys: ok});
        })
    } else {
        console.log('DB has not keys');
    }
});

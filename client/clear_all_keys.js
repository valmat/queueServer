#!/usr/bin/env node
//
// Очищает БД, удаляя все ключи
//
"use strict"

const load_cfg  = require('./load_cfg.js');
const {options} = load_cfg();

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
#!/usr/bin/env node
//
// Генерирует команду для выполнения bash
//
"use strict"

const load_cfg     = require('./load_cfg.js');
const {options}    = load_cfg();

const RocksHandler = require('./node_rocks/handler.js');
const handler      = new RocksHandler(options);


const from_pref = options.prefixes.famaly + options.prefixes.created;
const to_pref   = options.prefixes.famaly + options.prefixes.started;

handler.moveKey(from_pref, to_pref, function(isOk, pair) {
    if(!isOk) {
        options.run.done()
    } else if(('' == pair.key) || (null == pair.value)) {
        options.run.done()
    } else {
        console.log(options.run.next_prefix + `${pair.key}:` + options.run.next(pair))
    }
});
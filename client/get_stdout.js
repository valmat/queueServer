#!/usr/bin/env node --max-old-space-size=512

"use strict"

const load_cfg         = require('./load_cfg.js');
const {options, index} = load_cfg('index');

const RocksHandler     = require('./node_rocks/handler.js');
const handler          = new RocksHandler(options);

const key   = options.prefixes.famaly + options.prefixes.stdout + index;

handler.get(key, value => {
    console.log(value);
});

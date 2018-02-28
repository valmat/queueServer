#!/usr/bin/env node
//
// Заполняет ключи по заданному файлу и правилу
//
"use strict"

const load_cfg     = require('./load_cfg.js');
const {options}    = load_cfg();

const RocksHandler = require('./node_rocks/handler.js');

const lineReader   = require('line-reader');
const values       = new Map();

let line_nom = 0;
lineReader.eachLine(options.fill.file, function(line, last) {
    let {index, value} = options.fill.process(line_nom, line);
    let key_pref = options.prefixes.famaly + options.prefixes.created;
    values.set(`${key_pref}${index}`, value);

    line_nom++;
    if (last) {
        const handler = new RocksHandler(options);
        handler.setMap(values, function(ok) {
            console.log({values_set:ok, values_size: values.size});
        });
        return false;
    }
});
#!/usr/bin/env node
//
// Возвращает поле из конфига
//
"use strict"

const load_cfg         = require('./load_cfg.js');
const {options, field} = load_cfg("field");

let chain = field.split('.');

let value = options;
for(let e of chain) {
    value = value[e];
}

process.stdout.write(value);
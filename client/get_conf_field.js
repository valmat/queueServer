#!/usr/local/bin/node --max-old-space-size=512

// Возвращает поле из конфига

"use strict"

const options = require('./configs.js');

if (!process.argv[2]) {
    console.log("Usage:\n" + process.argv[1] + ' <field>');
    process.exit(0);
}
const field = process.argv[2];

let chain = field.split('.');

let value = options;
for(let e of chain) {
    value = value[e];
}

console.log(value);

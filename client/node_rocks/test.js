#!/usr/local/bin/node --max-old-space-size=512
//#!/usr/bin/nodejs

//http://127.0.0.1:5591/tail

"use strict"

const options      = require('../configs.js');
const RocksHandler = require('./handler.js');
const handler      = new RocksHandler(options);




/*
const from = 'key:';
const to = 'key:';

handler.httpPost('move-fpref', function(response) {
    console.log(response.isOk());
    
    //console.log({key: response.read(), value: response.getValue()});
    console.log(response.getPair());

    console.log(response);
}, `${from}\n${to}`)

handler.moveKey(from, to, function(isOk, pair) {
    console.log(isOk);
    console.log(pair);
})

handler.get('key:4', function(val) {
    console.log({val});
})


handler.set('new_key', 'new_val\ndfdsfd', function(ok) {
    console.log({ok});
})
handler.get('new_key', function(new_val) {
    console.log({new_val});
})


handler.del('dgdgdf', function(ok) {
    console.log({del:ok});
})

handler.mdel(['kkkk', 'qwqwqw'], function(ok) {
    console.log({mdel:ok});
})

handler.setObj({
    sss1:5,
    sss2:'dsfsdf',
    pi:3.14
}, function(ok) {
    console.log({ok});
});

handler.firstPref('key:', function(pair) {
    console.log({pair});
})


let a = new Map();
a.set('k_m_1', 'v_m_1');
a.set('k_m_2', 'v_m_2');
a.set('k_m_3', 'v_m_3');
a.set('k_m_4', 'v_m_4');
handler.setMap(a, function(ok) {
    console.log({ok});
});

let b = [
    ['k_a_1', 'v_a_1'],
    ['k_a_2', 'v_a_2'],
    ['k_a_3', 'v_a_3'],
    ['k_a_4', 'v_a_4'],
];
handler.setMap(b, function(ok) {
    console.log({ok});
});

handler.incr('incr1', null, function(ok) {
    console.log({ok});
});
handler.incr('incr2', 5, function(ok) {
    console.log({ok});
});


handler.incrGet('incr3', null, function(val) {
    console.log({val});
});
handler.incrGet('incr4', 5, function(val) {
    console.log({val});
});



handler.mget(['k_a_1','k_a_2','k_a_3'], function(resp) {
    for(let pair of resp) {
        console.log({pair});
    }
});


handler.getall('key:', function(resp) {
    for(let pair of resp) {
        console.log({pair});
    }
});

handler.getall(null, function(resp) {
    for(let pair of resp) {
        console.log({pair});
    }
});



*/

handler.incrGet('incr4', 5, function(val) {
    console.log({val});
});



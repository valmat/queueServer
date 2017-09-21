// Получает http щтвет и возращает данные

"use strict"


const rocks_response = require('./response.js');


const net = require('net');

const empty_fun = function() {};


function RocksHandler(options) {
    this._server = options.server;
}


RocksHandler.prototype.httpPost = function(path, on_response, data = null) {
    const client = new net.Socket();

    client.connect(this._server.port, this._server.host, function() {
        client.write("POST /" + path + " HTTP/1.1\r\n");
        //console.log({data})
        if(null != data) {
            client.write("Content-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n");
            client.write("Content-Length: " + data.length + "\r\n");
            client.write("Connection: Close\r\n");
            client.write("\r\n");
            client.write(data);
        } else {
            client.write("Content-Type: charset=UTF-8\r\n");
            client.write("Connection: Close\r\n");
            client.write("\r\n");
        }
    });
    client.on('data', function(response) {
        _on_response(client, response, on_response);
    });
    client.on('close', empty_fun);
}

RocksHandler.prototype.httpGet = function(path, on_response, data = null) {
    const client = new net.Socket();
    client.connect(this._server.port, this._server.host, function() {
        client.write(data ? `GET /${path}?${data} HTTP/1.1\r\n` : `GET /${path} HTTP/1.1\r\n`);
        client.write("Content-Type:charset=UTF-8\r\n");
        client.write("Connection: Close\r\n\r\n");
    });
    client.on('data', function(response) {
        _on_response(client, response, on_response);
    });
    client.on('close', function() {});
}

// get value by key
RocksHandler.prototype.get = function(key, on_response) {
    this.httpGet('get', function(resp) {
        on_response(resp.getValue());
    }, key);
}


RocksHandler.prototype.moveKey = function(from, to, on_response = empty_fun) {
    this.httpPost('move-fpref', function(resp) {
        on_response(resp.isOk(), resp.getPair());
    }, `${from}\n${to}`);
}


// set value for key
RocksHandler.prototype.set = function(key, val, on_response = empty_fun) {
    this.httpPost('set', function(resp) {
        on_response(resp.isOk());
    }, [key, new String(val).length, val].join('\n'));
}

// multi set values for keys
RocksHandler.prototype.setObj = function(obj, on_response = empty_fun) {
    this.httpPost('mset', function(resp) {
        on_response(resp.isOk());
    }, _data2str(obj));
}
// multi set values for keys
// Принимает объект типа Map или итерируемый объект (массив) пар [key, value]
RocksHandler.prototype.setMap = function(map, on_response = empty_fun) {
    let data = '';
    for(let pair of map) {
        //console.log({pair});
        let val = new String(pair[1]);
        data += `${pair[0]}\n${val.length}\n${val}\n`;
    }

    this.httpPost('mset', function(resp) {
        on_response(resp.isOk());
    }, data);
}

// remove key from db
RocksHandler.prototype.del = function(key, on_response = empty_fun) {
    this.httpPost('del', function(resp) {
        on_response(resp.isOk());
    }, key);
}

RocksHandler.prototype.mdel = function(keys, on_response = empty_fun) {
    this.httpPost('mdel', function(resp) {
        on_response(resp.isOk());
    }, keys.join('\n'));
}

// Получить первый ключ подходящий к префиксу
RocksHandler.prototype.firstPref = function(prefix, on_response) {
    this.httpGet('first-pref', function(resp) {
        on_response(resp.getPair());
    }, prefix);
}

// incriment value by key
RocksHandler.prototype.incr = function(key, value = null, on_response = empty_fun) {
    let cb = function(resp) {
        on_response(resp.isOk());
    };
    (null === value) ?
        this.httpPost('incr', cb, key ) :
        this.httpPost('incr', cb, `${key}\n${value}` );

}

// incriment key by value and return vlaue
RocksHandler.prototype.incrGet = function(key, value = null, on_response = empty_fun) {
    let cb = function(resp) {
        on_response(parseInt(resp.getValue()));
    };
    (null === value) ?
        this.httpPost('get-incr', cb, key ) :
        this.httpPost('get-incr', cb, `${key}\n${value}` );
}


// multi get key-value pairs by keys
RocksHandler.prototype.mget = function(keys, on_response) {
    this.httpGet('mget', function(resp) {
        on_response(resp.mgetIterator());
        //getMultiValue();
    }, keys.join('&'));
}

// multi get all key-value pairs (by key-prefix)
// @return MgetIterator
RocksHandler.prototype.getall = function(prefix = null, on_response) {
    let cb = function(resp) {
        on_response(resp.mgetIterator());
    };
    (null === prefix) ?
        this.httpGet('tail', cb) :
        this.httpGet('prefit', cb, prefix );
}




///////////////////////////////////////////


function _on_response(client, response, on_response) {
    let [headers, ...body] = new String(response).split('\r\n\r\n');
    body = body.join('\r\n\r\n');

    client.destroy(); // kill client after server's response
    on_response(new rocks_response(body));
}

function _getBinarySize(str) {
    return Buffer.byteLength(str, 'utf8');
}

function _data2str(obj) {
    let ret = '';
    for(let key of Object.keys(obj)) {
        let val = new String(obj[key]);
        ret += `${key}\n${val.length}\n${val}\n`;
    }
    return ret;
}


module.exports = RocksHandler;
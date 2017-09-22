// Handle http response and extract data

"use strict"


const RocksResponse = require('./response.js');
const net           = require('net');

const empty_fun = function() {};

class RocksHandler {
    constructor(options) {
        this._server = options.server;
    }

    // implements common http post method
    httpPost(path, on_response, data = null) {
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

    // implements common http get method
    httpGet(path, on_response, data = null) {
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
    get(key, on_response) {
        this.httpGet('get', function(resp) {
            on_response(resp.getValue());
        }, key);
    }

    // Move value by keys prefixes.
    // Finds first key starts with prefix `from` and replace its prefix to `to`
    moveKey(from, to, on_response = empty_fun) {
        this.httpPost('move-fpref', function(resp) {
            on_response(resp.isOk(), resp.getPair());
        }, `${from}\n${to}`);
    }


    // set value for key
    set(key, val, on_response = empty_fun) {
        this.httpPost('set', function(resp) {
            on_response(resp.isOk());
        }, [key, new String(val).length, val].join('\n'));
    }

    // multiply set values for keys
    setObj(obj, on_response = empty_fun) {
        this.httpPost('mset', function(resp) {
            on_response(resp.isOk());
        }, _obj2str(obj));
    }

    // multiply set values for keys
    // Accepts an object of type Map or iterable object (array) of pairs [key, value]
    setMap(map, on_response = empty_fun) {
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
    del(key, on_response = empty_fun) {
        this.httpPost('del', function(resp) {
            on_response(resp.isOk());
        }, key);
    }

    // remove multiply keys from db
    mdel(keys, on_response = empty_fun) {
        this.httpPost('mdel', function(resp) {
            on_response(resp.isOk());
        }, keys.join('\n'));
    }

    // Finds first key starts with prefix and retrive its pair
    firstPref(prefix, on_response) {
        this.httpGet('first-pref', function(resp) {
            on_response(resp.getPair());
        }, prefix);
    }

    // incriment value by key
    incr(key, value = null, on_response = empty_fun) {
        let cb = function(resp) {
            on_response(resp.isOk());
        };
        (null === value) ?
            this.httpPost('incr', cb, key ) :
            this.httpPost('incr', cb, `${key}\n${value}` );

    }

    // incriment key by value and return vlaue
    incrGet(key, value = null, on_response = empty_fun) {
        let cb = function(resp) {
            on_response(parseInt(resp.getValue()));
        };
        (null === value) ?
            this.httpPost('get-incr', cb, key ) :
            this.httpPost('get-incr', cb, `${key}\n${value}` );
    }


    // multi get key-value pairs by keys
    mget(keys, on_response) {
        this.httpGet('mget', function(resp) {
            on_response(resp.mgetIterator());
            //getMultiValue();
        }, keys.join('&'));
    }

    // multi get all key-value pairs (by key-prefix)
    // returns MgetIterator
    getall(prefix = null, on_response) {
        let cb = function(resp) {
            on_response(resp.mgetIterator());
        };
        (null === prefix) ?
            this.httpGet('tail', cb) :
            this.httpGet('prefit', cb, prefix );
    }
}



// private:


function _on_response(client, response, on_response) {
    // Retrive body from HTTP responce
    let body = new String(response.slice(response.indexOf('\r\n\r\n') + 4));

    client.destroy();
    on_response(new RocksResponse(body));
}

function _getBinarySize(str) {
    return Buffer.byteLength(str, 'utf8');
}

function _obj2str(obj) {
    let ret = '';
    for(let key of Object.keys(obj)) {
        let val = new String(obj[key]);
        ret += `${key}\n${val.length}\n${val}\n`;
    }
    return ret;
}


module.exports = RocksHandler;

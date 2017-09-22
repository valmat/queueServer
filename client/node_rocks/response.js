"use strict"



function Pair(key = null, value = null) {
    this.key   = key;
    this.value = value;
}



function RocksResponse(body) {
    this._body = body;
}

// Read line before '\n'
RocksResponse.prototype.read = function(len = 0) {
    if(0 == len) {
        let [rez, ...body] = this._body.split('\n');
        this._body = body.join('\n');
        return rez;
    } else {
        let rez = this._body.substring(0, len);
        this._body = this._body.substring(len+1);
        return rez;
    }
}


RocksResponse.prototype.isOk = function() {
    return 'OK' === this.read();
}

RocksResponse.prototype.getValue = function() {
    if(!this._body.length) {
        return null;
    }
    let val_len = parseInt(this.read());
    if(val_len < 0 || !this._body.length) {
        return null;
    }
    return val_len ? this.read(val_len) : '';
}

RocksResponse.prototype.getPair = function() {
    let key = this.read();
    return new Pair(key, this.getValue());
}

RocksResponse.prototype.mgetIterator = function* () {
    while(this._body.length) {
        yield this.getPair();
    }
}

RocksResponse.prototype.raw = function() {
    return this._body;
}

module.exports = RocksResponse;

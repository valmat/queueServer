"use strict"


class Pair
{
    constructor(key = null, value = null) {
        this.key   = key;
        this.value = value;
    }
}

class RocksResponse
{
    constructor(body) {
        this._body = body;
    }

    // Read line before '\n'
    read(len = 0) {
        if(0 == len) {
            let [rez, ...body] = this._body.split('\n');
            this._body = body.join('\n');
            return rez;
        } else {
            let buf =  Buffer.from(this._body, 'utf-8')
            let rez    = buf.slice(0, len).toString("utf-8");
            this._body = buf.slice(len+1).toString("utf-8");
            return rez;
        }
    }

    isOk() {
        return 'OK' === this.read();
    }

    getValue() {
        if(!this.size()) {
            return null;
        }
        let val_len = parseInt(this.read());
        if(val_len < 0 || !this.size()) {
            return null;
        }
        return val_len ? this.read(val_len) : '';
    }

    getPair() {
        let key = this.read();
        return new Pair(key, this.getValue());
    }

    *mgetIterator() {
        while(this.size()) {
            yield this.getPair();
        }
    }

    raw() {
        return this._body;
    }

    size() {
        return Buffer.byteLength(this._body, 'utf8')
    }
}

module.exports = RocksResponse;
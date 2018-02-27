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
    // @Buffer body
    constructor(body) {
        this._body = body;
    }

    // Read line before '\n'
    read(len = 0) {
        // if len is not specified read to first '\n' char
        (0 == len) && (len = this._body.indexOf('\n'));

        let ret    = this._body.slice(0, len).toString("utf-8");
        this._body = this._body.slice(len+1);
        return ret;
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

    // returns buffer
    raw() {
        return this._body;
    }

    toString() {
        return this._body.toString('utf-8');
    }

    size() {
        return this._body.length;
    }
}

module.exports = RocksResponse;
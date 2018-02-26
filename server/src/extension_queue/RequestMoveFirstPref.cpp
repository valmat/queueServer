/**
 *  RequestMoveFirstPref.cpp
 *
 *  Request listener for command "/move-fpref"
 *  Находит первый ключ с префиксом `from` и заменяет `from` на префикс `to`
 *  Возвращает статус операции (произошли ли ошибки при выполнении) и пару:
 *  Базу ключа и значение
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/queueServer
 */

#include "../include.h"
#include "RequestMoveFirstPref.h"
//#include "../utils/_ShowType.h"

using namespace RocksUtils;


namespace RocksServer {

    /**
     *  Runs request listener
     *  @param       protocol in object
     *  @param       protocol out object
     */
    void RequestMoveFirstPref::run(const ProtocolInPostKeys &in, const ProtocolOut &out) noexcept
    {
        // Detect if current method is POST and any data transfered
        if( !in.check(out) ) {
            out.fail();
            return;
        }

        rocksdb::Slice from, to;
        tolist(from, to) = in;

        if(from.empty() || to.empty()) {
            out.fail();
            out.setValue("empty key(s)");
            EvLogger::writeLog("empty key(s)");
            return;
        }

        auto prefix = *in.begin();
        auto iter(db.newIter());
        iter->Seek(prefix);
        
        // Check validity of the key
        if(!iter->Valid() || !iter->key().starts_with(prefix) || !iter->status().ok()) {
            out.ok();
            out.setFailPair();
            return;
        }
        if(from.size() == iter->key().size()) {
            out.fail();
            out.setValue("key prefix is too long");
            EvLogger::writeLog("key prefix is too long");
            return;
        }

        rocksdb::Slice key   = iter->key();
        rocksdb::Slice value = iter->value();

        // Get part of the key that comes after `from`
        rocksdb::Slice key_base(key.data() + from.size(), key.size() - from.size());
        
        std::string newkey;
        newkey.reserve(to.size() + key_base.size());
        newkey = to.ToString() + key_base.ToString();

        // create a RocksDB write batch
        Batch batch;
        batch.del(key);
        batch.set(newkey, value);
        if( !db.commit(batch) ) {
            out.fail();
            out.setValue(db.getStatus().data());
            EvLogger::writeLog(db.getStatus().data());
        } else {
            out.ok();
            out.setPair(key_base, value);
        }
    }

}

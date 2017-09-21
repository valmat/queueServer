/**
 *  RequestFirstPref.h
 *
 *  Request listener for command "/first-pref"
 *  Возвращает первую пару соответствующую префиксу ключа
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */
#pragma once

namespace RocksServer {

    class RequestFirstPref : public RequestBase<ProtocolInGet, ProtocolOut>
    {
    public:
        RequestFirstPref(RocksDBWrapper &rdb) noexcept : db(rdb) {}
        
        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInGet &in, const ProtocolOut &out) noexcept override;

        virtual ~RequestFirstPref() {}
    private:
        RocksDBWrapper& db;
    };

}

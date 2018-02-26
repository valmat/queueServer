/**
 *  RequestMoveFirstPref.h
 *
 *  Request listener for command "/first-pref"
 *  Перемещает значение  по префиксам ключей
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/queueServer
 */
#pragma once

namespace RocksServer {

    class RequestMoveFirstPref : public RequestBase<ProtocolInPostKeys, ProtocolOut>
    {
    public:
        RequestMoveFirstPref(RocksDBWrapper &rdb) noexcept : db(rdb) {}
        
        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInPostKeys &in, const ProtocolOut &out) noexcept override;
        
        virtual ~RequestMoveFirstPref() {}
    private:
        RocksDBWrapper& db;
    };

}

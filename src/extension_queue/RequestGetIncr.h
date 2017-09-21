/**
 *  RequestGetIncr.h
 *
 *  Request listener for command "/get-incr"
 *  Инкрементирует и возвращает полученное значение
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once
 
namespace RocksServer {

    class RequestGetIncr : public RequestBase<ProtocolInPostKeys, ProtocolOut>
    {
    public:
        RequestGetIncr(RocksDBWrapper &rdb) noexcept : db(rdb) {}
        
        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInPostKeys &in, const ProtocolOut &out) noexcept override;

        virtual ~RequestGetIncr() {}
    private:
        RocksDBWrapper& db;
    };

}

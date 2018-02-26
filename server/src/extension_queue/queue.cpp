/**
 *  RocksServer plugin
 *  https://github.com/valmat/queueServer
 */

#include "../include.h"

#include "RequestMoveFirstPref.h"
#include "RequestFirstPref.h"
#include "RequestGetIncr.h"



using namespace RocksServer;

/*
 * Create plugin
 * 
 * @param extension  object of Extension
 * @param rdb        wrapped object of RocksDB
 */
PLUGIN(Extension extension, RocksDBWrapper& rdb)
{
    extension
             .bind("/get-incr",   new RequestGetIncr(rdb))
             .bind("/first-pref", new RequestFirstPref(rdb))
             .bind("/move-fpref", new RequestMoveFirstPref(rdb));
             ;

}
/**
 *  categories.cpp
 *  Управление разделами
 */

#include "../include.h"

#include "RequestMoveFirstPref.h"
#include "RequestFirstPref.h"
#include "RequestGetIncr.h"



using namespace RocksServer;

/*
 * Create plugin
 * 
 * You can use one of the following forms of macro PLUGIN with one, two or three arguments:
 *
 * PLUGIN(Extension extension, RocksDBWrapper& rdb, const IniConfigs& cfg)
 * PLUGIN(Extension extension, RocksDBWrapper& rdb)
 * PLUGIN(Extension extension)
 *
 * The macro `PLUGIN` is defined in rocksserver/api.h. 
 * Instead of using the macro, you can write
 * `extern "C" void plugin(...)` if you like
 *
 * @param extension  object of Extension
 * @param rdb        wrapped object of RocksDB
 * @param cfg        Reference to configuration settings
 */
PLUGIN(Extension extension, RocksDBWrapper& rdb)
{
    extension
             .bind("/get-incr",   new RequestGetIncr(rdb))
             .bind("/first-pref", new RequestFirstPref(rdb))
             .bind("/move-fpref", new RequestMoveFirstPref(rdb));
             ;

}
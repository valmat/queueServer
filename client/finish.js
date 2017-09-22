#!/usr/local/bin/node --max-old-space-size=512

// Вызывается после завершения команды

"use strict"

const options      = require('./configs.js');
const RocksHandler = require('./node_rocks/handler.js');
const handler      = new RocksHandler(options);

const fs         = require('fs');
const lineReader = require('line-reader');

const from_pref  = options.prefixes.famaly + options.prefixes.started;
const to_pref    = options.prefixes.famaly + options.prefixes.finished;




if (!process.argv[4]) {
    console.log("Usage:\n" + process.argv[1] + ' <index> <out_file> <err_file>');
    process.exit(0);
}

const index    = process.argv[2];
const out_file = process.argv[3];
const err_file = process.argv[4];


fs.readFile(out_file, 'utf8', function (err, out_content) {
    if (err) {
        console.error(err);
        out_content = `out_file "${out_file}" does not exist`;
    }
    //console.log(out_content)
    fs.readFile(err_file, 'utf8', function (err, err_content) {
        if (err) {
            console.error(err);
            out_content = `err_file "${err_file}" does not exist`;
        }
        //console.log(err_content)
        //
        // TODO
        // Нужно использовать delset
        //
        const from_key  = from_pref + index;
        const to_key    = to_pref + index;

        handler.get(from_key, function(val) {
            if(null !== val) {
                //console.log({val});
                handler.set(to_key, val, function(ok) {
                    //console.log({ok});
                    if(ok) {
                        handler.del(from_key, function(ok) {
                            if(ok) {
                                console.error(`moved ${from_key} --> ${to_key}`);

                                const contens  = [
                                    [options.prefixes.famaly + options.prefixes.stderr + index, err_content],
                                    [options.prefixes.famaly + options.prefixes.stdout + index, out_content]
                                ];
                                handler.setMap(contens, function(ok) {
                                    console.error({save_otputs: ok, index});
                                });

                                // Функция вызывается на этап завершения
                                options.run.finish(index, val);
                            } else {
                                console.error(`fail ${from_key} --> ${to_key}`);
                            }
                        });
                    }
                });
            }
        });
    });
});

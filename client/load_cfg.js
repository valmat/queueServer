// Handle http response and extract data

"use strict"

const fs = require('fs');

function load_cfg(...opts) {
    
    const usage = "Usage:\n"                  +
            process.argv[1]                   +
            opts.map(x => ` <${x}>`).join('') +
            ' [config.file]';

    if (process.argv.length  < opts.length + 2) {
        console.error(usage);
        process.exit(1);
    }

    // Fill object fields
    let obj = {};
    opts.forEach((item, i) => {
        obj[item] = process.argv[2+i];
    });

    // Configs file
    let cfg_file = (process.argv.length  > opts.length + 2) ?
        process.argv[opts.length + 2] :
        'configs.js';

    // `configs.js` --> `./configs.js`
    if('.' !== cfg_file[0] && '/' !== cfg_file[0] ) {
        cfg_file = './'+cfg_file;
    }

    // Check if file exsts
    if (!fs.existsSync(cfg_file) || !fs.statSync(cfg_file).isFile()) {
        console.error(`File ${cfg_file} does not exist`);
        console.error(usage);
        process.exit(1);
    }

    obj['options'] = require(cfg_file);

    return obj;
}

module.exports = load_cfg;
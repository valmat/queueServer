#!/usr/local/bin/node --max-old-space-size=512

"use strict"

function sleep (time) {
  return new Promise((resolve) => setTimeout(resolve, time));
}

// Usage!
sleep(1000).then(() => {
    console.log(`<< ${process.argv.join('@')} >>`)
});

sleep(3000).then(() => {
    console.error('STDERR_' + process.argv[2])
});


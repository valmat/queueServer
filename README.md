
# queueServer

*Simple queue server based on RocksServer.
It implemented as RocksServer plugin for serverside and nodejs scripts as a client.*

Depends:
[RocksServer](https://github.com/valmat/RocksServer) (serverside), [nodejs](https://nodejs.org/) (clientside)


This repository has the goal of simply deploying on the client side. All you need is the client has installed nodejs.

I wrote this queue server for distributed task launch. This is part of my own algorithm for training ensembles of neural networks.

queueServer has a simple purpose -- it atomically assigns tasks. It remembers which task was started, whether it was completed and what result of its execution.


## How to customize
Just edit [configs.ini](server/bin/configs.ini) (serverside), [configs.js](client/configs.js) (clientside)


## License
[BSD](LICENSE)

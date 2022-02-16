# Resources

## Server/Client Architecture

- Existing protocol: [Ultima Online Protocol](http://necrotoolz.sourceforge.net/kairpacketguide/)


When entering the world, the client requests all of the nearby entities from the
world. TCP for movement and simple tasks that don't require immediate approval
from the server. UDP for spell casting, trading and basically everything else.
There should be a place to view the stock market from the offical website.

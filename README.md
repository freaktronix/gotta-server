# gotta-server

a massively multiplayer online role-playing game located in a world of monsters.

## development environment

we're using lxc for developing the server core that means linux is required to build and run the core.
To create the development environment invoke the following commands

    # create/update development environment
    $ bash ./env.sh <container>

    # to join into the container run
    $ lxc-attach -n <container> -- /bin/su -l [user]

## realmd

this is the central authentication system to manage accounts and realmlists.

    make realmd

# documentation

## network protocol

### client to realmd

* client creates a tcp connection to realmd
* * datastream is serialized with google's protobuf
* * * client sends `authReq`
* * * client receives `authResp`

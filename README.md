MacOS prerequisite:

 - download homebrew 

    $ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

- install glfw

    $ brew install glfw

- install molten

    $ brew install molten-vk

- install vulkan

    $ brew install vulkan-loader
    $ brew install vulkan-tools
    $ brew install vulkan-validationlayers
    $ brew install vulkan-headers
    $ brew install vulkan-profiles
    $ brew install vulkan-utility-libraries
    $ brew install vulkan-volk

- install bear for compile_commands.json

    $ brew install bear

- download the vulkan sdk from https://vulkan.lunarg.com/sdk/home and created it in the default location

building the project:

- build the project

    $ make

- build only release

    $ make release

- build only debug

    $ make debug

- list all options

    $ make help

MacOS running the project:

- running release build

    $ source .env.darwin && ./bin/release/build

- running debug build 

    $ source .env.darwin && ./bin/debug/build

- running release test

    $ source .env.darwin && ./bin/release/test

- running debug test

    $ source .env.darwin && ./bin/debug/test

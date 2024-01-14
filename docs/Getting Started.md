# Getting Started
This section will help you get started on your Cevy journey as quilckly as possible. It will walk you through setting up your development environment an writing a simple Cevy app.

# Quick Start
If you want to drive in immediately and already have a working C++ setup, fell free to follow this "quick start" guid. Otherwis, move on to the next page.

### Try the Examples

1. Clone the [Cevy Repository][1]

``` git clone git@github.com:Arthur-Aillet/Cevy.git Cevy```

2. Navigate to the new "Cevy" folder

``` cd Cevy```

3. Switch to the version as you want or the latest version

``` Actually is not possible, we just have one version```

4. Try the exemple in the [R-Type project][2]

### Add Cevy as a Dependency

You can add in your CMakefile or in a specific file a FetchModule included in your CMakefile

```
include(FetchContent)
FetchContent_Declare(
    cevy
    GIT_REPOSITORY https://github.com/Arthur-Aillet/Cevy
    GIT_TAG main (or another the tag version)
)
FetchContent_Populate(cevy)
```






[1]: https://github.com/Arthur-Aillet/Cevy "Title"
[2]: https://github.com/Arthur-Aillet/RType "Title"

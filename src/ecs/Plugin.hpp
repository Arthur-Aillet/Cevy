#pragma once

class App;

class Plugin {
    public:
        virtual void build(App&) = 0;
};

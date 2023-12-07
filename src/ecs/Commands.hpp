#pragma once
#include <functional>
#include <vector>
#include <memory>

#include "App.hpp"

using World = App;
#include "Entity.hpp"


struct Command {
    Command() {};
    virtual void operator()(World& world) = 0;
};

template<typename ...Args>
struct SpawnPack {
    void operator()(World& world) override {
        world.spawn<Args...>();
        /// implement
    };
};

template<typename ...Args>
struct InsertPack {
    Entity entity;
    void operator()(World& world) override {
        world.insert<Args...>(entity);
    };
};

struct Despawn : public Command {
    Entity entity;
    void operator()(World& world) override {
        // world.despawn(entity);
        world.kill_entity(entity);
    };
};

class Commands {
    protected:
        std::vector<std::function<void (World&)>> _custom_queue;
    public:
        template<typename F>
        void add(F func) {
            _custom_queue.push_back(std::function<void (World&)>(func));
        }


};

class EntityCommands {
    protected:
        Entity entity;
        std::vector<std::unique_ptr<Commands>> commands;

    public:
        const Entity& id() const {
            return entity;
        }

        template<typename ...Args>
        EntityCommands& spawnPack() {
            commands.emplace_back(std::make_unique<SpawnPack<Args...>>(entity));
            return *this;
        }

        void despawn() {
            commands.emplace_back(std::make_unique<Despawn>(entity));
        }

        template<typename ...Args>
        EntityCommands& insertPack() {
            commands.emplace_back(std::make_unique<InsertPack<Args...>>(entity));
            return *this;
        }

};

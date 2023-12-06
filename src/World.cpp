#include "./World.hpp"

Entity World::spawn_entity()
{
    size_t pos = _entities.first_free();
    Entity new_e = Entity(pos);

    _entities.insert_at(pos, new_e);
    return new_e;
}

Entity World::entity_from_index(std::size_t idx)
{
    Entity new_e = Entity(idx);

    _entities.insert_at(idx, new_e);
    return new_e;
}

void World::kill_entity(Entity const &e)
{
    for (auto const& [type, data] : _components_arrays) {
        std::get<1>(data)(*this, e);
    }
}

void World::runStage()
{
    std::vector<std::reference_wrapper<system>> curr_sys;

    std::copy_if(
        _systems.begin(),
        _systems.end(),
        std::back_inserter(curr_sys),
        [this](const system& sys) { return std::get<1>(sys) == _stage;});

    /* this part could be multi-threaded */
    for (auto sys : curr_sys) {
        std::get<0>(sys.get())(*this);
    }

    _stage = static_cast<STAGE>(1 + static_cast<int>(_stage) );
}

void World::runStages() {
    _stage = STAGE::First;
    do {
        runStage();
    }
    while (_stage != STAGE::RESET && _stage != STAGE::ABORT);
}

void World::run() {
    while (!_stop) {
        runStages();
    }
}

void World::quit() const {
    _stop = true;
}

void World::abort() {
    _stage = STAGE::ABORT;
    _stop = true;
}

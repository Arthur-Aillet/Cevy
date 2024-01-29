/*
** AgarthaSoftware, 2024
** Cevy
** File description:
** NetworkAction
*/

#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <list>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

#include "App.hpp"
#include "Commands.hpp"
#include "Plugin.hpp"
#include "Query.hpp"
#include "Resource.hpp"
#include "Stage.hpp"
#include "network/CevyNetwork.hpp"
#include "network/Synchroniser.hpp"
#include "network/network.hpp"

/**
 * @brief A handler for network activity
 *
 * Stores and exposes systems mapped to events and actions
 * Controlled through \link cevy::ecs::Commands ecs::Commands \endlink
 *
 */
class cevy::NetworkActions : public ecs::Plugin {
  public:
  /**
   * Whether it acts on behalf of a server or client
   */
  enum class Mode {
    Server,
    Client,
  };

  class exception : public std::exception {
    public:
    exception(const std::string& msg) : _msg(msg) {};
    const char *what() const noexcept override {
      return _msg.c_str();
    }
    protected:
    std::string _msg;
  };

  using Actor = CevyNetwork::ConnectionDescriptor;

  /// Contructor
  // NetworkActions(CevyNetwork &net) : _net(net){};
  NetworkActions(CevyNetwork &net, Synchroniser& sync) : _net(net), _sync(sync) {};

  /// deleted copy-constructor
  NetworkActions(const NetworkActions &) = delete;

  NetworkActions &operator=(const NetworkActions &) = delete;
  NetworkActions &operator=(NetworkActions &&) = delete;
  ~NetworkActions(){};

  using ActionFailureMode = CevyNetwork::ActionFailureMode;
  using EActionFailureMode = ActionFailureMode::EActionFailureMode;

  /// Presume will let an action fail or succeed before the server's explicit response
  enum class Presume {
    idk,
    fail,
    success,
  };

  /**
   *  @brief Action
   *
   * Actions are defined by their key, and can require an additional argument
   */
  template <size_t N = 0, Presume p = Presume::idk, typename Data = std::tuple<>>
  struct Action {
    inline static const uint16_t value = N;
    inline static const Presume presume = p;
    inline static const size_t serialized_size = sum(::cevy::serialized_size<Data>::value);
    using Arg = Data;
  };

  /**
   *  @brief Event
   *
   * Events are defined by their key, and can require an additional argument
   */
  template <size_t N = 0, typename Data = std::tuple<>>
  struct Event {
    inline static const uint16_t value = N;
    inline static const size_t serialized_size = sum(::cevy::serialized_size<Data>::value);
    using Arg = Data;
  };

  using ClientJoin = Event<CevyNetwork::Event::ClientJoin, Actor>;


  void activity_system(ecs::Commands cmd) {
    while (true) {
      auto event = _net.recvEvent();
      if (!event)
        break;
      auto& [id, vec] = event.value();
      vec.resize(std::max(vec.size(), _remote_event_arg_size[id]));
      event_remote(cmd, id, vec);
    }
    while (true) {
      auto action = _net.recvAction();
      if (!action)
        break;
      auto& [actor, state, id, vec] = action.value();
      EActionFailureMode fail = EActionFailureMode(state);
      vec.resize(std::max(vec.size(), _remote_act_arg_size[id]));
      if (fail == EActionFailureMode::Action_Success) {
        actionSuccess_remote(cmd, id, vec);
      } else {
        actionFailure_remote(cmd, fail, id, vec);
      }
    }
  }

  protected:
  void event_remote(ecs::Commands& cmd, uint16_t id, std::vector<uint8_t> &vec) {
    std::cerr << "(INFO)event_remote: treating" << id << std::endl;
    if (id == CevyNetwork::Event::Summon) {
      uint8_t archetype = deserialize<uint8_t>(vec);
      _sync.get().summon(cmd, archetype);
    }
    if (_events.find(id) != _events.end()) {
      _events.find(id)->second(cmd);
    } else if (_remote_events.find(id) != _remote_events.end()) {
      _remote_events.find(id)->second(cmd, vec);
    } else {
      std::cerr << "(WARNING)event_remote: unmapped" << std::endl;
    }
  }

  void actionSuccess_remote(ecs::Commands& cmd, uint16_t id, std::vector<uint8_t> &vec) {
    std::cerr << "(INFO)actionSuccess_remote: treating" << id << std::endl;
    if (_actions.find(id) != _actions.end()) {
      std::get<1>(_actions.find(id)->second)(cmd);
    } else if (_remote_actions.find(id) != _remote_actions.end()) {
      std::get<1>(_remote_actions.find(id)->second)(cmd, vec);
    } else {
      std::cerr << "(WARNING)actionSuccess_remote: unmapped" << std::endl;
    }
  }

  void actionFailure_remote(ecs::Commands& cmd, EActionFailureMode fail, uint16_t id, std::vector<uint8_t> &vec) {
    std::cerr << "(INFO)actionFailure_remote: treating" << id << std::endl;
    if (_actions.find(id) != _actions.end()) {
      std::get<2>(_actions.find(id)->second)(cmd, fail);
    } else if (_remote_actions.find(id) != _remote_actions.end()) {
      std::get<2>(_remote_actions.find(id)->second)(cmd, fail, vec);
    } else {
      std::cerr << "(WARNING)actionFailure_remote: unmapped" << std::endl;
    }
  }


  public:

  virtual void build(cevy::ecs::App &app) override {
    app.add_systems<ecs::core_stage::PreUpdate>(make_function<void, ecs::Commands>([this](auto cmd) -> void { activity_system(cmd);}));
  }

  /**
   * @brief add an action with regular functions
   *
   * @tparam A Action to map to
   * @tparam Arg0 arguments to the server system
   * @tparam Arg1 arguments to the client_success system
   * @tparam Arg2 arguments to the client_fail system
   * @param server server-side system
   * @param client_success system triggered on action success
   * @param client_fail system triggered on action failure
   */
  template <typename A, typename... Arg0, typename... Arg1, typename... Arg2>
  void add_action(
      EActionFailureMode (&&server)(Actor, Arg0...),
      bool (&&client_success)(Arg1...),
      bool (&&client_fail)(EActionFailureMode, Arg2...)) {
    _actions[A::value] = std::make_tuple(
        server_system([server](ecs::Commands &cmd, Actor actor) { return cmd.system_with(server, actor); }),
        system_success([client_success](ecs::Commands &cmd) { return cmd.system(client_success); }),
        system_fail([client_fail](ecs::Commands &cmd, EActionFailureMode fail) { return cmd.system_with(client_fail, fail); }));
  };

  /**
   * @brief add an action with std::function
   *
   * @tparam A Action to map to
   * @tparam Arg0 arguments to the server system
   * @tparam Arg1 arguments to the client_success system
   * @tparam Arg2 arguments to the client_fail system
   * @param server server-side system
   * @param client_success system triggered on action success
   * @param client_fail system triggered on action failure
   */
  template <typename A, typename... Arg0, typename... Arg1, typename... Arg2>
  void add_action(
      std::function<EActionFailureMode(Actor, Arg0...)> server = server_default,
      std::function<bool(Arg1...)> client_success = success_default,
      std::function<bool(EActionFailureMode, Arg2...)> client_fail = failure_default) {
    _actions[A::value] = std::make_tuple(
        server_system([server](ecs::Commands &cmd, Actor actor) { return cmd.system_with(server, actor); }),
        system_success([client_success](ecs::Commands &cmd) { return cmd.system(client_success); }),
        system_fail([client_fail](ecs::Commands &cmd, EActionFailureMode fail) { return cmd.system_with(client_fail, fail); }));
  }

  template<typename T, typename... Ts>
  struct head {
    using value_type = T;
  };

  /**
   * @brief add an action with std::functions, taking an extra parameter
   *
   * @tparam A Action to map to
   * @tparam A::Arg extra argument, specified by the Action
   * @tparam Arg0 arguments to the server system
   * @tparam Arg1 arguments to the client_success system
   * @tparam Arg2 arguments to the client_fail system
   * @param server server-side system
   * @param client_success system triggered on action success
   * @param client_fail system triggered on action failure
   */
  template <typename A, typename... Arg0, typename... Arg1, typename... Arg2>
  void add_action_with(
      std::function<EActionFailureMode(Actor, typename A::Arg, Arg0...)> server,
      std::function<bool(typename A::Arg, Arg1...)> client_success,
      std::function<bool(Arg2...)> client_fail) {

    static_assert(std::is_same<typename head<Arg2...>::value_type, EActionFailureMode>::value, "first arg of fail function must be `EActionFailure`");
    // static_assert(std::is_same<Arg2...[0], EActionFailureMode>::value, "");
    // , typename A::Arg, , );
    super_server_system<typename A::Arg> server_lambda = [server](ecs::Commands &cmd, Actor actor, typename A::Arg given) {
      return cmd.system_with_tuple(server, std::make_tuple(actor, given));
    };
    super_system_success<typename A::Arg> success_lambda = [client_success](ecs::Commands &cmd, typename A::Arg given) {
      return cmd.system_with(client_success, given);
    };
    super_system_fail<typename A::Arg> fail_lambda = [client_fail](ecs::Commands &cmd, std::tuple<EActionFailureMode, typename A::Arg> pair) {
      return cmd.system_with_tuple(client_fail, pair);
    };

    _super_actions[A::value] = std::make_tuple(
      std::make_any<super_server_system<typename A::Arg>>(server_lambda),
      std::make_any<super_system_success<typename A::Arg>>(success_lambda),
      std::make_any<super_system_fail<typename A::Arg>>(fail_lambda)
    );

    _remote_actions[A::value] = std::make_tuple(
      [server_lambda](ecs::Commands &cmd, Actor actor, std::vector<uint8_t> &vec){ typename A::Arg arg = deserialize<typename A::Arg>(vec); return server_lambda(cmd, actor, std::move(arg));},
      [success_lambda](ecs::Commands &cmd, std::vector<uint8_t> &vec){ typename A::Arg arg = deserialize<typename A::Arg>(vec); return success_lambda(cmd, std::move(arg));},
      [fail_lambda](ecs::Commands &cmd, EActionFailureMode fail, std::vector<uint8_t> &vec){ typename A::Arg arg = deserialize<typename A::Arg>(vec); return fail_lambda(cmd, std::make_pair(fail, std::move(arg)));}
    );
    _remote_act_arg_size[A::value] = A::serialized_size;
  }

  /**
   * @brief add an action with regular functions, taking an extra parameter
   *
   * @tparam A Action to map to
   * @tparam A::Arg extra argument, specified by the Action
   * @tparam Arg0 arguments to the server system
   * @tparam Arg1 arguments to the client_success system
   * @tparam Arg2 arguments to the client_fail system
   * @param server server-side system
   * @param client_success system triggered on action success
   * @param client_fail system triggered on action failure
   */
  template <typename A, typename... Arg0, typename... Arg1, typename... Arg2>
  void add_action_with(
      EActionFailureMode (&&server)(Actor actor, typename A::Arg, Arg0...),
      bool (&&client_success)(typename A::Arg, Arg1...) = []() {},
      bool (&&client_fail)(EActionFailureMode, typename A::Arg, Arg2...) = []() {}) {
    super_server_system<typename A::Arg> server_lambda = [server](ecs::Commands &cmd, Actor actor, typename A::Arg given) {
      return cmd.system_with_tuple(server, std::make_tuple(actor, given));
    };
    super_system_success<typename A::Arg> success_lambda = [client_success](ecs::Commands &cmd, typename A::Arg given) {
      return cmd.system_with(client_success, given);
    };

    super_system_fail<typename A::Arg> fail_lambda = [client_fail](ecs::Commands &cmd, std::tuple<EActionFailureMode, typename A::Arg> pair) {
      return cmd.system_with_tuple(client_fail, pair);
    };

    _super_actions[A::value] = std::make_tuple(
      std::make_any<super_server_system<typename A::Arg>>(server_lambda),
      std::make_any<super_system_success<typename A::Arg>>(success_lambda),
      std::make_any<super_system_fail<typename A::Arg>>(fail_lambda)
    );

    _remote_actions[A::value] = std::make_tuple(
      [server_lambda](ecs::Commands &cmd, Actor actor, std::vector<uint8_t> &vec){ typename A::Arg arg = deserialize<typename A::Arg>(vec); return server_lambda(cmd, actor, std::move(arg));},
      [success_lambda](ecs::Commands &cmd, std::vector<uint8_t> &vec){ typename A::Arg arg = deserialize<typename A::Arg>(vec); return success_lambda(cmd, std::move(arg));},
      [fail_lambda](ecs::Commands &cmd, EActionFailureMode fail, std::vector<uint8_t> &vec){ typename A::Arg arg = deserialize<typename A::Arg>(vec); return fail_lambda(cmd, std::make_pair(fail, std::move(arg)));}
    );

    _remote_act_arg_size[A::value] = A::serialized_size;
  }

  /**
   * @brief specify the event join function
   *
   * @param func system to trigger
   */
  template <typename F>
  void on_client_join(F &&func) {
    add_event_with<ClientJoin>(func);
  };


  /**
   * @brief add an event with a regular function
   *
   * @tparam E Event to map to
   * @tparam Args arguments to the server system
   * @param func system to trigger
   */
  template <typename E, typename... Args>
  void add_event(bool (&&func)(Args...)) {
    _events[E::value] = [func](ecs::Commands &cmd) { cmd.system(func); };
  };

  /**
   * @brief add an event with a regular function, taking an extra parameter
   *
   * @tparam E Event to map to
   * @tparam E::Arg extra argument, specified by the Event
   * @tparam Args arguments to the system
   * @param func system to trigger
   */
  template <typename E, typename... Args>
  void add_event_with(bool (&&func)(typename E::Arg, Args...)) {
    super_system_success<typename E::Arg> lambda = [func](ecs::Commands &cmd, typename E::Arg given) {
      return cmd.system_with<typename E::Arg>(func, given);
    };
    _super_events[E::value] =
      std::make_any<super_system_success<typename E::Arg>>(lambda);

    _remote_events[E::value] = [lambda](ecs::Commands &cmd, std::vector<uint8_t> &vec){ typename E::Arg arg = deserialize<typename E::Arg>(vec); return lambda(cmd, arg); };
    _remote_event_arg_size[E::value] = E::serialized_size;
  };


  /**
   * @brief add an event with an std::function, taking an extra parameter
   *
   * @tparam E Event to map to
   * @tparam E::Arg extra argument, specified by the Event
   * @tparam Args arguments to the system
   * @param func system to trigger
   */
  template <typename E, typename... Args>
  void add_event_with(std::function<bool(typename E::Arg, Args...)> func = success_default) {
    super_system_success<typename E::Arg> lambda = [func](ecs::Commands &cmd, typename E::Arg given) {
      return cmd.system_with<typename E::Arg>(func, given);
    };
    _super_events[E::value] =
      std::make_any<super_system_success<typename E::Arg>>(lambda);

    _remote_events[E::value] = [lambda](ecs::Commands &cmd, std::vector<uint8_t> &vec){ typename E::Arg arg = deserialize<typename E::Arg>(vec); return lambda(cmd, arg); };
    _remote_event_arg_size[E::value] = E::serialized_size;
  };

  /**
   * @brief Call an action
   *
   * when run client-side, this will send the action to the server, and preform
   * the presumed system if specified
   *
   * when run server-side, this will perform the server system following which
   * the action is responded to with a failure or success
   *
   * @tparam A Action to call
   * @param cmd a reference to Commands
   */
  template <typename A>
  void action(ecs::Commands &cmd, Actor actor = -1) {
    try {
    if (_mode == Mode::Server) {
        EActionFailureMode ret = std::get<0>(_actions.at(A::value, actor))(cmd, actor);
        if (actor == -1)
          return;
        if (ret == ActionFailureMode::Action_Success) {
          if (A::presume != Presume::success)
            _net.sendActionSuccess(actor, A::value, std::vector<uint8_t>(serialized_size<typename A::Arg>::value));
        } else {
          if (A::presume != Presume::fail)
            _net.sendActionFailure(actor, A::value, ret, std::vector<uint8_t>(serialized_size<typename A::Arg>::value));
        }
      } else {
        if (A::presume == Presume::success)
          std::get<1>(_actions.at(A::value))(cmd);
        else if (A::presume == Presume::fail)
          std::get<2>(_actions.at(A::value))(cmd);
        _net.sendAction(A::value, std::vector<uint8_t>(2 + serialized_size<typename A::Arg>::value));
      }
      cmd.apply();
    } catch (std::out_of_range& e) {
      std::stringstream ss;
      ss << "no action at " << A::value;
      throw exception(ss.str());
    }
  }

  /**
   * @brief Call an action with a given argument
   *
   * the argument must correspond to the one specified by the Action
   *
   * when run client-side, this will send the action to the server, and preform
   * the presumed system if specified
   *
   * when run server-side, this will perform the server system following which
   * the action is responded to with a failure or success
   *
   * the argument will be send, so it must be defined for
   * \link cevy::serialize_impl serialize \endlink
   *
   * @tparam A Action to call
   * @tparam A::Arg Argument required
   * @param cmd a reference to Commands
   * @param given argument to give
   */
  template <typename A>
  void action_with(ecs::Commands &cmd, typename A::Arg given, Actor actor = -1) {
    try {
    auto &server = std::get<0>(_super_actions.at(A::value));
    auto &client_success = std::get<1>(_super_actions.at(A::value));
    auto &client_fail = std::get<2>(_super_actions.at(A::value));
    if (_mode == Mode::Server) {
      const auto &func =
          std::any_cast<super_server_system<typename A::Arg>&>(
              server);
      EActionFailureMode ret = func(cmd, actor, given);
      if (actor == -1)
        return;
      std::vector<uint8_t> block;
      if (ret == ActionFailureMode::Action_Success) {
        if (A::presume != Presume::success)
          _net.sendActionSuccess(actor, A::value);
      } else {
        if (A::presume != Presume::fail)
          _net.sendActionFailure(actor, A::value, ret);
      }
    } else {
      if (A::presume == Presume::success) {
        const auto &func =
            std::any_cast<super_system_success<typename A::Arg>&>(client_success);
        func(cmd, given);
      } else if (A::presume == Presume::fail) {
        const auto &func =
            std::any_cast<super_system_fail<typename A::Arg>&>(client_fail);
        func(cmd, std::make_tuple(ActionFailureMode::Presumed, given));
      }
      std::vector<uint8_t> vec;
      serialize(vec, given);
      if (vec.size() != serialized_size<typename A::Arg>::value)
        throw std::exception();
      _net.sendAction(A::value, vec);
    }
    cmd.apply();
    } catch (std::out_of_range& e) {
      std::stringstream ss;
      ss << "no action at " << A::value;
      throw exception(ss.str());
    }
  }

  /**
   * @brief send an event
   *
   * Event will be sent, and the system will be run on the server and all clients
   *
   * @tparam E Event to send
   * @param cmd a reference to Commands
   */
  template <typename E>
  void event(ecs::Commands &cmd) {
    try {
    _events.at(E::value)(cmd, 0);
    cmd.apply();
    std::vector<uint8_t> vec;
    vec.resize(E::serialized_size);
    _net.sendEvent(E::value, vec);
    } catch (std::out_of_range& e) {
      std::stringstream ss;
      ss << "no event at " << E::value;
      throw exception(ss.str());
    }
  }

  /**
   * @brief Send an event with a given argument
   *
   * the argument must correspond to the one specified by the Event
   *
   * Event will be sent, and the system will be run on the server and all clients
   *
   * the argument will be send, so it must be defined for
   * \link cevy::serialize_impl serialize \endlink
   *
   * @tparam E Event to send
   * @tparam E::Arg Argument required
   * @param cmd a reference to Commands
   * @param given argument to give
   */
  template <typename E>
  void event_with(ecs::Commands &cmd, typename E::Arg given) {
    try {
    const auto &func = std::any_cast<super_system_success<typename E::Arg>&>(
        _super_events.at(E::value));
    func(cmd, given);
    cmd.apply();
    std::vector<uint8_t> vec;
    serialize(vec, given);
    _net.sendEvent(E::value, vec);
    } catch (std::out_of_range& e) {
      std::stringstream ss;
      ss << "no event at " << E::value;
      throw exception(ss.str());
    }
  }

  public:
  template<typename Arg = std::tuple<>, typename... Args>
  using server_function = EActionFailureMode(Arg, Args...);

  template<typename Arg = std::tuple<>, typename... Args>
  using success_function = bool(Arg, Args...);

  template<typename Arg = std::tuple<>, typename... Args>
  using failure_function = bool(EActionFailureMode, Arg, Args...);

  template<typename Arg = std::tuple<>, typename... Args>
  using server_stdfunction = std::function<EActionFailureMode(Arg, Args...)>;

  template<typename Arg = std::tuple<>, typename... Args>
  using success_stdfunction = std::function<bool(Arg, Args...)>;

  template<typename Arg = std::tuple<>, typename... Args>
  using failure_stdfunction = std::function<bool(EActionFailureMode, Arg, Args...)>;

  // static server_stdfunction<> server_default;

  inline static std::function<server_function<>> server_default = [](auto){ return CevyNetwork::ActionFailureMode::Action_Success; };
  inline static std::function<success_function<>> success_default = [](auto){ return true; };
  inline static std::function<failure_function<>> failure_default = [](auto, auto){ return true; };

  protected:
  using server_system = std::function<EActionFailureMode(ecs::Commands &, size_t actor)>;
  using system_success = std::function<bool(ecs::Commands &)>;
  using system_fail = std::function<bool(ecs::Commands &, EActionFailureMode)>;

  template<typename Arg>
  using super_server_system = std::function<EActionFailureMode(ecs::Commands &, size_t actor, Arg)>;
  template<typename Arg>
  using super_system_success = std::function<bool(ecs::Commands &, Arg)>;
  template<typename Arg>
  using super_system_fail = std::function<bool(ecs::Commands &, std::tuple<EActionFailureMode, Arg>)>;

  using remote_server_system = std::function<EActionFailureMode(ecs::Commands &, size_t actor, std::vector<uint8_t>&)>;
  using remote_system_sucess = std::function<bool(ecs::Commands &, std::vector<uint8_t>&)>;
  using remote_system_fail = std::function<bool(ecs::Commands &, EActionFailureMode, std::vector<uint8_t>&)>;

  std::unordered_map<uint16_t, std::tuple<server_system, system_success, system_fail>> _actions;
  std::unordered_map<uint16_t, std::tuple<std::any, std::any, std::any>> _super_actions;
  std::unordered_map<uint16_t, std::tuple<remote_server_system, remote_system_sucess, remote_system_fail>> _remote_actions;
  std::unordered_map<uint16_t, system_success> _events;
  std::unordered_map<uint16_t, std::any> _super_events;
  std::unordered_map<uint16_t, remote_system_sucess> _remote_events;
  std::unordered_map<uint16_t, size_t> _remote_event_arg_size;
  std::unordered_map<uint16_t, size_t> _remote_act_arg_size;
  Mode _mode;
  cevy::CevyNetwork &_net;
  ref<Synchroniser> _sync;
};

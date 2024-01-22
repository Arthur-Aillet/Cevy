# RFC Relating to Network operations on C++evy Engine
---
## Protocols:

C++evy Networking uses 2 network protocols:

 - **UDP**
for most exchanges, including instantaneous action, state, and event information
 - **TCP/IP**
for specific metadata and setup: aquiring a connection, negotiation client/server states
additionnally, TCP/IP may be used for asset streaming, e.g. User Generated content, "Skins", etc.

### Data format

Each UDP packet contains a Payload header followed by Payload data
the payload header holds a value of half the payload size. The payload size is 2 * payload value.
A single padding byte of 0xff is permitted if the 'true' content is of odd length
the allowed maximum payload size is 506 bytes (to be encoded as `253` in the header)

|    Shorthand     |   Meta Object    |         Type        |                 Composition               |
|------------------|------------------|---------------------|-------------------------------------------|
|      Magic       |   Magic number   |     single value    |                   4 bytes                 |
|                  |     Payload      |      composite      |     *Payload header* + *Payload data*     |
|                  |  Payload Header  |      composite      |     *Timestamp* + *Payload Size*          |
|                  |    Timestamp     |     single value    |                   4 bytes                 |
|                  |   Payload size   |     single value    |                   1 bytes                 |
|                  |   Payload data   |      composite      |       2 * [*Payload Size*] bytes          |

The Payload Data itself contains a concatenation of Objects;
|    Shorthand     |   Emitter   |      Object      |         Type        |                 Composition               |
|------------------|-------------|------------------|---------------------|-------------------------------------------|
|      State       |    Server   |   State Update   |      composite      |      `C::State` + Descriptor + Data       |
|      State?      |    Client   |  State Request   |      composite      |      `C::StateRequest` + Descriptor       |
|      Event       |    Server   |      Event       |      composite      |   `C::Event` + Descriptor + Data + Data   |
|      Action      |    Client   |  Client Action   |      composite      |      `C::Action` + Descriptor + Data      |
|      Sucess      |    Server   |  Action Success  |      composite      |   `C::ActionSucess` + Descriptor + Data   |
|     Failure      |    Server   |  Action Failure  |      composite      | `C::ActionFailure` + Descriptor + Reason  |
TODO: metadata objects

Objects are composed of Sub-Objects;

|    Shorthand     |    Sub-Object    |         Type        |           Composition       |
|------------------|------------------|---------------------|-----------------------------|
|       Data       |  Structured data |      composite      |   *Short size* + raw data   |
|       Size       |    Short size    |     single value    |            1 byte           |
|        C         |  Communication   |        Enum         |            1 byte           |
|      Reason      |  Failure reason  |        Enum         |            1 byte           |
|    Descriptor    | Named descriptor |         ID          |            2 bytes          |




---
## Roles:

### Client:
The *client* is responsible for delivering client actions to the server, and updating its internal states as the server dictates.
Client Actions are actions, chosen to be performed by the user, that have an impact on the Server-Wide state, the Player, or any other Player.
These actions are fallible, and the client must act accordingly should the server fail the Client Action.
(A failure mode of Try Again may be made available, this indicates that the client should attempt the action again)
In an effort to decouple Networking from display/UI, some client actions can be assumed to have succeed before receiving confirmation from the server, in order to let the user observe the action in a timely manner.
Other actions, particularly security oriented, can not be assumed until the server has let the action succeed or fail.


### Server:
The *server* is responsible for delivering state and event information to the client,
missing state information should be considered to be out of date by the client.

the server can choose to let fail or succeed Client Actions. By default, it must respond.
the server keeps track of all connect Client Actions, and will relay state-change information triggered by such an action if applicable

### User:
The *user* operates the Client. By default, the user is not privy to all exchanges between client and server. The user may be a human or an other program, for the purposes of this protocol

### Bot:
A *bot* is a special type of client with no user. It is privy to all information and will perform Client Actions based on its internal logic. Bots should be run on the same machine as the server and skip UDP transactions for a more direct IPC. Bots may be run on a different machine through UDP, or on the same machine trough UDP, but care should be taken not to waste ressources.

---

## States, Events and Actions

### State

A State is information specied for a given timepoint, it is defined to be true in that instant, but undefined outside. A State that wasn't specified to have changed can be assumed to be unchanged if the assumption can be corrected afterwards by the Client.

The client may request the server to specify a State. By default, the server is not required to fulfill that request

#### State Communication

|  Emitter  | Communication |      Component     |       Value       |
|-----------|---------------|--------------------|-------------------|
|  Server   |    State      | {named descriptor} | {structured data} |
|           |    1 byte     |       2 bytes      |   variable size   |

|  Emitter  |   Request     |      Component     |
|-----------|---------------|--------------------|
|  Client   |  RequestState | {named descriptor} |
|           |    1 byte     |       2 bytes      |

### Event

An Event is a trigger. It is defined to have happened at a given timepoint.

#### State Communication

|  Emitter  | Communication |      Component     |       Value       |
|-----------|---------------|--------------------|-------------------|
|  Server   |     Event     | {named descriptor} | {structured data} |
|           |    1 byte     |       2 bytes      |   variable size   |

### Client Action

A Client Action is a request, initiated by the client. By default, the server must respond to with a success or failure.
It has one Success response, with an optionnal structured message, and several Failure responses.


#### Client Action Responses

|  Emitter  |   Request     |      Component     |       Value       |
|-----------|---------------|--------------------|-------------------|
|  Client   |     Action    |   {named action}   | {structured data} |
|           |    1 byte     |       2 bytes      |   variable size   |

| Emitter |
|---------|
| Server  |

↘

|   Response    |     Component      |       Value        |                 Meaning                      | Consequence                                                                                                    |
|---------------|--------------------|--------------------|----------------------------------------------|----------------------------------------------------------------------------------------------------------------|
| ActionSuccess |   {named action}   | {optional message} | this action was sucessful                    | the client can operate following this action's sucess                                                          |
| ActionFailure |   {named action}   | ActionUnavailable  | this action failed due to the current states | the client should communicate the error to the user such that they may attempt it again later                  |
| ActionFailure |   {named action}   |   ActionDisabled   | this action is not available for this server | the client should not try this action again                                                                    |
| ActionFailure |   {named action}   |     ActionError    | this action does not exist or is not valid   | the client has generated an erroneous Action                                                                   |
| ActionFailure |   {named action}   |    ActionWaiting   | this action failed due to a time trigger     | the client should communicate the error to the user such that they may attempt it again later                  |
| ActionFailure |   {named action}   |    ActionDelayed   | this action will succeed at a later time     | the client should consider the action failed. The server may later send an ActionSuccess regarding this Action |
| ActionFailure |   {named action}   |        BUSY        | the server refused to process the action     | the client should send the action request again after a small delay                                            |
|    1 byte     |      2 bytes       | 1 byte or variable |                                              |                                                                                                                |

## Built in Events

### Summon Entity

|  Emitter  | Communication |      Component     |         Value         |
|-----------|---------------|--------------------|-----------------------|
|  Server   |     Event     |       Summon       | SyncId + Archetype ID |
|           |    1 byte     |       2 bytes      |   2 bytes + 1 byte    |

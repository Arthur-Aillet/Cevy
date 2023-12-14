# RFC Relating to Network operations on C++evy Engine
---
## Protocols:

C++evy Networking uses 2 network protocols:

 - **UDP**
for most exchanges, including instantaneous action, state, and state change information
 - **TCP/IP**
for specific metadata and setup: aquiring a connection, negotiation client/server states
additionnally, TCP/IP may be used for asset streaming, e.g. User Generated content, "Skins", etc.

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
The *server* is responsible for delivering state and state change information to the client,
missing state information should be considered to be out of date by the client.

the server can choose to let fail or succeed Client Actions, but it must respond.
the server keeps track of all connect Client Actions, and will relay state-change information triggered by such an action if applicable

### User:
The *user* operates the Client. By default, the user is not privy to all exchanges between client and server. The user may be a human or an other program, for the purposes of this protocol

### Bot:
A *bot* is a special type of client with no user. It is privy to all information and will perform Client Actions based on its internal logic. Bots should be run on the same machine as the server and skip UDP transactions for a more direct IPC. Bots may be run on a different machine through UDP, or on the same machine trough UDP, but care should be taken not to waste ressources.

---

## States, State Changes and Actions

### State

A State is information specied for a given timepoint, it is defined to be true in that instant, but undefined outside. A State that wasn't specified to have changed can be assumed to be unchanged if the assumption can be corrected afterwards by the Client.
//TODO: Reword this ?

The client may request the server to specify a State. By default, the server is not required to fulfill that request

#### State Communication

|  Emitter  | Communication |     Component     |       Value       |
|-----------|---------------|-------------------|-------------------|
|  Server   |    State      | {named descritor} | {structured data} |

|  Emitter  |   Request     |     Component     |
|-----------|---------------|-------------------|
|  Client   |  RequestState | {named descritor} |

### State Change

A State Change is an event that specifies a State's value was updated. It is defined to have happened at a given timepoint, but neither Pre and Post values are defined to have been true at any given timepoint

#### State Communication

|  Emitter  | Communication |     Component     |     Pre Value     |     Post Value    |
|-----------|---------------|-------------------|-------------------|-------------------|
|  Server   |  StateChange  | {named descritor} | {structured data} | {structured data} |

### Client Action

A Client Action is a request, initiated by the client, that the server must let succeed or fail. It has one Success response, with an optionnal structured message, and several Failure responses.


#### Client Action Responses

|  Emitter  |   Request     |     Component      |       Value       |
|-----------|---------------|--------------------|-------------------|
|  Client   |     Action    |   {named action}   | {structured data} |


| Emitter |
|---------|
| Server  |

↘

|   Response    |     Component      |       Value        |                 Meaning                      | Consequence                                                                                                          |
|---------------|--------------------|--------------------|----------------------------------------------|----------------------------------------------------------------------------------------------------------------------|
| ActionSuccess |   {named action}   | {optional message} | this action was sucessful                    | the client can operate following this action's sucess                                                                |
| ActionFailure |   {named action}   | ActionUnavailable  | this action failed due to the current states | the client should communicate the error to the user such that they may attempt it again later                        |
| ActionFailure |   {named action}   |   ActionDisabled   | this action is not available for this server | the client should not try this action again                                                                          |
| ActionFailure |   {named action}   |     ActionError    | this action does not exist or is not valid   | the client has generated an erroneous Action                                                                         |
| ActionFailure |   {named action}   |    ActionWaiting   | this action failed due to a time trigger     | the client should communicate the error to the user such that they may attempt it again later                        |
| ActionFailure |   {named action}   |    ActionDelayed   | this action will succeed at a later time     | the client should consider the action failed. The server will send a State Change regarding the sucess of the Action |
| ActionFailure |   {named action}   |        BUSY        | the server refused to process the action     | the client should send the action request again after a small delay                                                  |
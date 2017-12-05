type actorPath;

type actorRef = {. "parent": actorRef, "path": actorPath, "name": string};

type ctx = {
  .
  "sender": Js.nullable(actorRef),
  "parent": actorRef,
  "path": actorPath,
  "self": actorRef,
  "name": string,
  "children": Nact_jsMap.t(string, actorRef)
};

type persistentCtx('msgType) = {
  .
  "sender": Js.nullable(actorRef),
  "parent": actorRef,
  "path": actorPath,
  "self": actorRef,
  "name": string,
  "children": Nact_jsMap.t(string, actorRef),
  "persist": 'msgType => Js.Promise.t(unit),
  "recovering": bool
};

type statefulActor('state, 'msgType) =
  (Js.nullable('state), 'msgType, ctx) => Js.Promise.t('state);

type statelessActor('msgType) = ('msgType, ctx) => Js.Promise.t(unit);

type persistentActor('state, 'msgType) =
  (Js.nullable('state), 'msgType, persistentCtx('msgType)) => Js.Promise.t('state);

[@bs.module "nact"]
external spawn : (actorRef, statefulActor('state, 'msgType), Js.nullable(string)) => actorRef =
  "spawn";

[@bs.module "nact"]
external spawnStateless : (actorRef, statelessActor('msgType), Js.nullable(string)) => actorRef =
  "spawnStateless";

type actor;

[@bs.module "nact/lib/actor"] [@bs.val "Actor"] external actor : actor = "";

[@bs.send] external getSafeTimeout : (actor, int) => int = "getSafeTimeout";

[@bs.module "nact"]
external spawnPersistent :
  (actorRef, persistentActor('state, 'msgType), string, Js.nullable(string)) => actorRef =
  "spawnPersistent";

[@bs.module "nact"] external configurePersistence : ('engine, actorRef) => unit =
  "configurePersistence";

[@bs.module "nact"] external stop : actorRef => unit = "stop";

[@bs.module "nact"] external start : unit => actorRef = "start";

[@bs.module "nact"] external dispatch : (actorRef, 'msgType) => unit = "dispatch";

[@bs.module "nact"]
external query : (actorRef, actorRef => 'msgType, int) => Js.Promise.t('expectedResult) =
  "query";
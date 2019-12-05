let f = (connectionSide: ConnectionSide.t): Js.Json.t =>
  Json.Encode.(
    object_([
      ("node", ConnectionNodeToJson.f(connectionSide.node)),
      ("nib", ConnectionNibToJson.f(connectionSide.nib)),
    ])
  );

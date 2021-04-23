let f = (connectionSide: ConnectionSide.t): Js.Json.t => {
  open Json.Encode
  object_(list{
    ("node", ConnectionNodeToJson.f(connectionSide.node)),
    ("nib", ConnectionNibToJson.f(connectionSide.nib)),
  })
}

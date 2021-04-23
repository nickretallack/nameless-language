let f = (connectionNib: ConnectionNib.t): Js.Json.t => {
  open Json.Encode
  object_(
    switch connectionNib {
    | ValueConnection => list{("type", string("value"))}
    | NibConnection(nibID) => list{("type", string("nib")), ("nibID", string(nibID))}
    | PositionalConnection(index) => list{("type", string("positional")), ("index", int(index))}
    },
  )
}

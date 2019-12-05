let f = (connectionNib: ConnectionNib.t): Js.Json.t =>
  Json.Encode.(
    object_(
      switch (connectionNib) {
      | ValueConnection => [("type", string("value"))]
      | NibConnection(nibID) => [
          ("type", string("nib")),
          ("nibID", string(nibID)),
        ]
      | PositionalConnection(index) => [
          ("type", string("positional")),
          ("index", int(index)),
        ]
      },
    )
  );

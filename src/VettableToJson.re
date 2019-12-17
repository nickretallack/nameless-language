let f = (vettable: Vettable.t): Js.Json.t =>
  Json.Encode.(
    object_([
      ("text", string(vettable.text)),
      ("vetted", bool(vettable.vetted)),
      ("authorID", nullable(string, vettable.authorID)),
    ])
  );

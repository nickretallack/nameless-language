let f = (vettable: Vettable.t): Js.Json.t => {
  open Json.Encode
  object_(list{
    ("text", string(vettable.text)),
    ("vetted", bool(vettable.vetted)),
    ("authorID", nullable(string, vettable.authorID)),
  })
}

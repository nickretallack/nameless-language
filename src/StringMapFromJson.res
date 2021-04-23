let f = (decode: Js.Json.t => 'a, json: Js.Json.t): Belt.Map.String.t<'a> =>
  Belt.Map.String.fromArray(Js.Dict.entries(Json.Decode.dict(decode, json)))

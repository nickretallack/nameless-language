let f = (json: Js.Json.t): Vettable.t => {
  open Json.Decode
  open Vettable
  {
    text: field("text", string, json),
    vetted: field("vetted", bool, json),
    authorID: field("authorID", optional(string), json),
  }
}

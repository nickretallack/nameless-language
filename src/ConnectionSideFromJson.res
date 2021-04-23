let f = (json: Js.Json.t): ConnectionSide.t => {
  open Json.Decode
  open ConnectionSide
  {
    node: field("node", ConnectionNodeFromJson.f, json),
    nib: field("nib", ConnectionNibFromJson.f, json),
  }
}

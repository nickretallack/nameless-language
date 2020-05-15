let f = (json: Js.Json.t): ConnectionSide.t =>
  Json.Decode.(
    ConnectionSide.{
      node: field("node", ConnectionNodeFromJson.f, json),
      nib: field("nib", ConnectionNibFromJson.f, json),
    }
  );

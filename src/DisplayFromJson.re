let f = (json: Js.Json.t): DefinitionDisplay.t =>
  Json.Decode.(
    DefinitionDisplay.{
      inputOrdering: field("inputOrdering", list(string), json),
      outputOrdering: field("outputOrdering", list(string), json),
    }
  );

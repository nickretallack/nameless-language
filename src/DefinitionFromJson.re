let f = (json: Js.Json.t): Definition.t =>
  Json.Decode.(
    Definition.{
      implementation: field("implementation", ImplementationFromJson.f, json),
      documentation: field("documentation", DocumentationFromJson.f, json),
      display: field("display", DisplayFromJson.f, json),
    }
  );

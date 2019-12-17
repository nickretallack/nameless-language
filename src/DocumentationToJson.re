let f = (documentation: Documentation.t): Js.Json.t =>
  Json.Encode.(
    object_([
      ("name", TranslatableToJson.f(documentation.name)),
      ("description", TranslatableToJson.f(documentation.description)),
      ("inputs", MapToJson.f(documentation.inputs, TranslatableToJson.f)),
      ("outputs", MapToJson.f(documentation.outputs, TranslatableToJson.f)),
    ])
  );

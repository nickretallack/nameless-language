open Definition;
open AppActions;
open Helpers;

let component = ReasonReact.statelessComponent("DefinitionHeader");

let round = (x: float) => int_of_float(x +. 0.5);

let convertPrimitiveValueType =
    (primitiveValue: primitiveValue, typeName: string): primitiveValue =>
  switch (typeName) {
  | "text" => TextValue(primitiveValueToString(primitiveValue))
  | "number" =>
    NumberValue(
      try (float_of_string(primitiveValueToString(primitiveValue))) {
      | Failure(_) => 0.0
      },
    )
  | "integer" =>
    IntegerValue(
      switch (primitiveValue) {
      | NumberValue(value) => round(value)
      | _ => int_of_string(primitiveValueToString(primitiveValue))
      },
    )
  | _ => raise(Not_found)
  };

let make =
    (
      ~documentation: documentation,
      ~emit: definitionAction => unit,
      _children,
    ) => {
  ...component,
  render: _self => {
    let changeName = event => emit(ChangeName(getEventValue(event)));

    <div>
      <input
        type_="text"
        className="name"
        value={getTranslated(documentation.name, "en")}
        onChange=changeName
      />
    </div>;
  },
};
